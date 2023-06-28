#include "ObjectIdentification.h"

ObjectIdentification::ObjectIdentification(const std::string& modelConfig, const std::string& modelWeights) {
    net = cv::dnn::readNetFromDarknet(modelConfig, modelWeights);
}

void ObjectIdentification::identifyObjects(const std::string& imagePath) {
    // Load image
    cv::Mat image = cv::imread(imagePath);

    // Create a 4D blob from the image
    cv::Mat blob = cv::dnn::blobFromImage(image, 1 / 255.0, cv::Size(416, 416), cv::Scalar(0, 0, 0), true, false);

    // Set input blob to the network
    net.setInput(blob);

    // Get output layers names
    std::vector<cv::String> outputNames = net.getUnconnectedOutLayersNames();

    // Forward pass through the network
    std::vector<cv::Mat> detections;
    net.forward(detections, outputNames);

    // Process the detections
    for (const auto& detection : detections) {
        for (int i = 0; i < detection.rows; ++i) {
            const float* data = detection.ptr<float>(i);
            float confidence = data[5];

            // Filter out weak detections
            if (confidence > 0.5) {
                int x = static_cast<int>(data[0] * image.cols);
                int y = static_cast<int>(data[1] * image.rows);
                int width = static_cast<int>(data[2] * image.cols);
                int height = static_cast<int>(data[3] * image.rows);

                // Draw bounding box
                cv::Rect boundingBox(x - width / 2, y - height / 2, width, height);
                cv::rectangle(image, boundingBox, cv::Scalar(0, 255, 0), 2);

                // Print class label and confidence
                std::string label = cv::format("%.2f", confidence);
                cv::putText(image, label, cv::Point(x, y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
            }
        }
    }

    // Display the output image
    cv::imshow("Object Detection", image);
    cv::waitKey(0);
}
