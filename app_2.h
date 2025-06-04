#ifndef APP_2_H
#define APP_2_H

#include "app_1.h" // We'll share the same data structures
#include <string>
#include <vector>
#include <map>

class App2
{
public:
    App2();
    ~App2();

    // Statistics functions
    bool viewStatistics();
    bool filterStatistics(int activityId, int goalId);

    // Progress and visualization
    bool viewProgress(int goalId);

    // Goal management
    bool deleteGoal(int goalId);

    // Data backup
    bool backupData(const std::string &filePath);

private:
    std::vector<Activity> activities;
    std::vector<Goal> goals;
    const std::string activitiesFilename = "activities_cpp.csv";
    const std::string goalsFilename = "activities_goals_cpp.csv";

    // File operations
    void loadActivities();
    void loadGoals();

    // Helper functions
    std::string getActivityTypeName(ActivityType type);
    bool isDateValid(const std::string &date);
    bool isDateInRange(const std::string &date, const std::string &startDate, const std::string &endDate);
    int calculateDaysBetween(const std::string &dateStart, const std::string &dateEnd);
    std::string formatDuration(double minutes);

    // Visualization helpers
    void generateProgressChart(int goalId);
    void generateActivitySummary(ActivityType type);
};

#endif // APP_2_H