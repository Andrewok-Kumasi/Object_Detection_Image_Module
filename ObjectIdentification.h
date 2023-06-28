#ifndef OBJECT_IDENTIFICATION_H
#define OBJECT_IDENTIFICATION_H

#include <opencv2/opencv.hpp>

class ObjectIdentification {
public:
    ObjectIdentification(const std::string& modelConfig, const std::string& modelWeights);
    void identifyObjects(const std::string& imagePath);

private:
    cv::dnn::Net net;
};

#endif  // OBJECT_IDENTIFICATION_H
