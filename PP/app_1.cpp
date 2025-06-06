#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>
#include "activity.h"
#include "CoreTracker.h"
#include "Color.h"

int main(int argc, char *argv[])
{
    CoreTracker tracker;

    if (argc < 2)
    {
        std::cout << Color::BOLD + Color::CYAN + "Usage: " << argv[0] << " <command> [arguments]" + Color::RESET << "\n";
        std::cout << Color::BOLD + Color::YELLOW + "Commands:" + Color::RESET << "\n";
        std::cout << Color::GREEN + "  add_activity" + Color::RESET + " <activity_ID>\n";
        std::cout << Color::GREEN + "  view_activity" + Color::RESET + " <activity_ID>\n";
        std::cout << Color::GREEN + "  view_activities" + Color::RESET << "\n";
        std::cout << Color::MAGENTA + "  add_goal" + Color::RESET + " <goal_ID> <activity_ID> <description> <deadline> <target_reps> <target_duration> <target_distance>\n";
        std::cout << Color::MAGENTA + "  view_goal" + Color::RESET + " <goal_ID>\n";
        std::cout << Color::MAGENTA + "  view_goals" + Color::RESET << "\n";
        std::cout << Color::MAGENTA + "  modify_goal" + Color::RESET + " <goal_ID> <activity_ID> <description> <deadline> <target_reps> <target_duration> <target_distance>\n";
        return 1;
    }

    std::string command = argv[1];

    if (command == "add_activity" && argc >= 3)
    {
        int id = std::stoi(argv[2]);
        tracker.addActivity(id);
    }
    else if (command == "view_activity" && argc >= 3)
    {
        int id = std::stoi(argv[2]);
        tracker.viewActivity(id);
    }
    else if (command == "view_activities")
    {
        tracker.viewActivities();
    }
    else if (command == "add_goal" && argc >= 9)
    {
        int goalId = std::stoi(argv[2]);
        int activityId = std::stoi(argv[3]);
        std::string description = argv[4];
        std::string deadline = argv[5];
        int targetReps = std::stoi(argv[6]);
        double targetDuration = std::stod(argv[7]);
        double targetDistance = std::stod(argv[8]);
        tracker.addGoal(goalId, activityId, description, deadline, targetReps, targetDuration, targetDistance);
    }
    else if (command == "view_goal" && argc >= 3)
    {
        int id = std::stoi(argv[2]);
        tracker.viewGoal(id);
    }
    else if (command == "view_goals")
    {
        tracker.viewGoals();
    }
    else if (command == "modify_goal" && argc >= 9)
    {
        int goalId = std::stoi(argv[2]);
        int activityId = std::stoi(argv[3]);
        std::string description = argv[4];
        std::string deadline = argv[5];
        int targetReps = std::stoi(argv[6]);
        double targetDuration = std::stod(argv[7]);
        double targetDistance = std::stod(argv[8]);
        tracker.modifyGoal(goalId, activityId, description, deadline, targetReps, targetDuration, targetDistance);
    }
    else
    {
        std::cout << Color::RED << "Invalid command or insufficient arguments." << Color::RESET << "\n";
        return 1;
    }

    return 0;
}