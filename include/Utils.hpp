#ifndef UTILS_HPP
#define UTILS_HPP

#include <opencv2/core.hpp>
#include <string>
#include <vector>
#include <utility>

namespace utils {

// Load image safely with error logging
cv::Mat loadImage(const std::string& path, int flags = cv::IMREAD_COLOR);

// Safely display images (catches errors in headless environments)
void displayImages(const std::vector<std::pair<std::string, cv::Mat>>& window_image_pairs);

// Map a gray scale value to ASCII character
char get_ascii_char(double gray_scale);

// Count white and black pixels in a single-channel Mat
void calculate_intensity(const cv::Mat& im, int& white_count, int& black_count);

} // namespace utils

#endif // UTILS_HPP
