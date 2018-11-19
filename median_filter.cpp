#include "median_filter.h"

#include <opencv2/opencv.hpp>
#include <chrono>

MedianFilterSimple::MedianFilterSimple(cv::Mat image, int filter_size) : _image(image), _filter_size(filter_size) {
    if (image.channels() > 1) {
        throw std::invalid_argument("Image has more than 1 channel");
    }
    if (filter_size % 2 != 1) {
        throw std::invalid_argument("Size of the filter is not odd");
    }
}

void MedianFilterSimple::process(cv::Mat &destination) {
    std::vector<uchar> filter(_filter_size * _filter_size);
    for (int y = 0; y < _image.rows; y++) {
        for (int x = 0; x < _image.cols; x++) {
            if (
                    y < _filter_size / 2 ||
                    x < _filter_size / 2 ||
                    y + _filter_size / 2 >= _image.rows ||
                    x + _filter_size / 2 >= _image.cols
                    ) {

                continue;
            }
            for (int i = 0; i < _filter_size; i++) {
                for (int j = 0; j < _filter_size; j++) {
                    filter[i * _filter_size + j] = _image.at<uchar>(y + i - _filter_size / 2, x + j - _filter_size / 2);
                }
            }
            std::sort(begin(filter), end(filter));
            destination.at<uchar>(y, x) = filter[_filter_size / 2];
        }
    }
}

void median_filter_simple(cv::Mat &image, cv::Mat &destination) {
    MedianFilterSimple sp(image, 5);
    auto start = std::chrono::high_resolution_clock::now();
    sp.process(destination);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;
}