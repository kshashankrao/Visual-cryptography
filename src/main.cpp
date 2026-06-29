#include "Task.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv)
{
    // Seed random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    if (argc < 2)
    {
        std::cerr << "Error: Mode argument is required." << std::endl;
        TaskFactory::printUsage(argv[0]);
        return 1;
    }

    std::string mode = argv[1];
    auto task = TaskFactory::createTask(mode);
    if (!task)
    {
        std::cerr << "Error: Unknown mode '" << mode << "'." << std::endl;
        TaskFactory::printUsage(argv[0]);
        return 1;
    }

    // Pass the remaining command line arguments to the task
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);
    }

    return task->execute(args);
}
