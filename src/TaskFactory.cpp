#include "Task.hpp"
#include "ClassicVCTask.hpp"
#include "DualLayerVCTask.hpp"
#include "ASCIIArtTask.hpp"
#include <iostream>

std::unique_ptr<CryptographyTask> TaskFactory::createTask(const std::string& mode)
{
    if (mode == "classic")
    {
        return std::make_unique<ClassicVCTask>();
    }
    else if (mode == "dual_layer")
    {
        return std::make_unique<DualLayerVCTask>();
    }
    else if (mode == "ascii_art")
    {
        return std::make_unique<ASCIIArtTask>();
    }
    return nullptr;
}

void TaskFactory::printUsage(const std::string& program_name)
{
    std::cout << "Usage: " << program_name << " <mode> [arguments]" << std::endl;
    std::cout << "Available modes:" << std::endl;
    std::cout << "  classic      - Classic (2, 2) Visual Cryptography" << std::endl;
    std::cout << "                 Arguments: <image_path>" << std::endl;
    std::cout << "  dual_layer   - Dual-Layer Visual Cryptography (legible shares)" << std::endl;
    std::cout << "                 Arguments: <secret_image_path>" << std::endl;
    std::cout << "  ascii_art    - ASCII Art Generator" << std::endl;
    std::cout << "                 Arguments: <image_path>" << std::endl;
}
