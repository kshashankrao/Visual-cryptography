#ifndef DUAL_LAYER_VC_TASK_HPP
#define DUAL_LAYER_VC_TASK_HPP

#include "Task.hpp"
#include <opencv2/core.hpp>

class DualLayerVCTask : public CryptographyTask {
public:
    int execute(const std::vector<std::string>& args) override;

private:
    struct AlphabetTemplates {
        std::vector<cv::Mat> negative_images;
        std::vector<cv::Mat> positive_images;
    };

    static const int BLOCK_HEIGHT = 20;
    static const int BLOCK_WIDTH = 20;

    AlphabetTemplates create_alphabet_templates(const std::string& text);
};

#endif // DUAL_LAYER_VC_TASK_HPP
