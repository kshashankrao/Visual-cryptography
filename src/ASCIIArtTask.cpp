#include "ASCIIArtTask.hpp"
#include "Utils.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>



int ASCIIArtTask::execute(const std::vector<std::string>& args)
{
    std::string image_path = "assets/map.jpg";
    if (args.size() >= 2)
    {
        image_path = args[1];
    }

    cv::Mat image = utils::loadImage(image_path, cv::IMREAD_COLOR);
    if (image.empty())
    {
        return 1;
    }

    // Resize image to 32x32 for ASCII art representation
    cv::Mat resized;
    cv::resize(image, resized, cv::Size(32, 32), 0, 0, cv::INTER_LINEAR);

    std::cout << "ASCII Art for " << image_path << ":" << std::endl;
    std::cout << std::string(34, '-') << std::endl;

    // Scan rows from bottom to top (to match original BMP scan-line behavior)
    for (int r = resized.rows - 1; r >= 0; r--)
    {
        std::cout << "  ";
        for (int c = 0; c < resized.cols; c++)
        {
            cv::Vec3b pixel = resized.at<cv::Vec3b>(r, c);
            // pixel is in BGR order
            double b = pixel[0] / 256.0;
            double g = pixel[1] / 256.0;
            double r_val = pixel[2] / 256.0;

            double grayScale = 0.3 * r_val + 0.6 * g + 0.11 * b;
            char ch = utils::get_ascii_char(grayScale);
            if (ch == '[')
            {
                ch = 'z';
            }
            std::cout << ch;
        }
        std::cout << std::endl;
    }
    std::cout << std::string(34, '-') << std::endl;

    return 0;
}
