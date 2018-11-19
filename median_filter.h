#ifndef MEDIANFILTER_MEDIANFILTER_H
#define MEDIANFILTER_MEDIANFILTER_H

#include <opencv2/opencv.hpp>

struct point {
    size_t x, y;
};

/*
 * 1.3 sec
 *
 * */
class MedianFilterSimple {
public:
    MedianFilterSimple(cv::Mat image, int filter_size);
    void process(cv::Mat &destination);
private:
    cv::Mat _image;
    int _filter_size;
private:
};

void median_filter_simple(cv::Mat &image, cv::Mat &destination);

#endif //MEDIANFILTER_MEDIANFILTER_H
