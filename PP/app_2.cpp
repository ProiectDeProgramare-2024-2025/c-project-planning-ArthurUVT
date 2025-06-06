#include <iostream>
#include <string>
#include "AdvancedTracker.h"
#include "activity.h"

int main(int argc, char *argv[])
{
    AdvancedTracker tracker;

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <command> [arguments]\n";
        std::cout << "Commands:\n";
        std::cout << "  view_statistics\n";
        std::cout << "  filter_statistics <activity_ID> <goal_ID>\n";
        std::cout << "  view_progres <goal_ID>\n";
        std::cout << "  delete_goal <goal_ID>\n";
        std::cout << "  backup <file_path>\n";
        return 1;
    }

    std::string command = argv[1];

    if (command == "view_statistics")
    {
        tracker.viewStatistics();
    }
    else if (command == "filter_statistics" && argc >= 4)
    {
        int activityId = std::stoi(argv[2]);
        int goalId = std::stoi(argv[3]);
        tracker.filterStatistics(activityId, goalId);
    }
    else if (command == "view_progres" && argc >= 3)
    {
        int goalId = std::stoi(argv[2]);
        tracker.viewProgress(goalId);
    }
    else if (command == "delete_goal" && argc >= 3)
    {
        int goalId = std::stoi(argv[2]);
        tracker.deleteGoal(goalId);
    }
    else if (command == "backup" && argc >= 3)
    {
        std::string filePath = argv[2];
        tracker.backup(filePath);
    }
    else
    {
        std::cout << "Invalid command or insufficient arguments.\n";
        return 1;
    }

    return 0;
}