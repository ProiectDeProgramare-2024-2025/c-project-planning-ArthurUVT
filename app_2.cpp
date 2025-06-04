#include "app_2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <map>
#include <numeric>
#include <cmath>

// Constructor - load data
App2::App2()
{
    loadActivities();
    loadGoals();
}

// Destructor
App2::~App2()
{
    // No need to save anything in App2
}

// View general statistics
bool App2::viewStatistics()
{
    if (activities.empty())
    {
        std::cout << "No activities recorded yet." << std::endl;
        return false;
    }

    // Prepare containers for statistics
    std::map<ActivityType, int> activityCounts;
    std::map<ActivityType, double> totalDurations;
    std::map<ActivityType, double> totalDistances;
    std::map<ActivityType, int> totalReps;

    // Process all activities
    for (const Activity &activity : activities)
    {
        activityCounts[activity.type]++;
        totalDurations[activity.type] += activity.duration;
        totalDistances[activity.type] += activity.distance;
        totalReps[activity.type] += activity.repetitions;
    }

    // Display general statistics
    std::cout << "=== ACTIVITY STATISTICS ===" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    std::cout << "Total Activities: " << activities.size() << std::endl;
    std::cout << std::string(30, '-') << std::endl;

    // Display statistics for each activity type
    for (int i = 0; i <= static_cast<int>(ActivityType::STRENGTH); i++)
    {
        ActivityType type = static_cast<ActivityType>(i);
        if (activityCounts[type] > 0)
        {
            std::cout << "Activity Type: " << getActivityTypeName(type) << std::endl;
            std::cout << "  Count: " << activityCounts[type] << std::endl;
            std::cout << "  Total Duration: " << totalDurations[type] << " minutes" << std::endl;
            std::cout << "  Average Duration: " << (totalDurations[type] / activityCounts[type]) << " minutes" << std::endl;

            if (type == ActivityType::RUNNING || type == ActivityType::WALKING || type == ActivityType::SWIMMING)
            {
                std::cout << "  Total Distance: " << totalDistances[type] << " km" << std::endl;
                std::cout << "  Average Distance: " << (totalDistances[type] / activityCounts[type]) << " km" << std::endl;
                std::cout << "  Average Pace: " << (totalDurations[type] / totalDistances[type]) << " min/km" << std::endl;
            }

            if (type == ActivityType::STRENGTH)
            {
                std::cout << "  Total Repetitions: " << totalReps[type] << std::endl;
                std::cout << "  Average Repetitions: " << (totalReps[type] / activityCounts[type]) << std::endl;
            }

            std::cout << std::string(30, '-') << std::endl;
        }
    }

    return true;
}

// Filter statistics by activity and goal
bool App2::filterStatistics(int activityId, int goalId)
{
    // Check if activity ID is valid
    bool validActivity = (activityId >= 0 && activityId < activities.size());

    // Check if goal ID is valid
    bool validGoal = (goalId >= 0 && goalId < goals.size());

    if (!validActivity && !validGoal)
    {
        std::cerr << "Invalid activity and goal IDs." << std::endl;
        return false;
    }

    std::cout << "=== FILTERED STATISTICS ===" << std::endl;

    // Show activity information
    if (validActivity)
    {
        const Activity &activity = activities[activityId];
        std::cout << "Activity ID: " << activityId << std::endl;
        std::cout << "Type: " << getActivityTypeName(activity.type) << std::endl;
        std::cout << "Date: " << activity.date << std::endl;
        std::cout << "Duration: " << activity.duration << " minutes" << std::endl;

        if (activity.type == ActivityType::RUNNING || activity.type == ActivityType::WALKING ||
            activity.type == ActivityType::SWIMMING)
        {
            std::cout << "Distance: " << activity.distance << " kilometers" << std::endl;
            std::cout << "Pace: " << (activity.duration / activity.distance) << " min/km" << std::endl;
        }

        if (activity.type == ActivityType::STRENGTH)
        {
            std::cout << "Repetitions: " << activity.repetitions << std::endl;
        }

        std::cout << std::string(30, '-') << std::endl;
    }

    // Show goal information
    if (validGoal)
    {
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

        // If the goal is not achieved, show progress
        if (!goal.achieved)
        {
            double completedDuration = 0.0;
            double completedDistance = 0.0;
            int completedReps = 0;

            // Sum up relevant activities
            for (const Activity &activity : activities)
            {
                if (activity.type == goal.type)
                {
                    completedDuration += activity.duration;
                    completedDistance += activity.distance;
                    completedReps += activity.repetitions;
                }
            }

            std::cout << std::string(30, '-') << std::endl;
            std::cout << "Progress:" << std::endl;

            // Duration progress
            double durationPercentage = (completedDuration / goal.targetDuration) * 100.0;
            std::cout << "Duration: " << completedDuration << "/" << goal.targetDuration
                      << " minutes (" << std::fixed << std::setprecision(2) << durationPercentage << "%)" << std::endl;

            // Distance progress (if applicable)
            if (goal.type == ActivityType::RUNNING || goal.type == ActivityType::WALKING ||
                goal.type == ActivityType::SWIMMING)
            {
                double distancePercentage = (completedDistance / goal.targetDistance) * 100.0;
                std::cout << "Distance: " << completedDistance << "/" << goal.targetDistance
                          << " km (" << std::fixed << std::setprecision(2) << distancePercentage << "%)" << std::endl;
            }

            // Repetitions progress (if applicable)
            if (goal.type == ActivityType::STRENGTH)
            {
                double repsPercentage = (static_cast<double>(completedReps) / goal.targetReps) * 100.0;
                std::cout << "Repetitions: " << completedReps << "/" << goal.targetReps
                          << " (" << std::fixed << std::setprecision(2) << repsPercentage << "%)" << std::endl;
            }
        }
    }

    return true;
}

// View progress for a specific goal
bool App2::viewProgress(int goalId)
{
    if (goalId < 0 || goalId >= goals.size())
    {
        std::cerr << "Invalid goal ID." << std::endl;
        return false;
    }

    const Goal &goal = goals[goalId];

    std::cout << "=== GOAL PROGRESS ===" << std::endl;
    std::cout << "Goal ID: " << goalId << std::endl;
    std::cout << "Description: " << goal.description << std::endl;
    std::cout << "Activity Type: " << getActivityTypeName(goal.type) << std::endl;
    std::cout << "Deadline: " << goal.deadline << std::endl;
    std::cout << "Status: " << (goal.achieved ? "Achieved" : "In Progress") << std::endl;

    // Calculate progress
    double completedDuration = 0.0;
    double completedDistance = 0.0;
    int completedReps = 0;

    // Sum up relevant activities
    for (const Activity &activity : activities)
    {
        if (activity.type == goal.type)
        {
            completedDuration += activity.duration;
            completedDistance += activity.distance;
            completedReps += activity.repetitions;
        }
    }

    // Duration progress
    double durationPercentage = (completedDuration / goal.targetDuration) * 100.0;
    durationPercentage = std::min(durationPercentage, 100.0); // Cap at 100%

    std::cout << "\nDuration Progress: " << std::endl;
    std::cout << "[";
    int progressChars = static_cast<int>(durationPercentage / 5.0);
    for (int i = 0; i < 20; i++)
    {
        std::cout << (i < progressChars ? "█" : " ");
    }
    std::cout << "] " << std::fixed << std::setprecision(2) << durationPercentage << "%" << std::endl;
    std::cout << completedDuration << "/" << goal.targetDuration << " minutes" << std::endl;

    // Distance progress (if applicable)
    if (goal.type == ActivityType::RUNNING || goal.type == ActivityType::WALKING ||
        goal.type == ActivityType::SWIMMING)
    {
        double distancePercentage = (completedDistance / goal.targetDistance) * 100.0;
        distancePercentage = std::min(distancePercentage, 100.0); // Cap at 100%

        std::cout << "\nDistance Progress: " << std::endl;
        std::cout << "[";
        progressChars = static_cast<int>(distancePercentage / 5.0);
        for (int i = 0; i < 20; i++)
        {
            std::cout << (i < progressChars ? "█" : " ");
        }
        std::cout << "] " << std::fixed << std::setprecision(2) << distancePercentage << "%" << std::endl;
        std::cout << completedDistance << "/" << goal.targetDistance << " kilometers" << std::endl;
    }

    // Repetitions progress (if applicable)
    if (goal.type == ActivityType::STRENGTH)
    {
        double repsPercentage = (static_cast<double>(completedReps) / goal.targetReps) * 100.0;
        repsPercentage = std::min(repsPercentage, 100.0); // Cap at 100%

        std::cout << "\nRepetitions Progress: " << std::endl;
        std::cout << "[";
        progressChars = static_cast<int>(repsPercentage / 5.0);
        for (int i = 0; i < 20; i++)
        {
            std::cout << (i < progressChars ? "█" : " ");
        }
        std::cout << "] " << std::fixed << std::setprecision(2) << repsPercentage << "%" << std::endl;
        std::cout << completedReps << "/" << goal.targetReps << " repetitions" << std::endl;
    }

    // Show days remaining until deadline
    std::time_t now = std::time(nullptr);
    std::tm tm = {};
    std::istringstream deadlineStream(goal.deadline);
    deadlineStream >> std::get_time(&tm, "%Y-%m-%d");

    if (deadlineStream.fail())
    {
        std::cerr << "Error parsing deadline date." << std::endl;
    }
    else
    {
        std::time_t deadlineTime = std::mktime(&tm);
        double daysRemaining = std::difftime(deadlineTime, now) / (60 * 60 * 24);

        if (daysRemaining > 0)
        {
            std::cout << "\nDays remaining until deadline: " << static_cast<int>(daysRemaining) << std::endl;
        }
        else if (daysRemaining <= 0 && !goal.achieved)
        {
            std::cout << "\nDeadline has passed!" << std::endl;
        }
    }

    return true;
}

// Delete a goal
bool App2::deleteGoal(int goalId)
{
    if (goalId < 0 || goalId >= goals.size())
    {
        std::cerr << "Invalid goal ID." << std::endl;
        return false;
    }

    std::string description = goals[goalId].description;

    // We don't actually delete the goal here; we just mark it for deletion
    // by writing it to a temporary file with a "DELETED" flag

    std::ofstream outFile(goalsFilename + ".tmp");

    if (!outFile.is_open())
    {
        std::cerr << "Error: Could not open temporary file for writing." << std::endl;
        return false;
    }

    for (size_t i = 0; i < goals.size(); ++i)
    {
        if (i != static_cast<size_t>(goalId))
        {
            const Goal &goal = goals[i];
            outFile << static_cast<int>(goal.type) << ","
                    << goal.description << ","
                    << goal.deadline << ","
                    << goal.targetReps << ","
                    << goal.targetDuration << ","
                    << goal.targetDistance << ","
                    << (goal.achieved ? "1" : "0") << std::endl;
        }
    }

    outFile.close();

    // Rename the temporary file to the original file
    std::remove(goalsFilename.c_str());
    std::rename((goalsFilename + ".tmp").c_str(), goalsFilename.c_str());

    std::cout << "Goal '" << description << "' deleted successfully!" << std::endl;

    // Reload goals from file
    loadGoals();

    return true;
}

// Backup data to a file
bool App2::backupData(const std::string &filePath)
{
    // Backup activities
    std::ifstream activitiesInFile(activitiesFilename);
    std::ofstream activitiesOutFile(filePath + "_activities.csv");

    if (!activitiesInFile.is_open())
    {
        std::cerr << "Error: Could not open activities file for reading." << std::endl;
        return false;
    }

    if (!activitiesOutFile.is_open())
    {
        std::cerr << "Error: Could not open backup file for writing." << std::endl;
        activitiesInFile.close();
        return false;
    }

    activitiesOutFile << activitiesInFile.rdbuf();

    activitiesInFile.close();
    activitiesOutFile.close();

    // Backup goals
    std::ifstream goalsInFile(goalsFilename);
    std::ofstream goalsOutFile(filePath + "_goals.csv");

    if (!goalsInFile.is_open())
    {
        std::cerr << "Error: Could not open goals file for reading." << std::endl;
        return false;
    }

    if (!goalsOutFile.is_open())
    {
        std::cerr << "Error: Could not open backup goals file for writing." << std::endl;
        goalsInFile.close();
        return false;
    }

    goalsOutFile << goalsInFile.rdbuf();

    goalsInFile.close();
    goalsOutFile.close();

    std::cout << "Data backed up successfully to:" << std::endl;
    std::cout << "- " << filePath + "_activities.csv" << std::endl;
    std::cout << "- " << filePath + "_goals.csv" << std::endl;

    return true;
}

// Load activities from file
void App2::loadActivities()
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

// Load goals from file
void App2::loadGoals()
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

// Helper to get activity type name
std::string App2::getActivityTypeName(ActivityType type)
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
bool App2::isDateValid(const std::string &date)
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

// Check if date is in range
bool App2::isDateInRange(const std::string &date, const std::string &startDate, const std::string &endDate)
{
    return (date.compare(startDate) >= 0 && date.compare(endDate) <= 0);
}

// Calculate days between two dates
int App2::calculateDaysBetween(const std::string &dateStart, const std::string &dateEnd)
{
    std::tm tmStart = {}, tmEnd = {};
    std::istringstream ssStart(dateStart), ssEnd(dateEnd);

    ssStart >> std::get_time(&tmStart, "%Y-%m-%d");
    ssEnd >> std::get_time(&tmEnd, "%Y-%m-%d");

    if (ssStart.fail() || ssEnd.fail())
    {
        return -1; // Error in parsing
    }

    std::time_t timeStart = std::mktime(&tmStart);
    std::time_t timeEnd = std::mktime(&tmEnd);

    return static_cast<int>(std::difftime(timeEnd, timeStart) / (60 * 60 * 24));
}

// Format duration in minutes to hours and minutes
std::string App2::formatDuration(double minutes)
{
    int hours = static_cast<int>(minutes) / 60;
    int mins = static_cast<int>(minutes) % 60;

    std::ostringstream oss;
    if (hours > 0)
    {
        oss << hours << "h ";
    }
    oss << mins << "m";

    return oss.str();
}

// Generate ASCII progress chart for a goal
void App2::generateProgressChart(int goalId)
{
    // Implementation would go here - omitted for brevity
}

// Generate summary for an activity type
void App2::generateActivitySummary(ActivityType type)
{
    // Implementation would go here - omitted for brevity
}