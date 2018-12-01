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
    const int filter_cells = _filter_size * _filter_size;
    const int filter_size_half = _filter_size / 2;
    const int minus_filter_size_half = -filter_size_half;
    const int row_ending = _image.rows - filter_size_half;
    const int column_ending = _image.cols - filter_size_half;

    uint32_t **s = new uint32_t*[_filter_size + 1];
    for (int i = 0; i < _filter_size + 1; i++) {
        s[i] = new uint32_t[_image.cols];
    }
    for (int j = 0; j < _image.cols; j++) {
        s[0][j] = 0;
    }

    uint32_t *sum = new uint32_t[_image.cols];
    for (int x = 0; x < _image.cols; x++) {
        sum[x] = 0;
    }

    for (int y = 0; y < _filter_size; y++) {
        s[y + 1][0] = _image.at<uchar>(y, 0);
        for (int x = 0; x < _image.cols; x++) {
            s[y + 1][x] = _image.at<uchar>(y, x) + s[y + 1][x - 1];
            sum[x] += s[y + 1][x];
        }
    }

    for (int y = filter_size_half; y < row_ending; y++) {
        if (y > filter_size_half) {
            uint32_t *tmp = s[0];
            for (int i = 0; i < _filter_size; i++) {
                s[i] = s[i + 1];
            }
            s[_filter_size] = tmp;
            s[_filter_size][0] = _image.at<uchar>(y + filter_size_half, 0);
            sum[0] += s[_filter_size][0] - s[0][0];
            for (int x = 1; x < _image.cols; x++) {
                s[_filter_size][x] = _image.at<uchar>(y + filter_size_half, x) + s[_filter_size][x - 1];
                sum[x] += s[_filter_size][x] - s[0][x];
            }
        }

        uint64_t current_sum = sum[filter_size_half + filter_size_half];
        destination.at<uchar>(y, filter_size_half) = static_cast<uchar>(current_sum / static_cast<double>(filter_cells));
        for (int x = filter_size_half + 1; x < column_ending; x++) {
            uint64_t current_sum = sum[x + filter_size_half] - sum[x + minus_filter_size_half - 1];
            destination.at<uchar>(y, x) = static_cast<uchar>(current_sum / static_cast<double>(filter_cells));
        }
    }

    delete [] sum;

    for (int i = 0; i < _filter_size; i++) {
        delete [] s[i];
    }
    delete [] s;
}

void box_filter_optimized(cv::Mat &image, cv::Mat &destination) {
    BoxFilterOptimized sp(image, 5);
    box_filter(sp, destination);
}