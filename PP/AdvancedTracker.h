#ifndef ADVANCED_TRACKER_H
#define ADVANCED_TRACKER_H

#include <vector>
#include <string>
#include "activity.h"

class AdvancedTracker
{
private:
    std::vector<Activity> activities;
    std::vector<Goal> goals;
    const std::string ACTIVITIES_FILE = "activities_cpp.csv";
    const std::string GOALS_FILE = "activities_goals_cpp.csv";

    // Helper method for progress bars
    void displayProgressBar(const std::string &label, double percentage);

    // Helper method for date
    std::string getCurrentDate();

public:
    AdvancedTracker();

    // File operations
    void loadActivities();
    void loadGoals();
    void saveGoals();

    // Statistical methods
    void viewStatistics();
    void filterStatistics(int activityId, int goalId);
    void viewProgress(int goalId);

    // Goal management
    void deleteGoal(int goalId);

    // Backup functionality
    void backup(const std::string &filePath);
};

#endif // ADVANCED_TRACKER_H