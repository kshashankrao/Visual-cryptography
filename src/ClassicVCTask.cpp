#include "ClassicVCTask.hpp"
#include "Utils.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cstdlib>

int ClassicVCTask::execute(const std::vector<std::string>& args)
{
    std::string image_path = "assets/0791_orig.png";
    if (args.size() >= 2)
    {
        image_path = args[1];
    }

    cv::Mat imgcolor = utils::loadImage(image_path, cv::IMREAD_COLOR);
    if (imgcolor.empty())
    {
        return 1;
    }

    cv::Mat image_grey;
    cv::cvtColor(imgcolor, image_grey, cv::COLOR_BGR2GRAY);

    cv::Mat imgBin;
    cv::threshold(image_grey, imgBin, 120, 255, cv::THRESH_BINARY);

    std::cout << "Loaded image: " << image_path << " (" << imgBin.cols << "x" << imgBin.rows << ")" << std::endl;

    cv::Mat imgShare1(imgBin.rows, imgBin.cols * 2, CV_8UC1, cv::Scalar(255));
    cv::Mat imgShare2(imgBin.rows, imgBin.cols * 2, CV_8UC1, cv::Scalar(255));

    for (int j = 0; j < imgBin.rows; j++)
    {
        for (int i = 0; i < imgBin.cols; i++)
        {
            int AI = rand() % 2;
            int intensity = imgBin.at<uchar>(j, i);

            if (intensity == 255)
            {
                if (AI == 1)
                {
                    imgShare1.at<uchar>(j, i * 2) = 0;
                    imgShare1.at<uchar>(j, i * 2 + 1) = 255;

                    imgShare2.at<uchar>(j, i * 2) = 0;
                    imgShare2.at<uchar>(j, i * 2 + 1) = 255;
                }
                else
                {
                    imgShare1.at<uchar>(j, i * 2) = 255;
                    imgShare1.at<uchar>(j, i * 2 + 1) = 0;

                    imgShare2.at<uchar>(j, i * 2) = 255;
                    imgShare2.at<uchar>(j, i * 2 + 1) = 0;
                }
            }
            else
            {
                if (AI == 1)
                {
                    imgShare1.at<uchar>(j, i * 2) = 0;
                    imgShare1.at<uchar>(j, i * 2 + 1) = 255;

                    imgShare2.at<uchar>(j, i * 2) = 255;
                    imgShare2.at<uchar>(j, i * 2 + 1) = 0;
                }
                else
                {
                    imgShare1.at<uchar>(j, i * 2) = 255;
                    imgShare1.at<uchar>(j, i * 2 + 1) = 0;

                    imgShare2.at<uchar>(j, i * 2) = 0;
                    imgShare2.at<uchar>(j, i * 2 + 1) = 255;
                }
            }
        }
    }

    cv::Mat final_reconstructed;
    cv::min(imgShare1, imgShare2, final_reconstructed);

    cv::imwrite("classic_share1.png", imgShare1);
    cv::imwrite("classic_share2.png", imgShare2);
    cv::imwrite("classic_reconstructed.png", final_reconstructed);

    std::cout << "Classic Visual Cryptography encryption completed successfully." << std::endl;
    std::cout << "Generated images: classic_share1.png, classic_share2.png, classic_reconstructed.png" << std::endl;

    utils::displayImages({
        {"Share 1", imgShare1},
        {"Share 2", imgShare2},
        {"Reconstructed", final_reconstructed}
    });

    return 0;
}
