#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <vector>
#include <memory>

class CryptographyTask {
public:
    virtual ~CryptographyTask() = default;
    virtual int execute(const std::vector<std::string>& args) = 0;
};

class TaskFactory {
public:
    static std::unique_ptr<CryptographyTask> createTask(const std::string& mode);
    static void printUsage(const std::string& program_name);
};

#endif // TASK_HPP
