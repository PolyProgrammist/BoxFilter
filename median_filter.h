#ifndef MEDIANFILTER_MEDIANFILTER_H
#define MEDIANFILTER_MEDIANFILTER_H

#include <opencv2/opencv.hpp>

struct point {
    size_t x, y;
};

class MedianFilter {
public:
    virtual void process(cv::Mat &destination) = 0;
};

void median_filter(cv::Mat &image, cv::Mat &destination);

/*
 * 1.3 sec
 *
 * */
class MedianFilterSimple: public MedianFilter {
public:
    MedianFilterSimple(cv::Mat image, int filter_size);
    void process(cv::Mat &destination) override;
private:
    cv::Mat _image;
    int _filter_size;
private:
};

void median_filter_simple(cv::Mat &image, cv::Mat &destination);

class MedianFilterOptimized: public MedianFilter {
public:
    MedianFilterOptimized(cv::Mat image, int filter_size);
    void process(cv::Mat &destination) override;
private:
    cv::Mat _image;
    int _filter_size;
private:
};

void median_filter_optimized(cv::Mat &image, cv::Mat &destination);

#endif //MEDIANFILTER_MEDIANFILTER_H
