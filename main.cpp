#include <iostream>
#include "median_filter.h"


int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        std::cout << "usage: DisplayImage.out <Image_Path>" << std::endl;
        return -1;
    }

    cv::Mat image;
    image = cv::imread(argv[1], 0);

    if ( !image.data )
    {
        std::cout << "No image data" << std::endl;
        return -1;
    }

    cv::Mat newimage(image.rows, image.cols, CV_8UC1);

    median_filter_simple(image, newimage);
    imwrite("output/output_simple.png", newimage);
    for (int i = 0; i < 10; i++) {
        median_filter_optimized(image, newimage);
    }
    imwrite("output/output_optimized.png", newimage);

    return 0;
}