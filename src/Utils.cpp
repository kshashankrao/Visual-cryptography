#include "Utils.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>

namespace utils {

cv::Mat loadImage(const std::string& path, int flags)
{
    cv::Mat img = cv::imread(path, flags);
    if (img.empty())
    {
        std::cerr << "Error: Could not read image from path: " << path << std::endl;
    }
    return img;
}

void displayImages(const std::vector<std::pair<std::string, cv::Mat>>& window_image_pairs)
{
    try
    {
        for (const auto& pair : window_image_pairs)
        {
            cv::namedWindow(pair.first, cv::WINDOW_AUTOSIZE);
            cv::imshow(pair.first, pair.second);
        }
        if (!window_image_pairs.empty())
        {
            cv::waitKey(0);
        }
    }
    catch (...)
    {
        std::cout << "Note: GUI windows are not supported in this environment. Skipping display." << std::endl;
    }
}

char get_ascii_char(double gray_scale)
{
    const double MAX_GRAYSCALE = 0.3 * (255.0 / 256.0) + 0.6 * (255.0 / 256.0) + 0.11 * (255.0 / 256.0);
    if (gray_scale > 0.6)
    {
        return static_cast<char>(65 + (gray_scale - 0.6) * (1 * 26 / (MAX_GRAYSCALE - 0.6)));
    }
    else if (gray_scale >= 0.3)
    {
        return static_cast<char>(97 + (gray_scale - 0.3) * (1 * 26 / 0.3));
    }
    else
    {
        return static_cast<char>(32 + (gray_scale * (1 * 16 / 0.3)));
    }
}

void calculate_intensity(const cv::Mat& im, int& white_count, int& black_count)
{
    white_count = 0;
    black_count = 0;
    for (int j = 0; j < im.rows; j++)
    {
        for (int i = 0; i < im.cols; i++)
        {
            int intensity = im.at<uchar>(j, i);
            if (intensity == 0)
            {
                black_count++;
            }
            else
            {
                white_count++;
            }
        }
    }
}

} // namespace utils
