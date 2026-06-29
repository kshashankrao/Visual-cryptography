#include "Utils.hpp"
#include <iostream>
#include <cassert>
#include <opencv2/core.hpp>

// Simple macro to run a test and print status
#define RUN_TEST(test_func) \
    std::cout << "[ RUN      ] " << #test_func << std::endl; \
    try { \
        test_func(); \
        std::cout << "[       OK ] " << #test_func << std::endl; \
    } catch (const std::exception& e) { \
        std::cerr << "[  FAILED  ] " << #test_func << " with exception: " << e.what() << std::endl; \
        failed_tests++; \
    } catch (...) { \
        std::cerr << "[  FAILED  ] " << #test_func << " with unknown exception" << std::endl; \
        failed_tests++; \
    }

int failed_tests = 0;

void test_get_ascii_char()
{
    // Test boundary behaviors
    char dark = utils::get_ascii_char(0.1);
    char mid = utils::get_ascii_char(0.4);
    char light = utils::get_ascii_char(0.8);

    // Verify mapping range expectations
    // Dark pixels (<0.3) map to non-alphabetic chars (space + printable symbols, ascii 32-48)
    assert(dark >= 32 && dark < 97);
    
    // Mid range [0.3, 0.6] maps to lowercase letters (a-z, ascii 97-122)
    assert(mid >= 'a' && mid <= 'z');

    // Light range (>0.6) maps to uppercase letters (A-Z, ascii 65-90)
    assert(light >= 'A' && light <= 'Z');

    std::cout << "Mapped chars: dark='" << dark << "', mid='" << mid << "', light='" << light << "'" << std::endl;
}

void test_calculate_intensity()
{
    // Create a 10x10 single channel Mat
    cv::Mat mat(10, 10, CV_8UC1, cv::Scalar(255)); // All white

    // Set 35 pixels to black (0)
    for (int i = 0; i < 35; i++)
    {
        mat.at<uchar>(i / 10, i % 10) = 0;
    }

    int white_count = 0;
    int black_count = 0;
    utils::calculate_intensity(mat, white_count, black_count);

    assert(black_count == 35);
    assert(white_count == 65);
}

void test_load_image()
{
    // Test loading non-existent file
    cv::Mat empty_img = utils::loadImage("non_existent_file_path.png");
    assert(empty_img.empty());

    // Test loading valid file
    cv::Mat valid_img = utils::loadImage("assets/0791_orig.png");
    assert(!valid_img.empty());
}

int main()
{
    std::cout << "[==========] Running visual cryptography tests" << std::endl;

    RUN_TEST(test_get_ascii_char);
    RUN_TEST(test_calculate_intensity);
    RUN_TEST(test_load_image);

    if (failed_tests > 0)
    {
        std::cerr << "[==========] Failed " << failed_tests << " tests." << std::endl;
        return 1;
    }

    std::cout << "[==========] All tests passed successfully!" << std::endl;
    return 0;
}
