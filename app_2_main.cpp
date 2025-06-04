#include "app_2.h"
#include <iostream>
#include <string>
#include <sstream>

void printUsage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "./app_2 view_statistics" << std::endl;
    std::cout << "./app_2 filter_statistics <activity ID> <goal ID>" << std::endl;
    std::cout << "./app_2 view_progress <goal ID>" << std::endl;
    std::cout << "./app_2 delete_goal <goal ID>" << std::endl;
    std::cout << "./app_2 backup <file path>" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printUsage();
        return 1;
    }

    std::string command = argv[1];
    App2 app;

    try
    {
        if (command == "view_statistics")
        {
            app.viewStatistics();
        }
        else if (command == "filter_statistics")
        {
            if (argc < 4)
            {
                std::cout << "Missing activity ID or goal ID." << std::endl;
                printUsage();
                return 1;
            }

            int activityId = std::stoi(argv[2]);
            int goalId = std::stoi(argv[3]);
            app.filterStatistics(activityId, goalId);
        }
        else if (command == "view_progress")
        {
            if (argc < 3)
            {
                std::cout << "Missing goal ID." << std::endl;
                printUsage();
                return 1;
            }

            int goalId = std::stoi(argv[2]);
            app.viewProgress(goalId);
        }
        else if (command == "delete_goal")
        {
            if (argc < 3)
            {
                std::cout << "Missing goal ID." << std::endl;
                printUsage();
                return 1;
            }

            int goalId = std::stoi(argv[2]);
            app.deleteGoal(goalId);
        }
        else if (command == "backup")
        {
            if (argc < 3)
            {
                std::cout << "Missing file path." << std::endl;
                printUsage();
                return 1;
            }

            std::string filePath = argv[2];
            app.backupData(filePath);
        }
        else
        {
            std::cout << "Unknown command: " << command << std::endl;
            printUsage();
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred." << std::endl;
        return 1;
    }

    return 0;
}