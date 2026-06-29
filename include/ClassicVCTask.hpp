#ifndef CLASSIC_VC_TASK_HPP
#define CLASSIC_VC_TASK_HPP

#include "Task.hpp"

class ClassicVCTask : public CryptographyTask {
public:
    int execute(const std::vector<std::string>& args) override;
};

#endif // CLASSIC_VC_TASK_HPP
