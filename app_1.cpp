#include "app_1.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>

// Constructor - initialize and load data
App1::App1()
{
    loadActivities();
    loadGoals();
}

// Destructor - save data to files
App1::~App1()
{
    saveActivities();
    saveGoals();
}

// Add a new activity
bool App1::addActivity(ActivityType type, const std::string &date, double duration,
                       double distance, int repetitions)
{
    // Validate date format
    if (!isDateValid(date))
    {
        std::cerr << "Invalid date format. Please use YYYY-MM-DD." << std::endl;
        return false;
    }

    // Validate duration
    if (duration <= 0)
    {
        std::cerr << "Duration must be greater than 0." << std::endl;
        return false;
    }

    // Activity-specific validation
    if ((type == ActivityType::RUNNING || type == ActivityType::WALKING ||
         type == ActivityType::SWIMMING) &&
        distance <= 0)
    {
        std::cerr << "Distance must be greater than 0 for this activity type." << std::endl;
        return false;
    }

    if (type == ActivityType::STRENGTH && repetitions <= 0)
    {
        std::cerr << "Repetitions must be greater than 0 for strength training." << std::endl;
        return false;
    }

    // Create and add the activity
    Activity newActivity(type, date, duration, distance, repetitions);
    activities.push_back(newActivity);

    std::cout << "Activity added successfully!" << std::endl;
    saveActivities(); // Save immediately
    return true;
}

// View a specific activity
bool App1::viewActivity(int activityId)
{
    if (activityId < 0 || activityId >= activities.size())
    {
        std::cerr << "Invalid activity ID." << std::endl;
        return false;
    }

    const Activity &activity = activities[activityId];

    std::cout << "Activity ID: " << activityId << std::endl;
    std::cout << "Type: " << getActivityTypeName(activity.type) << std::endl;
    std::cout << "Date: " << activity.date << std::endl;
    std::cout << "Duration: " << activity.duration << " minutes" << std::endl;

    if (activity.type == ActivityType::RUNNING || activity.type == ActivityType::WALKING ||
        activity.type == ActivityType::SWIMMING)
    {
        std::cout << "Distance: " << activity.distance << " kilometers" << std::endl;
    }

    if (activity.type == ActivityType::STRENGTH)
    {
        std::cout << "Repetitions: " << activity.repetitions << std::endl;
    }

    return true;
}

// View all activities
bool App1::viewAllActivities()
{
    if (activities.empty())
    {
        std::cout << "No activities recorded yet." << std::endl;
        return false;
    }

    std::cout << "All Activities:" << std::endl;
    std::cout << std::setw(5) << "ID" << " | "
              << std::setw(10) << "Type" << " | "
              << std::setw(12) << "Date" << " | "
              << std::setw(10) << "Duration" << " | "
              << std::setw(10) << "Distance" << " | "
              << std::setw(10) << "Reps" << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    for (size_t i = 0; i < activities.size(); ++i)
    {
        const Activity &activity = activities[i];
        std::cout << std::setw(5) << i << " | "
                  << std::setw(10) << getActivityTypeName(activity.type) << " | "
                  << std::setw(12) << activity.date << " | "
                  << std::setw(10) << activity.duration << " | ";

        if (activity.type == ActivityType::RUNNING || activity.type == ActivityType::WALKING ||
            activity.type == ActivityType::SWIMMING)
        {
            std::cout << std::setw(10) << activity.distance << " | ";
        }
        else
        {
            std::cout << std::setw(10) << "N/A" << " | ";
        }

        if (activity.type == ActivityType::STRENGTH)
        {
            std::cout << std::setw(10) << activity.repetitions << std::endl;
        }
        else
        {
            std::cout << std::setw(10) << "N/A" << std::endl;
        }
    }

    return true;
}

// Add a new goal
bool App1::addGoal(ActivityType type, const std::string &description, const std::string &deadline,
                   int targetReps, double targetDuration, double targetDistance)
{
    // Validate date format
    if (!isDateValid(deadline))
    {
        std::cerr << "Invalid deadline format. Please use YYYY-MM-DD." << std::endl;
        return false;
    }

    // Activity-specific validation
    if ((type == ActivityType::RUNNING || type == ActivityType::WALKING ||
         type == ActivityType::SWIMMING) &&
        targetDistance <= 0)
    {
        std::cerr << "Target distance must be greater than 0 for this activity type." << std::endl;
        return false;
    }

    if (type == ActivityType::STRENGTH && targetReps <= 0)
    {
        std::cerr << "Target repetitions must be greater than 0 for strength training." << std::endl;
        return false;
    }

    if (targetDuration <= 0)
    {
        std::cerr << "Target duration must be greater than 0." << std::endl;
        return false;
    }

    // Create and add the goal
    Goal newGoal(type, description, deadline, targetDistance, targetDuration, targetReps);
    goals.push_back(newGoal);

    std::cout << "Goal added successfully!" << std::endl;
    saveGoals(); // Save immediately
    return true;
}

// View a specific goal
bool App1::viewGoal(int goalId)
{
    if (goalId < 0 || goalId >= goals.size())
    {
        std::cerr << "Invalid goal ID." << std::endl;
        return false;
    }

    const Goal &goal = goals[goalId];

    std::cout << "Goal ID: " << goalId << std::endl;
    std::cout << "Description: " << goal.description << std::endl;
    std::cout << "Activity Type: " << getActivityTypeName(goal.type) << std::endl;
    std::cout << "Deadline: " << goal.deadline << std::endl;
    std::cout << "Target Duration: " << goal.targetDuration << " minutes" << std::endl;

    if (goal.type == ActivityType::RUNNING || goal.type == ActivityType::WALKING ||
        goal.type == ActivityType::SWIMMING)
    {
        std::cout << "Target Distance: " << goal.targetDistance << " kilometers" << std::endl;
    }

    if (goal.type == ActivityType::STRENGTH)
    {
        std::cout << "Target Repetitions: " << goal.targetReps << std::endl;
    }

    std::cout << "Status: " << (goal.achieved ? "Achieved" : "In Progress") << std::endl;

    return true;
}

// View all goals
bool App1::viewAllGoals()
{
    if (goals.empty())
    {
        std::cout << "No goals set yet." << std::endl;
        return false;
    }

    std::cout << "All Goals:" << std::endl;
    std::cout << std::setw(5) << "ID" << " | "
              << std::setw(20) << "Description" << " | "
              << std::setw(10) << "Type" << " | "
              << std::setw(12) << "Deadline" << " | "
              << std::setw(10) << "Duration" << " | "
              << std::setw(10) << "Distance" << " | "
              << std::setw(10) << "Reps" << " | "
              << std::setw(10) << "Status" << std::endl;
    std::cout << std::string(100, '-') << std::endl;

    for (size_t i = 0; i < goals.size(); ++i)
    {
        const Goal &goal = goals[i];
        std::cout << std::setw(5) << i << " | "
                  << std::setw(20) << goal.description << " | "
                  << std::setw(10) << getActivityTypeName(goal.type) << " | "
                  << std::setw(12) << goal.deadline << " | "
                  << std::setw(10) << goal.targetDuration << " | ";

        if (goal.type == ActivityType::RUNNING || goal.type == ActivityType::WALKING ||
            goal.type == ActivityType::SWIMMING)
        {
            std::cout << std::setw(10) << goal.targetDistance << " | ";
        }
        else
        {
            std::cout << std::setw(10) << "N/A" << " | ";
        }

        if (goal.type == ActivityType::STRENGTH)
        {
            std::cout << std::setw(10) << goal.targetReps << " | ";
        }
        else
        {
            std::cout << std::setw(10) << "N/A" << " | ";
        }

        std::cout << std::setw(10) << (goal.achieved ? "Achieved" : "In Progress") << std::endl;
    }

    return true;
}

// Modify an existing goal
bool App1::modifyGoal(int goalId, ActivityType type, const std::string &description,
                      const std::string &deadline, int targetReps, double targetDuration,
                      double targetDistance)
{
    if (goalId < 0 || goalId >= goals.size())
    {
        std::cerr << "Invalid goal ID." << std::endl;
        return false;
    }

    // Validate date format
    if (!isDateValid(deadline))
    {
        std::cerr << "Invalid deadline format. Please use YYYY-MM-DD." << std::endl;
        return false;
    }

    // Activity-specific validation
    if ((type == ActivityType::RUNNING || type == ActivityType::WALKING ||
         type == ActivityType::SWIMMING) &&
        targetDistance <= 0)
    {
        std::cerr << "Target distance must be greater than 0 for this activity type." << std::endl;
        return false;
    }

    if (type == ActivityType::STRENGTH && targetReps <= 0)
    {
        std::cerr << "Target repetitions must be greater than 0 for strength training." << std::endl;
        return false;
    }

    if (targetDuration <= 0)
    {
        std::cerr << "Target duration must be greater than 0." << std::endl;
        return false;
    }

    // Update the goal
    Goal &goal = goals[goalId];
    goal.type = type;
    goal.description = description;
    goal.deadline = deadline;
    goal.targetReps = targetReps;
    goal.targetDuration = targetDuration;
    goal.targetDistance = targetDistance;
    goal.achieved = false; // Reset achievement status after modification

    std::cout << "Goal modified successfully!" << std::endl;
    saveGoals(); // Save immediately
    return true;
}

// Load activities from file
void App1::loadActivities()
{
    activities.clear();
    std::ifstream inFile(activitiesFilename);

    if (!inFile.is_open())
    {
        std::cerr << "Warning: Could not open file " << activitiesFilename << " for reading. Starting with empty activities list." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inFile, line))
    {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> segmentList;

        while (std::getline(ss, segment, ','))
        {
            segmentList.push_back(segment);
        }

        if (segmentList.size() >= 5)
        {
            try
            {
                int typeInt = std::stoi(segmentList[0]);
                ActivityType type = static_cast<ActivityType>(typeInt);
                std::string date = segmentList[1];
                double duration = std::stod(segmentList[2]);
                double distance = std::stod(segmentList[3]);
                int repetitions = std::stoi(segmentList[4]);

                Activity activity(type, date, duration, distance, repetitions);
                activities.push_back(activity);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error parsing line: " << line << " - " << e.what() << std::endl;
            }
        }
    }

    inFile.close();
}

// Save activities to file
void App1::saveActivities()
{
    std::ofstream outFile(activitiesFilename);

    if (!outFile.is_open())
    {
        std::cerr << "Error: Could not open file " << activitiesFilename << " for writing." << std::endl;
        return;
    }

    for (const Activity &activity : activities)
    {
        outFile << static_cast<int>(activity.type) << ","
                << activity.date << ","
                << activity.duration << ","
                << activity.distance << ","
                << activity.repetitions << std::endl;
    }

    outFile.close();
}

// Load goals from file
void App1::loadGoals()
{
    goals.clear();
    std::ifstream inFile(goalsFilename);

    if (!inFile.is_open())
    {
        std::cerr << "Warning: Could not open file " << goalsFilename << " for reading. Starting with empty goals list." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inFile, line))
    {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> segmentList;

        while (std::getline(ss, segment, ','))
        {
            segmentList.push_back(segment);
        }

        if (segmentList.size() >= 7)
        {
            try
            {
                int typeInt = std::stoi(segmentList[0]);
                ActivityType type = static_cast<ActivityType>(typeInt);
                std::string description = segmentList[1];
                std::string deadline = segmentList[2];
                int targetReps = std::stoi(segmentList[3]);
                double targetDuration = std::stod(segmentList[4]);
                double targetDistance = std::stod(segmentList[5]);
                bool achieved = (segmentList[6] == "1");

                Goal goal(type, description, deadline, targetDistance, targetDuration, targetReps);
                goal.achieved = achieved;
                goals.push_back(goal);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error parsing line: " << line << " - " << e.what() << std::endl;
            }
        }
    }

    inFile.close();
}

// Save goals to file
void App1::saveGoals()
{
    std::ofstream outFile(goalsFilename);

    if (!outFile.is_open())
    {
        std::cerr << "Error: Could not open file " << goalsFilename << " for writing." << std::endl;
        return;
    }

    for (const Goal &goal : goals)
    {
        outFile << static_cast<int>(goal.type) << ","
                << goal.description << ","
                << goal.deadline << ","
                << goal.targetReps << ","
                << goal.targetDuration << ","
                << goal.targetDistance << ","
                << (goal.achieved ? "1" : "0") << std::endl;
    }

    outFile.close();
}

// Helper to get activity type name
std::string App1::getActivityTypeName(ActivityType type)
{
    switch (type)
    {
    case ActivityType::RUNNING:
        return "Running";
    case ActivityType::WALKING:
        return "Walking";
    case ActivityType::SWIMMING:
        return "Swimming";
    case ActivityType::CARDIO:
        return "Cardio";
    case ActivityType::STRENGTH:
        return "Strength";
    default:
        return "Unknown";
    }
}

// Validate date format (YYYY-MM-DD)
bool App1::isDateValid(const std::string &date)
{
    if (date.length() != 10)
        return false;
    if (date[4] != '-' || date[7] != '-')
        return false;

    try
    {
        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));

        if (year < 1900 || year > 2100)
            return false;
        if (month < 1 || month > 12)
            return false;
        if (day < 1 || day > 31)
            return false;

        // Check days in month (simplified)
        if (month == 2)
        {
            bool isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
            if (day > (isLeapYear ? 29 : 28))
                return false;
        }
        else if (month == 4 || month == 6 || month == 9 || month == 11)
        {
            if (day > 30)
                return false;
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}