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
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", newimage);

    cv::waitKey(0);

    imwrite("output.png", newimage);

    return 0;
}