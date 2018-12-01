#include "box_filter.h"

#include <opencv2/opencv.hpp>
#include <chrono>

void box_filter(BoxFilter &mf, cv::Mat &destination) {
    auto start = std::chrono::high_resolution_clock::now();
    mf.process(destination);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;
}

BoxFilterSimple::BoxFilterSimple(cv::Mat image, int filter_size) : _image(image), _filter_size(filter_size) {
    if (image.channels() > 1) {
        throw std::invalid_argument("Image has more than 1 channel");
    }
    if (filter_size % 2 != 1) {
        throw std::invalid_argument("Size of the filter is not odd");
    }
}

void BoxFilterSimple::process(cv::Mat &destination) {
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
            uint64_t sum = 0;
            for (int i = 0; i < _filter_size; i++) {
                for (int j = 0; j < _filter_size; j++) {
                    sum += _image.at<uchar>(y + i - _filter_size / 2, x + j - _filter_size / 2);
                }
            }
            destination.at<uchar>(y, x) = static_cast<uchar>(sum / 25.0);
        }
    }
}

void box_filter_simple(cv::Mat &image, cv::Mat &destination) {
    BoxFilterSimple sp(image, 5);
    box_filter(sp, destination);
}

BoxFilterOptimized::BoxFilterOptimized(cv::Mat image, int filter_size) : _image(image), _filter_size(filter_size) {
    if (image.channels() > 1) {
        throw std::invalid_argument("Image has more than 1 channel");
    }
    if (filter_size != 5) {
        throw std::invalid_argument("Size of the filter is not odd");
    }
}

void BoxFilterOptimized::process(cv::Mat &destination) {
    const int filter_size_half = _filter_size / 2;
    const int minus_filter_size_half = -filter_size_half;
    const int row_ending = _image.rows - filter_size_half;
    const int column_ending = _image.cols - filter_size_half;

    for (int y = filter_size_half; y < row_ending; y++) {
        for (int x = filter_size_half; x < column_ending; x++) {
            uint64_t sum = 0;
            for (int i = minus_filter_size_half; i <= filter_size_half; i++) {
                for (int j = minus_filter_size_half; j <= filter_size_half; j++) {
                    sum += _image.at<uchar>(y + i, x + j);
                }
            }
            destination.at<uchar>(y, x) = static_cast<uchar>(sum / 25.0);;
        }
    }
}

void box_filter_optimized(cv::Mat &image, cv::Mat &destination) {
    BoxFilterOptimized sp(image, 5);
    box_filter(sp, destination);
}