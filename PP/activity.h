#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <string>
#include <vector>

// C++11 enum class for better type safety
enum class ActivityType
{
    RUNNING = 0,
    WALKING = 1,
    SWIMMING = 2,
    CARDIO = 3,
    STRENGTH = 4,
    UNKNOWN // For error handling or default
};

// Forward declarations of utility functions
std::string activityTypeToString(ActivityType type);
ActivityType stringToActivityType(const std::string &typeStr);

// Struct to hold activity data using std::string
struct Activity
{
    ActivityType type = ActivityType::UNKNOWN;
    std::string date = ""; // Format: YYYY-MM-DD
    double duration = 0.0; // in minutes (using double for potentially better precision)
    double distance = 0.0; // in kilometers
    int repetitions = 0;   // for strength activities

    // Constructor declarations
    Activity(ActivityType t, const std::string &d, double dur, double dist = 0.0, int reps = 0);
    Activity(); // Default constructor
};

// Structure for tracking goals and achievements
struct Goal
{
    ActivityType type = ActivityType::UNKNOWN;
    double targetDistance = 0.0;  // in kilometers
    double targetDuration = 0.0;  // in minutes
    int targetReps = 0;           // for strength activities
    std::string deadline = "";    // Format: YYYY-MM-DD
    std::string description = ""; // User-friendly description
    bool achieved = false;

    // Constructor declaration
    Goal(ActivityType t, const std::string &desc, const std::string &dl,
         double dist = 0.0, double dur = 0.0, int reps = 0);
    Goal(); // Default constructor
};

#endif // ACTIVITY_H
