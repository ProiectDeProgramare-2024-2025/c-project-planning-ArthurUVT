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

// Struct to hold activity data using std::string
struct Activity
{
    ActivityType type = ActivityType::UNKNOWN;
    std::string date = ""; // Format: YYYY-MM-DD
    double duration = 0.0; // in minutes (using double for potentially better precision)
    double distance = 0.0; // in kilometers
    int repetitions = 0;   // for strength activities

    // Optional: Constructor for easier creation
    Activity(ActivityType t, std::string d, double dur, double dist = 0.0, int reps = 0)
        : type(t), date(std::move(d)), duration(dur), distance(dist), repetitions(reps) {}

    // Default constructor
    Activity() = default;
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

    // Constructor
    Goal(ActivityType t, std::string desc, std::string dl,
         double dist = 0.0, double dur = 0.0, int reps = 0)
        : type(t), targetDistance(dist), targetDuration(dur),
          targetReps(reps), deadline(std::move(dl)),
          description(std::move(desc)), achieved(false) {}

    // Default constructor
    Goal() = default;
};

#endif // ACTIVITY_H
