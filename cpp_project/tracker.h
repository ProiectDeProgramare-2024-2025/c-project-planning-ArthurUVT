#ifndef TRACKER_H
#define TRACKER_H

#include <vector>
#include <string>
#include <regex>
#include "activity.h"

// ANSI Color Codes (as const strings)
const std::string COLOR_RESET = "\033[0m";
const std::string COLOR_CYAN = "\033[0;36m";
const std::string COLOR_YELLOW = "\033[0;33m";
const std::string COLOR_GREEN = "\033[0;32m";
const std::string COLOR_RED = "\033[0;31m";
const std::string COLOR_BLUE = "\033[0;34m";
const std::string COLOR_MAGENTA = "\033[0;35m";

class Tracker
{
public:
    // Constructor: Loads data from the file
    Tracker(const std::string &filename);

    // Destructor: Saves data to the file
    ~Tracker();

    // Main application loop
    void run();

private:
    std::vector<Activity> activities; // Use std::vector for dynamic storage
    std::vector<Goal> goals;          // Store user goals
    std::string dataFilename;         // Store the filename for saving
    std::string goalsFilename;        // Store the goals filename

    // Menu display functions
    void displayMainMenu();
    void displayAddMenu();
    void displayFilterMenu();
    void displaySearchMenu();
    void displayGoalsMenu();

    // Core activity functions
    void addActivity(ActivityType type);
    void viewActivities();
    void viewStatistics();

    // New search and filter functions
    void searchActivities();
    void searchByKeyword(const std::string &keyword);
    void searchByDate(const std::string &date);
    void filterActivities();
    void filterByType(ActivityType type);
    void filterByDateRange(const std::string &startDate, const std::string &endDate);
    void filterByDuration(double minDuration, double maxDuration);

    // Data visualization functions
    void showProgressChart();
    void showActivityDistribution();

    // Goal management functions
    void manageGoals();
    void addGoal();
    void viewGoals();
    void checkGoalAchievements();

    // Data management functions
    void backupData();
    void restoreFromBackup();

    // File I/O
    void loadFromFile();
    void saveToFile();
    void loadGoalsFromFile();
    void saveGoalsToFile();

    // Utility functions
    void clearScreen();
    void waitForEnter();
    std::string getActivityTypeName(ActivityType type);
    std::string getCurrentDate();
    bool isDateInRange(const std::string &date, const std::string &startDate, const std::string &endDate);
    bool isDateValid(const std::string &date);
    int calculateDaysBetween(const std::string &dateStart, const std::string &dateEnd);
    std::string formatDuration(double minutes);

    // Input validation helpers
    int getIntegerInput(const std::string &prompt, int minVal, int maxVal);
    double getDoubleInput(const std::string &prompt, double minVal, bool allowEqual = false);
    std::string getStringInput(const std::string &prompt, const std::string &defaultValue);
    std::string getDateInput(const std::string &prompt, const std::string &defaultValue);
};

#endif // TRACKER_H
