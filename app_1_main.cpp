#include "app_1.h"
#include <iostream>
#include <string>
#include <sstream>

void printUsage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "./app_1 add_activity <activity ID>" << std::endl;
    std::cout << "./app_1 view_activity <activity ID>" << std::endl;
    std::cout << "./app_1 view_activities" << std::endl;
    std::cout << "./app_1 add_goal <goal ID> <activity ID> <description> <deadline> <target repetitions> <target duration> <target distance>" << std::endl;
    std::cout << "./app_1 view_goal <goal ID>" << std::endl;
    std::cout << "./app_1 view_goals" << std::endl;
    std::cout << "./app_1 modify_goal <goal ID> <activity ID> <description> <deadline> <target repetitions> <target duration> <target distance>" << std::endl;
}

ActivityType getActivityTypeFromId(int id)
{
    if (id >= 0 && id <= 4)
    {
        return static_cast<ActivityType>(id);
    }
    return ActivityType::UNKNOWN;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printUsage();
        return 1;
    }

    std::string command = argv[1];
    App1 app;

    try
    {
        if (command == "add_activity")
        {
            if (argc < 3)
            {
                std::cout << "Missing activity ID." << std::endl;
                printUsage();
                return 1;
            }

            int activityTypeId = std::stoi(argv[2]);
            ActivityType type = getActivityTypeFromId(activityTypeId);

            if (type == ActivityType::UNKNOWN)
            {
                std::cout << "Invalid activity type ID. Use 0-4 for Running, Walking, Swimming, Cardio, Strength." << std::endl;
                return 1;
            }

            // Interactive mode to collect activity details
            std::string date;
            double duration = 0.0;
            double distance = 0.0;
            int repetitions = 0;

            std::cout << "Enter date (YYYY-MM-DD): ";
            std::getline(std::cin, date);

            std::cout << "Enter duration (minutes): ";
            std::cin >> duration;

            if (type == ActivityType::RUNNING || type == ActivityType::WALKING || type == ActivityType::SWIMMING)
            {
                std::cout << "Enter distance (kilometers): ";
                std::cin >> distance;
            }

            if (type == ActivityType::STRENGTH)
            {
                std::cout << "Enter repetitions: ";
                std::cin >> repetitions;
            }

            // Add the activity
            if (app.addActivity(type, date, duration, distance, repetitions))
            {
                std::cout << "Activity added successfully." << std::endl;
            }
        }
        else if (command == "view_activity")
        {
            if (argc < 3)
            {
                std::cout << "Missing activity ID." << std::endl;
                printUsage();
                return 1;
            }

            int activityId = std::stoi(argv[2]);
            app.viewActivity(activityId);
        }
        else if (command == "view_activities")
        {
            app.viewAllActivities();
        }
        else if (command == "add_goal")
        {
            if (argc < 8)
            {
                std::cout << "Missing parameters for adding a goal." << std::endl;
                printUsage();
                return 1;
            }

            int goalId = std::stoi(argv[2]);
            int activityTypeId = std::stoi(argv[3]);
            std::string description = argv[4];
            std::string deadline = argv[5];
            int targetReps = std::stoi(argv[6]);
            double targetDuration = std::stod(argv[7]);
            double targetDistance = 0.0;

            if (argc > 8)
            {
                targetDistance = std::stod(argv[8]);
            }

            ActivityType type = getActivityTypeFromId(activityTypeId);

            if (type == ActivityType::UNKNOWN)
            {
                std::cout << "Invalid activity type ID. Use 0-4 for Running, Walking, Swimming, Cardio, Strength." << std::endl;
                return 1;
            }

            if (app.addGoal(type, description, deadline, targetReps, targetDuration, targetDistance))
            {
                std::cout << "Goal added successfully." << std::endl;
            }
        }
        else if (command == "view_goal")
        {
            if (argc < 3)
            {
                std::cout << "Missing goal ID." << std::endl;
                printUsage();
                return 1;
            }

            int goalId = std::stoi(argv[2]);
            app.viewGoal(goalId);
        }
        else if (command == "view_goals")
        {
            app.viewAllGoals();
        }
        else if (command == "modify_goal")
        {
            if (argc < 8)
            {
                std::cout << "Missing parameters for modifying a goal." << std::endl;
                printUsage();
                return 1;
            }

            int goalId = std::stoi(argv[2]);
            int activityTypeId = std::stoi(argv[3]);
            std::string description = argv[4];
            std::string deadline = argv[5];
            int targetReps = std::stoi(argv[6]);
            double targetDuration = std::stod(argv[7]);
            double targetDistance = 0.0;

            if (argc > 8)
            {
                targetDistance = std::stod(argv[8]);
            }

            ActivityType type = getActivityTypeFromId(activityTypeId);

            if (type == ActivityType::UNKNOWN)
            {
                std::cout << "Invalid activity type ID. Use 0-4 for Running, Walking, Swimming, Cardio, Strength." << std::endl;
                return 1;
            }

            if (app.modifyGoal(goalId, type, description, deadline, targetReps, targetDuration, targetDistance))
            {
                std::cout << "Goal modified successfully." << std::endl;
            }
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