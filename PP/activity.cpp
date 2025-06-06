#include "activity.h"
#include <algorithm>

// Convert ActivityType to string
std::string activityTypeToString(ActivityType type)
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

// Convert string to ActivityType
ActivityType stringToActivityType(const std::string &str)
{
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower == "running")
        return ActivityType::RUNNING;
    if (lower == "walking")
        return ActivityType::WALKING;
    if (lower == "swimming")
        return ActivityType::SWIMMING;
    if (lower == "cardio")
        return ActivityType::CARDIO;
    if (lower == "strength")
        return ActivityType::STRENGTH;
    return ActivityType::UNKNOWN;
}

// Activity constructor implementation
Activity::Activity(ActivityType t, const std::string &d, double dur, double dist, int reps)
    : type(t), date(d), duration(dur), distance(dist), repetitions(reps)
{
}

// Activity default constructor
Activity::Activity() = default;

// Goal constructor implementation
Goal::Goal(ActivityType t, const std::string &desc, const std::string &dl,
           double dist, double dur, int reps)
    : type(t), targetDistance(dist), targetDuration(dur),
      targetReps(reps), deadline(dl), description(desc), achieved(false)
{
}

// Goal default constructor
Goal::Goal() = default;