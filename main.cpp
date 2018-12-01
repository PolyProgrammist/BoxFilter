#include <iostream>
#include "box_filter.h"


int main(int argc, char** argv )
{
    if ( argc != 3 )
    {
        std::cout << "usage: BoxFilter <input_image_file_name> <output_image_file_name" << std::endl;
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

    box_filter_simple(image, newimage);
    box_filter_optimized(image, newimage);
    imwrite(argv[2], newimage);

    return 0;
}