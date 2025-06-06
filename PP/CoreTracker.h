#ifndef CORE_TRACKER_H
#define CORE_TRACKER_H

#include <vector>
#include <string>
#include "activity.h"
#include "Color.h"

class CoreTracker
{
private:
    std::vector<Activity> activities;
    std::vector<Goal> goals;
    const std::string ACTIVITIES_FILE = "activities_cpp.csv";
    const std::string GOALS_FILE = "activities_goals_cpp.csv";

public:
    CoreTracker();
    ~CoreTracker();

    // Utility methods
    std::string colorActivityType(ActivityType type);

    // File operations
    void loadActivities();
    void saveActivities();
    void loadGoals();
    void saveGoals();

    // Activity operations
    void addActivity(int id);
    void viewActivity(int id);
    void viewActivities();

    // Goal operations
    void addGoal(int goalId, int activityId, const std::string &description,
                 const std::string &deadline, int targetReps, double targetDuration, double targetDistance);
    void viewGoal(int id);
    void viewGoals();
    void modifyGoal(int goalId, int activityId, const std::string &description,
                    const std::string &deadline, int targetReps, double targetDuration, double targetDistance);
};

#endif // CORE_TRACKER_H