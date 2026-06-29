#ifndef ASCII_ART_TASK_HPP
#define ASCII_ART_TASK_HPP

#include "Task.hpp"

class ASCIIArtTask : public CryptographyTask {
public:
    int execute(const std::vector<std::string>& args) override;
};

#endif // ASCII_ART_TASK_HPP
