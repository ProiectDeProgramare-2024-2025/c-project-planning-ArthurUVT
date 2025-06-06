#include "AdvancedTracker.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <cmath>

AdvancedTracker::AdvancedTracker()
{
    loadActivities();
    loadGoals();
}

// Load activities from CSV
void AdvancedTracker::loadActivities()
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

// Load goals from CSV
void AdvancedTracker::loadGoals()
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
void AdvancedTracker::saveGoals()
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

// View general statistics
void AdvancedTracker::viewStatistics()
{
    std::cout << "=== General Statistics ===\n";

    if (activities.empty())
    {
        std::cout << "No activities recorded.\n";
        return;
    }

    // Count activities by type
    std::map<ActivityType, int> activityCounts;
    std::map<ActivityType, double> totalDuration;
    std::map<ActivityType, double> totalDistance;
    std::map<ActivityType, int> totalReps;

    for (const auto &activity : activities)
    {
        activityCounts[activity.type]++;
        totalDuration[activity.type] += activity.duration;
        totalDistance[activity.type] += activity.distance;
        totalReps[activity.type] += activity.repetitions;
    }

    std::cout << "\n--- Activity Summary ---\n";
    std::cout << std::left << std::setw(12) << "Type"
              << std::setw(8) << "Count"
              << std::setw(12) << "Total Dur."
              << std::setw(12) << "Total Dist."
              << std::setw(10) << "Total Reps" << "\n";
    std::cout << std::string(55, '-') << "\n";

    for (const auto &pair : activityCounts)
    {
        std::cout << std::left << std::setw(12) << activityTypeToString(pair.first)
                  << std::setw(8) << pair.second
                  << std::setw(12) << std::fixed << std::setprecision(1) << totalDuration[pair.first]
                  << std::setw(12) << std::fixed << std::setprecision(1) << totalDistance[pair.first]
                  << std::setw(10) << totalReps[pair.first] << "\n";
    }

    // Overall totals
    double grandTotalDuration = 0;
    double grandTotalDistance = 0;
    int grandTotalReps = 0;

    for (const auto &activity : activities)
    {
        grandTotalDuration += activity.duration;
        grandTotalDistance += activity.distance;
        grandTotalReps += activity.repetitions;
    }

    std::cout << "\n--- Overall Totals ---\n";
    std::cout << "Total Activities: " << activities.size() << "\n";
    std::cout << "Total Duration: " << std::fixed << std::setprecision(1) << grandTotalDuration << " minutes\n";
    std::cout << "Total Distance: " << std::fixed << std::setprecision(1) << grandTotalDistance << " km\n";
    std::cout << "Total Repetitions: " << grandTotalReps << "\n";

    if (!activities.empty())
    {
        std::cout << "Average Duration: " << std::fixed << std::setprecision(1)
                  << grandTotalDuration / activities.size() << " minutes\n";
    }
}

// Filter statistics by activity and goal
void AdvancedTracker::filterStatistics(int activityId, int goalId)
{
    std::cout << "=== Filtered Statistics ===\n";
    std::cout << "Activity ID: " << activityId << ", Goal ID: " << goalId << "\n\n";

    // Filter activities by type (activityId maps to ActivityType)
    ActivityType targetType = static_cast<ActivityType>(activityId);
    std::vector<Activity> filteredActivities;

    for (const auto &activity : activities)
    {
        if (activity.type == targetType)
        {
            filteredActivities.push_back(activity);
        }
    }

    if (filteredActivities.empty())
    {
        std::cout << "No activities found for the specified type.\n";
        return;
    }

    // Calculate statistics for filtered activities
    double totalDuration = 0;
    double totalDistance = 0;
    int totalReps = 0;

    for (const auto &activity : filteredActivities)
    {
        totalDuration += activity.duration;
        totalDistance += activity.distance;
        totalReps += activity.repetitions;
    }

    std::cout << "--- " << activityTypeToString(targetType) << " Statistics ---\n";
    std::cout << "Count: " << filteredActivities.size() << "\n";
    std::cout << "Total Duration: " << std::fixed << std::setprecision(1) << totalDuration << " minutes\n";
    std::cout << "Total Distance: " << std::fixed << std::setprecision(1) << totalDistance << " km\n";
    std::cout << "Total Repetitions: " << totalReps << "\n";
    std::cout << "Average Duration: " << std::fixed << std::setprecision(1)
              << totalDuration / filteredActivities.size() << " minutes\n";

    // Show goal comparison if goal exists
    if (goalId >= 0 && goalId < static_cast<int>(goals.size()))
    {
        const auto &goal = goals[goalId];
        std::cout << "\n--- Goal Comparison ---\n";
        std::cout << "Goal: " << goal.description << "\n";
        std::cout << "Deadline: " << goal.deadline << "\n";

        if (goal.targetDuration > 0)
        {
            double progress = (totalDuration / goal.targetDuration) * 100;
            std::cout << "Duration Progress: " << std::fixed << std::setprecision(1)
                      << progress << "% (" << totalDuration << "/" << goal.targetDuration << " mins)\n";
        }

        if (goal.targetDistance > 0)
        {
            double progress = (totalDistance / goal.targetDistance) * 100;
            std::cout << "Distance Progress: " << std::fixed << std::setprecision(1)
                      << progress << "% (" << totalDistance << "/" << goal.targetDistance << " km)\n";
        }

        if (goal.targetReps > 0)
        {
            double progress = (static_cast<double>(totalReps) / goal.targetReps) * 100;
            std::cout << "Repetitions Progress: " << std::fixed << std::setprecision(1)
                      << progress << "% (" << totalReps << "/" << goal.targetReps << ")\n";
        }
    }
}

// View progress for a specific goal with ASCII chart
void AdvancedTracker::viewProgress(int goalId)
{
    if (goalId < 0 || goalId >= static_cast<int>(goals.size()))
    {
        std::cout << "Goal ID " << goalId << " not found!\n";
        return;
    }

    const auto &goal = goals[goalId];
    std::cout << "=== Progress for Goal " << goalId << " ===\n";
    std::cout << "Goal: " << goal.description << "\n";
    std::cout << "Activity Type: " << activityTypeToString(goal.type) << "\n";
    std::cout << "Deadline: " << goal.deadline << "\n\n";

    // Filter activities by goal type
    std::vector<Activity> relevantActivities;
    for (const auto &activity : activities)
    {
        if (activity.type == goal.type)
        {
            relevantActivities.push_back(activity);
        }
    }

    if (relevantActivities.empty())
    {
        std::cout << "No activities found for this goal type.\n";
        return;
    }

    // Calculate current progress
    double totalDuration = 0;
    double totalDistance = 0;
    int totalReps = 0;

    for (const auto &activity : relevantActivities)
    {
        totalDuration += activity.duration;
        totalDistance += activity.distance;
        totalReps += activity.repetitions;
    }

    // Show progress bars
    std::cout << "--- Progress Visualization ---\n";

    if (goal.targetDuration > 0)
    {
        double progress = std::min(100.0, (totalDuration / goal.targetDuration) * 100);
        std::cout << "Duration Progress: " << std::fixed << std::setprecision(1) << progress << "%\n";
        displayProgressBar("Duration", progress);
        std::cout << "Current: " << totalDuration << " / Target: " << goal.targetDuration << " minutes\n\n";
    }

    if (goal.targetDistance > 0)
    {
        double progress = std::min(100.0, (totalDistance / goal.targetDistance) * 100);
        std::cout << "Distance Progress: " << std::fixed << std::setprecision(1) << progress << "%\n";
        displayProgressBar("Distance", progress);
        std::cout << "Current: " << totalDistance << " / Target: " << goal.targetDistance << " km\n\n";
    }

    if (goal.targetReps > 0)
    {
        double progress = std::min(100.0, (static_cast<double>(totalReps) / goal.targetReps) * 100);
        std::cout << "Repetitions Progress: " << std::fixed << std::setprecision(1) << progress << "%\n";
        displayProgressBar("Reps", progress);
        std::cout << "Current: " << totalReps << " / Target: " << goal.targetReps << "\n\n";
    }

    // Achievement status
    bool achieved = (goal.targetDuration == 0 || totalDuration >= goal.targetDuration) &&
                    (goal.targetDistance == 0 || totalDistance >= goal.targetDistance) &&
                    (goal.targetReps == 0 || totalReps >= goal.targetReps);

    std::cout << "Status: " << (achieved ? "ACHIEVED!" : "In Progress") << "\n";
}

// Display ASCII progress bar
void AdvancedTracker::displayProgressBar(const std::string &label, double percentage)
{
    const int barWidth = 50;
    int filledWidth = static_cast<int>((percentage / 100.0) * barWidth);

    std::cout << label << ": [";
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < filledWidth)
        {
            std::cout << "█";
        }
        else
        {
            std::cout << "░";
        }
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << percentage << "%\n";
}

// Delete goal by ID
void AdvancedTracker::deleteGoal(int goalId)
{
    if (goalId < 0 || goalId >= static_cast<int>(goals.size()))
    {
        std::cout << "Goal ID " << goalId << " not found!\n";
        return;
    }

    std::string description = goals[goalId].description;
    goals.erase(goals.begin() + goalId);
    saveGoals();

    std::cout << "Goal '" << description << "' deleted successfully!\n";
    std::cout << "Note: Goal IDs may have shifted. Please check updated goal list.\n";
}

// Backup data to specified file
void AdvancedTracker::backup(const std::string &filePath)
{
    std::cout << "=== Backing up data to: " << filePath << " ===\n";

    std::ofstream backupFile(filePath);
    if (!backupFile.is_open())
    {
        std::cout << "Error: Could not create backup file!\n";
        return;
    }

    // Write backup header
    backupFile << "=== Sports Tracker Backup ===\n";
    backupFile << "Generated on: " << getCurrentDate() << "\n\n";

    // Backup activities
    backupFile << "--- ACTIVITIES ---\n";
    backupFile << "ActivityType,Date,Duration,Distance,Repetitions\n";
    for (const auto &activity : activities)
    {
        backupFile << activityTypeToString(activity.type) << ","
                   << activity.date << ","
                   << activity.duration << ","
                   << activity.distance << ","
                   << activity.repetitions << "\n";
    }

    // Backup goals
    backupFile << "\n--- GOALS ---\n";
    backupFile << "ActivityType,Description,Deadline,TargetDistance,TargetDuration,TargetReps,Achieved\n";
    for (const auto &goal : goals)
    {
        backupFile << activityTypeToString(goal.type) << ","
                   << goal.description << ","
                   << goal.deadline << ","
                   << goal.targetDistance << ","
                   << goal.targetDuration << ","
                   << goal.targetReps << ","
                   << (goal.achieved ? "1" : "0") << "\n";
    }

    backupFile.close();

    std::cout << "Backup completed successfully!\n";
    std::cout << "Activities backed up: " << activities.size() << "\n";
    std::cout << "Goals backed up: " << goals.size() << "\n";
}

// Simple date function (in real app, would use proper date library)
std::string AdvancedTracker::getCurrentDate()
{
    return "2024-01-01"; // Placeholder - in real app would get actual date
}