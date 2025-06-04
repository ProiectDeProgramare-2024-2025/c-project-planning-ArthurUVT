#ifndef APP_1_H
#define APP_1_H

#include <string>
#include <vector>

// Activity Types
enum class ActivityType
{
    RUNNING = 0,
    WALKING = 1,
    SWIMMING = 2,
    CARDIO = 3,
    STRENGTH = 4,
    UNKNOWN
};

// Activity structure
struct Activity
{
    ActivityType type = ActivityType::UNKNOWN;
    std::string date = "";
    double duration = 0.0;
    double distance = 0.0;
    int repetitions = 0;

    Activity() = default;
    Activity(ActivityType t, std::string d, double dur, double dist = 0.0, int reps = 0)
        : type(t), date(std::move(d)), duration(dur), distance(dist), repetitions(reps) {}
};

// Goal structure
struct Goal
{
    ActivityType type = ActivityType::UNKNOWN;
    std::string description = "";
    std::string deadline = "";
    int targetReps = 0;
    double targetDuration = 0.0;
    double targetDistance = 0.0;
    bool achieved = false;

    Goal() = default;
    Goal(ActivityType t, std::string desc, std::string dl,
         double dist = 0.0, double dur = 0.0, int reps = 0)
        : type(t), description(std::move(desc)), deadline(std::move(dl)),
          targetReps(reps), targetDuration(dur), targetDistance(dist), achieved(false) {}
};

class App1
{
public:
    App1();
    ~App1();

    // Core functions
    bool addActivity(ActivityType type, const std::string &date, double duration,
                     double distance = 0.0, int repetitions = 0);
    bool viewActivity(int activityId);
    bool viewAllActivities();

    // Goal management
    bool addGoal(ActivityType type, const std::string &description, const std::string &deadline,
                 int targetReps, double targetDuration, double targetDistance);
    bool viewGoal(int goalId);
    bool viewAllGoals();
    bool modifyGoal(int goalId, ActivityType type, const std::string &description,
                    const std::string &deadline, int targetReps, double targetDuration,
                    double targetDistance);

private:
    std::vector<Activity> activities;
    std::vector<Goal> goals;
    const std::string activitiesFilename = "activities_cpp.csv";
    const std::string goalsFilename = "activities_goals_cpp.csv";

    // File operations
    void loadActivities();
    void saveActivities();
    void loadGoals();
    void saveGoals();

    // Helper functions
    std::string getActivityTypeName(ActivityType type);
    bool isDateValid(const std::string &date);
};

#endif // APP_1_H