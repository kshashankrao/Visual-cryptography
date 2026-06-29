#include "DualLayerVCTask.hpp"
#include "Utils.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>

DualLayerVCTask::AlphabetTemplates DualLayerVCTask::create_alphabet_templates(const std::string& text)
{
    AlphabetTemplates templates;
    for (size_t i = 0; i < text.length(); i++)
    {
        cv::Mat black_background(BLOCK_HEIGHT, BLOCK_WIDTH, CV_8UC1, cv::Scalar(0));
        cv::Mat white_background(BLOCK_HEIGHT, BLOCK_WIDTH, CV_8UC1, cv::Scalar(255));
        
        int flag = 0;
        int thicknessText = 1;
        while (flag != 1)
        {
            cv::Mat dst = white_background.clone();
            cv::putText(dst, std::string(1, text[i]), cv::Point(0.1 * BLOCK_WIDTH, 0.75 * BLOCK_HEIGHT), 
                        cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0), thicknessText);
            
            int white_count = 0, black_count = 0;
            utils::calculate_intensity(dst, white_count, black_count);
            
            if (std::abs(white_count - black_count) < 0.8 * white_count)
            {
                flag = 1;
            }
            else
            {
                thicknessText++;
            }
        }
        
        cv::putText(white_background, std::string(1, text[i]), cv::Point(0.2 * BLOCK_WIDTH, 0.75 * BLOCK_HEIGHT), 
                    cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0), thicknessText);
        cv::putText(black_background, std::string(1, text[i]), cv::Point(0.2 * BLOCK_WIDTH, 0.75 * BLOCK_HEIGHT), 
                    cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255), thicknessText);
        
        templates.negative_images.push_back(white_background);
        templates.positive_images.push_back(black_background);
    }
    return templates;
}

int DualLayerVCTask::execute(const std::vector<std::string>& args)
{
    std::string image_path = "assets/0791_orig.png";
    if (args.size() >= 2)
    {
        image_path = args[1];
    }

    std::string alphabets;
    std::cout << "\nEnter 4 alphabets (Example: ABCD): " << std::endl;
    if (!std::getline(std::cin, alphabets))
    {
        std::cerr << "Error reading alphabets from standard input." << std::endl;
        return 1;
    }

    // Clean up alphabets (remove spaces and convert to uppercase)
    alphabets.erase(std::remove_if(alphabets.begin(), alphabets.end(), ::isspace), alphabets.end());
    std::transform(alphabets.begin(), alphabets.end(), alphabets.begin(), [](unsigned char c) {
        return std::toupper(c);
    });

    if (alphabets.empty())
    {
        std::cerr << "Error: Alphabet string cannot be empty." << std::endl;
        return 1;
    }

    std::cout << "The alphabets are: " << alphabets << std::endl;
    AlphabetTemplates templates = create_alphabet_templates(alphabets);

    cv::Mat img = utils::loadImage(image_path, cv::IMREAD_GRAYSCALE);
    if (img.empty())
    {
        return 1;
    }

    // Resize input image to 200x200
    cv::resize(img, img, cv::Size(200, 200), 0, 0, cv::INTER_LINEAR);

    // Prepare share images
    int target_rows = img.rows * 5; // 1000
    int target_cols = img.cols * 5; // 1000
    cv::Mat share1(target_rows, target_cols, CV_8UC1, cv::Scalar(255));
    cv::Mat share2(target_rows, target_cols, CV_8UC1, cv::Scalar(255));

    // Resize secret image representation to 1000x1000
    cv::Mat img_large;
    cv::resize(img, img_large, cv::Size(target_cols, target_rows), 0, 0, cv::INTER_LINEAR);

    size_t cnt = 0;
    for (int j = 0; j < img_large.rows; j += BLOCK_HEIGHT)
    {
        for (int i = 0; i < img_large.cols; i += BLOCK_WIDTH)
        {
            int AI = rand() % 2;
            int intensity = img_large.at<uchar>(j, i);
            
            cv::Mat key1 = templates.negative_images[cnt];
            cv::Mat key2 = templates.positive_images[cnt];

            if (intensity > 100) // White pixel (background) -> shares are identical (matches template)
            {
                if (AI == 0)
                {
                    key1.copyTo(share1(cv::Rect(i, j, key1.cols, key1.rows)));
                    key1.copyTo(share2(cv::Rect(i, j, key1.cols, key1.rows)));
                }
                else
                {
                    key2.copyTo(share1(cv::Rect(i, j, key2.cols, key2.rows)));
                    key2.copyTo(share2(cv::Rect(i, j, key2.cols, key2.rows)));
                }
            }
            else // Black pixel (secret shape) -> shares are opposite (overlays to solid black)
            {
                if (AI == 1)
                {
                    key1.copyTo(share1(cv::Rect(i, j, key1.cols, key1.rows)));
                    key2.copyTo(share2(cv::Rect(i, j, key2.cols, key2.rows)));
                }
                else
                {
                    key2.copyTo(share1(cv::Rect(i, j, key2.cols, key2.rows)));
                    key1.copyTo(share2(cv::Rect(i, j, key1.cols, key1.rows)));
                }
            }

            cnt++;
            if (cnt == alphabets.length())
            {
                cnt = 0;
            }
        }
    }

    cv::imwrite("imgShare1.png", share1);
    cv::imwrite("imgShare2.png", share2);

    // Reconstruct the secret image by combining share1 and share2 using cv::min
    cv::Mat reconstructed;
    cv::min(share1, share2, reconstructed);
    cv::imwrite("reconstructed.png", reconstructed);

    std::cout << "Dual-Layer Visual Cryptography encryption and reconstruction completed successfully." << std::endl;
    std::cout << "Generated images: imgShare1.png, imgShare2.png, reconstructed.png" << std::endl;

    utils::displayImages({
        {"Encrypted Image 1", share1},
        {"Encrypted Image 2", share2},
        {"Reconstructed Image", reconstructed}
    });

    return 0;
}
