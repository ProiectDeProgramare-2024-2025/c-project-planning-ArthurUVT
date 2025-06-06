#include "CoreTracker.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "Color.h"

CoreTracker::CoreTracker()
{
    loadActivities();
    loadGoals();
}

CoreTracker::~CoreTracker()
{
    saveActivities();
    saveGoals();
}

// Convert ActivityType to string with color
std::string CoreTracker::colorActivityType(ActivityType type)
{
    switch (type)
    {
    case ActivityType::RUNNING:
        return Color::GREEN + "Running" + Color::RESET;
    case ActivityType::WALKING:
        return Color::BLUE + "Walking" + Color::RESET;
    case ActivityType::SWIMMING:
        return Color::CYAN + "Swimming" + Color::RESET;
    case ActivityType::CARDIO:
        return Color::MAGENTA + "Cardio" + Color::RESET;
    case ActivityType::STRENGTH:
        return Color::YELLOW + "Strength" + Color::RESET;
    default:
        return Color::RED + "Unknown" + Color::RESET;
    }
}

// Load activities from CSV
void CoreTracker::loadActivities()
{
    std::ifstream file(ACTIVITIES_FILE);
    if (!file.is_open())
        return;

    std::string line;
    std::getline(file, line); // Skip header if exists

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;

        while (std::getline(ss, cell, ','))
        {
            row.push_back(cell);
        }

        if (row.size() >= 5)
        {
            Activity activity;
            activity.type = stringToActivityType(row[0]);
            activity.date = row[1];
            activity.duration = std::stod(row[2]);
            activity.distance = std::stod(row[3]);
            activity.repetitions = std::stoi(row[4]);
            activities.push_back(activity);
        }
    }
    file.close();
}

// Save activities to CSV
void CoreTracker::saveActivities()
{
    std::ofstream file(ACTIVITIES_FILE);
    if (!file.is_open())
        return;

    file << "ActivityType,Date,Duration,Distance,Repetitions\n";
    for (const auto &activity : activities)
    {
        file << activityTypeToString(activity.type) << ","
             << activity.date << ","
             << activity.duration << ","
             << activity.distance << ","
             << activity.repetitions << "\n";
    }
    file.close();
}

// Load goals from CSV
void CoreTracker::loadGoals()
{
    std::ifstream file(GOALS_FILE);
    if (!file.is_open())
        return;

    std::string line;
    std::getline(file, line); // Skip header if exists

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;

        while (std::getline(ss, cell, ','))
        {
            row.push_back(cell);
        }

        if (row.size() >= 7)
        {
            Goal goal;
            goal.type = stringToActivityType(row[0]);
            goal.description = row[1];
            goal.deadline = row[2];
            goal.targetDistance = std::stod(row[3]);
            goal.targetDuration = std::stod(row[4]);
            goal.targetReps = std::stoi(row[5]);
            goal.achieved = (row[6] == "1" || row[6] == "true");
            goals.push_back(goal);
        }
    }
    file.close();
}

// Save goals to CSV
void CoreTracker::saveGoals()
{
    std::ofstream file(GOALS_FILE);
    if (!file.is_open())
        return;

    file << "ActivityType,Description,Deadline,TargetDistance,TargetDuration,TargetReps,Achieved\n";
    for (const auto &goal : goals)
    {
        file << activityTypeToString(goal.type) << ","
             << goal.description << ","
             << goal.deadline << ","
             << goal.targetDistance << ","
             << goal.targetDuration << ","
             << goal.targetReps << ","
             << (goal.achieved ? "1" : "0") << "\n";
    }
    file.close();
}

// Add activity with ID
void CoreTracker::addActivity(int id)
{
    std::cout << Color::BOLD + Color::CYAN + "=== Add New Activity (ID: " << id << ") ===" + Color::RESET << "\n";

    Activity activity;
    std::string typeStr;

    std::cout << "Available activity types: ";
    std::cout << Color::GREEN << "Running" << Color::RESET << ", ";
    std::cout << Color::BLUE << "Walking" << Color::RESET << ", ";
    std::cout << Color::CYAN << "Swimming" << Color::RESET << ", ";
    std::cout << Color::MAGENTA << "Cardio" << Color::RESET << ", ";
    std::cout << Color::YELLOW << "Strength" << Color::RESET << "\n";

    std::cout << "Enter activity type: ";
    std::cin >> typeStr;
    activity.type = stringToActivityType(typeStr);

    if (activity.type == ActivityType::UNKNOWN)
    {
        std::cout << Color::RED << "Invalid activity type!" << Color::RESET << "\n";
        return;
    }

    std::cout << "Enter date (YYYY-MM-DD): ";
    std::cin >> activity.date;

    std::cout << "Enter duration (minutes): ";
    std::cin >> activity.duration;

    std::cout << "Enter distance (km, 0 if not applicable): ";
    std::cin >> activity.distance;

    std::cout << "Enter repetitions (0 if not applicable): ";
    std::cin >> activity.repetitions;

    activities.push_back(activity);
    saveActivities();

    std::cout << Color::GREEN << "Activity added successfully!" << Color::RESET << "\n";
}

// View specific activity by ID
void CoreTracker::viewActivity(int id)
{
    if (id < 0 || id >= static_cast<int>(activities.size()))
    {
        std::cout << Color::RED << "Activity ID " << id << " not found!" << Color::RESET << "\n";
        return;
    }

    const auto &activity = activities[id];
    std::cout << Color::BOLD + Color::CYAN + "=== Activity Details (ID: " << id << ") ===" + Color::RESET << "\n";
    std::cout << Color::BOLD << "Type: " << Color::RESET << colorActivityType(activity.type) << "\n";
    std::cout << Color::BOLD << "Date: " << Color::RESET << activity.date << "\n";
    std::cout << Color::BOLD << "Duration: " << Color::RESET << activity.duration << " minutes\n";
    std::cout << Color::BOLD << "Distance: " << Color::RESET << activity.distance << " km\n";
    std::cout << Color::BOLD << "Repetitions: " << Color::RESET << activity.repetitions << "\n";
}

// View all activities
void CoreTracker::viewActivities()
{
    std::cout << Color::BOLD + Color::CYAN + "=== All Activities ===" + Color::RESET << "\n";
    if (activities.empty())
    {
        std::cout << Color::YELLOW << "No activities recorded." << Color::RESET << "\n";
        return;
    }

    std::cout << Color::BOLD << std::left << std::setw(5) << "ID"
              << std::setw(20) << "Type"
              << std::setw(12) << "Date"
              << std::setw(10) << "Duration"
              << std::setw(10) << "Distance"
              << std::setw(8) << "Reps" << Color::RESET << "\n";
    std::cout << std::string(70, '-') << "\n";

    for (size_t i = 0; i < activities.size(); ++i)
    {
        const auto &activity = activities[i];
        std::cout << std::left << std::setw(5) << i
                  << std::setw(20) << colorActivityType(activity.type)
                  << std::setw(12) << activity.date
                  << std::setw(10) << activity.duration
                  << std::setw(10) << activity.distance
                  << std::setw(8) << activity.repetitions << "\n";
    }
}

// Add goal
void CoreTracker::addGoal(int goalId, int activityId, const std::string &description,
                          const std::string &deadline, int targetReps, double targetDuration, double targetDistance)
{

    // Convert activityId to ActivityType (assuming 0=Running, 1=Walking, etc.)
    ActivityType type = static_cast<ActivityType>(activityId);
    if (activityId < 0 || activityId > 4)
    {
        type = ActivityType::UNKNOWN;
    }

    Goal goal(type, description, deadline, targetDistance, targetDuration, targetReps);
    goals.push_back(goal);
    saveGoals();

    std::cout << Color::GREEN << "Goal added successfully with ID: " << goalId << Color::RESET << "\n";
}

// View specific goal by ID
void CoreTracker::viewGoal(int id)
{
    if (id < 0 || id >= static_cast<int>(goals.size()))
    {
        std::cout << Color::RED << "Goal ID " << id << " not found!" << Color::RESET << "\n";
        return;
    }

    const auto &goal = goals[id];
    std::cout << Color::BOLD + Color::CYAN + "=== Goal Details (ID: " << id << ") ===" + Color::RESET << "\n";
    std::cout << Color::BOLD << "Activity Type: " << Color::RESET << colorActivityType(goal.type) << "\n";
    std::cout << Color::BOLD << "Description: " << Color::RESET << goal.description << "\n";
    std::cout << Color::BOLD << "Deadline: " << Color::RESET << goal.deadline << "\n";
    std::cout << Color::BOLD << "Target Distance: " << Color::RESET << goal.targetDistance << " km\n";
    std::cout << Color::BOLD << "Target Duration: " << Color::RESET << goal.targetDuration << " minutes\n";
    std::cout << Color::BOLD << "Target Repetitions: " << Color::RESET << goal.targetReps << "\n";
    std::cout << Color::BOLD << "Achieved: " << Color::RESET << (goal.achieved ? Color::GREEN + "Yes" : Color::RED + "No") << Color::RESET << "\n";
}

// View all goals
void CoreTracker::viewGoals()
{
    std::cout << Color::BOLD + Color::CYAN + "=== All Goals ===" + Color::RESET << "\n";
    if (goals.empty())
    {
        std::cout << Color::YELLOW << "No goals set." << Color::RESET << "\n";
        return;
    }

    std::cout << Color::BOLD << std::left << std::setw(5) << "ID"
              << std::setw(20) << "Type"
              << std::setw(20) << "Description"
              << std::setw(12) << "Deadline"
              << std::setw(10) << "Achieved" << Color::RESET << "\n";
    std::cout << std::string(70, '-') << "\n";

    for (size_t i = 0; i < goals.size(); ++i)
    {
        const auto &goal = goals[i];
        std::cout << std::left << std::setw(5) << i
                  << std::setw(20) << colorActivityType(goal.type)
                  << std::setw(20) << goal.description.substr(0, 18)
                  << std::setw(12) << goal.deadline
                  << std::setw(10) << (goal.achieved ? Color::GREEN + "Yes" + Color::RESET : Color::RED + "No" + Color::RESET) << "\n";
    }
}

// Modify goal
void CoreTracker::modifyGoal(int goalId, int activityId, const std::string &description,
                             const std::string &deadline, int targetReps, double targetDuration, double targetDistance)
{

    if (goalId < 0 || goalId >= static_cast<int>(goals.size()))
    {
        std::cout << Color::RED << "Goal ID " << goalId << " not found!" << Color::RESET << "\n";
        return;
    }

    Goal &goal = goals[goalId];
    goal.type = static_cast<ActivityType>(activityId);
    goal.description = description;
    goal.deadline = deadline;
    goal.targetReps = targetReps;
    goal.targetDuration = targetDuration;
    goal.targetDistance = targetDistance;

    saveGoals();
    std::cout << Color::GREEN << "Goal modified successfully!" << Color::RESET << "\n";
}