#include "tracker.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip> // For std::setw, std::left, std::fixed, std::setprecision
#include <ctime>
#include <cstdlib>   // For system()
#include <map>       // For statistics calculation
#include <numeric>   // For std::accumulate
#include <algorithm> // For std::sort, std::max_element, etc.
#include <cmath>     // For mathematical operations
#include <regex>     // For regex matching in search
#include <chrono>    // For time-based operations
#include <set>       // For unique collections

// Constructor
Tracker::Tracker(const std::string &filename) : dataFilename(filename)
{
    // Set goals filename based on data filename
    goalsFilename = dataFilename;
    size_t dotPos = goalsFilename.find_last_of('.');
    if (dotPos != std::string::npos)
    {
        goalsFilename = goalsFilename.substr(0, dotPos) + "_goals" + goalsFilename.substr(dotPos);
    }
    else
    {
        goalsFilename += "_goals.csv";
    }

    loadFromFile();
    loadGoalsFromFile();
    checkGoalAchievements(); // Check if any goals have been achieved
}

// Destructor (RAII for file saving)
Tracker::~Tracker()
{
    saveToFile();
    saveGoalsToFile();
}

// --- Main Application Logic ---

void Tracker::run()
{
    int option = -1;
    bool inSubmenu = false;
    int submenuType = 0; // Corresponds to main menu options

    do
    {
        if (!inSubmenu)
        {
            clearScreen();
            displayMainMenu();
            option = getIntegerInput("Enter option: ", 0, 9); // Updated for more menu items

            switch (option)
            {
            case 1: // Add activity
            case 2: // View activities
            case 3: // Search activities
            case 4: // Filter activities
            case 5: // View statistics
            case 6: // Show progress chart
            case 7: // Set new goal
            case 8: // View goals
            case 9: // Backup data
                inSubmenu = true;
                submenuType = option;
                break;
            case 0: // Exit
                std::cout << "Exiting the application. Goodbye!" << std::endl;
                break;
                // No default needed due to validated input
            }
        }
        else
        {
            clearScreen();
            switch (submenuType)
            {
            case 1: // Add Menu
            {
                displayAddMenu();
                int addOption = getIntegerInput("Enter option: ", 0, 5);
                ActivityType typeToAdd = ActivityType::UNKNOWN;
                switch (addOption)
                {
                case 1:
                    typeToAdd = ActivityType::RUNNING;
                    break;
                case 2:
                    typeToAdd = ActivityType::WALKING;
                    break;
                case 3:
                    typeToAdd = ActivityType::SWIMMING;
                    break;
                case 4:
                    typeToAdd = ActivityType::CARDIO;
                    break;
                case 5:
                    typeToAdd = ActivityType::STRENGTH;
                    break;
                case 0:
                    inSubmenu = false;
                    break; // Back to main menu
                }
                if (typeToAdd != ActivityType::UNKNOWN)
                {
                    addActivity(typeToAdd);
                    checkGoalAchievements(); // Check if any goals were achieved
                    inSubmenu = false;
                }
                break;
            }
            case 2: // View Activities
                viewActivities();
                inSubmenu = false; // Return to main menu after viewing
                break;
            case 3: // Search Activities
                searchActivities();
                inSubmenu = false;
                break;
            case 4: // Filter Activities
                filterActivities();
                inSubmenu = false;
                break;
            case 5: // View Stats
                viewStatistics();
                inSubmenu = false;
                break;
            case 6: // Progress Chart
                showProgressChart();
                inSubmenu = false;
                break;
            case 7: // Add Goal
                addGoal();
                inSubmenu = false;
                break;
            case 8: // View Goals
                viewGoals();
                inSubmenu = false;
                break;
            case 9: // Backup Data
                backupData();
                inSubmenu = false;
                break;
            }
        }
    } while (option != 0);
}

// --- Menu Display ---

void Tracker::displayMainMenu()
{
    std::cout << "===================================" << std::endl;
    std::cout << "    " << COLOR_CYAN << "SPORTS ACTIVITY TRACKER (C++)" << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << COLOR_YELLOW << "ACTIVITIES:" << COLOR_RESET << std::endl;
    std::cout << "  1 - Add Activity" << std::endl;
    std::cout << "  2 - View Activities" << std::endl;
    std::cout << "  3 - Search Activities" << std::endl;
    std::cout << "  4 - Filter Activities" << std::endl;
    std::cout << COLOR_YELLOW << "STATISTICS:" << COLOR_RESET << std::endl;
    std::cout << "  5 - View Statistics" << std::endl;
    std::cout << "  6 - Show Progress Chart" << std::endl;
    std::cout << COLOR_YELLOW << "GOALS:" << COLOR_RESET << std::endl;
    std::cout << "  7 - Set New Goal" << std::endl;
    std::cout << "  8 - View Goals & Achievements" << std::endl;
    std::cout << COLOR_YELLOW << "SYSTEM:" << COLOR_RESET << std::endl;
    std::cout << "  9 - Backup Data" << std::endl;
    std::cout << "  0 - Exit" << std::endl;
    std::cout << "===================================" << std::endl;
}

void Tracker::displayAddMenu()
{
    std::cout << "===================================" << std::endl;
    std::cout << "        " << COLOR_YELLOW << "ADD NEW ACTIVITY" << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "1 - " << COLOR_GREEN << "Running" << COLOR_RESET << std::endl;
    std::cout << "2 - " << COLOR_GREEN << "Walking" << COLOR_RESET << std::endl;
    std::cout << "3 - " << COLOR_GREEN << "Swimming" << COLOR_RESET << std::endl;
    std::cout << "4 - " << COLOR_GREEN << "Cardio" << COLOR_RESET << std::endl;
    std::cout << "5 - " << COLOR_GREEN << "Strength" << COLOR_RESET << std::endl;
    std::cout << "0 - Back to Main Menu" << std::endl;
    std::cout << "===================================" << std::endl;
}

void Tracker::displayFilterMenu()
{
    std::cout << "===================================" << std::endl;
    std::cout << "        " << COLOR_YELLOW << "FILTER ACTIVITIES" << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "1 - By Activity Type" << std::endl;
    std::cout << "2 - By Date Range" << std::endl;
    std::cout << "3 - By Duration Range" << std::endl;
    std::cout << "0 - Back to Main Menu" << std::endl;
    std::cout << "===================================" << std::endl;
}

void Tracker::displaySearchMenu()
{
    std::cout << "===================================" << std::endl;
    std::cout << "        " << COLOR_YELLOW << "SEARCH ACTIVITIES" << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "1 - By Keyword" << std::endl;
    std::cout << "2 - By Specific Date" << std::endl;
    std::cout << "0 - Back to Main Menu" << std::endl;
    std::cout << "===================================" << std::endl;
}

void Tracker::displayGoalsMenu()
{
    std::cout << "===================================" << std::endl;
    std::cout << "        " << COLOR_YELLOW << "GOALS MENU" << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "1 - Add New Goal" << std::endl;
    std::cout << "2 - View All Goals" << std::endl;
    std::cout << "3 - View Achieved Goals" << std::endl;
    std::cout << "4 - View Pending Goals" << std::endl;
    std::cout << "0 - Back to Main Menu" << std::endl;
    std::cout << "===================================" << std::endl;
}

// --- Core Logic Implementation ---

void Tracker::addActivity(ActivityType type)
{
    clearScreen();
    std::cout << "===================================" << std::endl;
    std::cout << "          ADD " << getActivityTypeName(type) << std::endl;
    std::cout << "===================================" << std::endl;

    Activity newActivity;
    newActivity.type = type;

    std::string defaultDate = getCurrentDate();
    newActivity.date = getDateInput("Date (YYYY-MM-DD)", defaultDate);
    // TODO: Add more robust date format validation if desired

    newActivity.duration = getDoubleInput("Duration (minutes, > 0): ", 0.0, false);

    switch (type)
    {
    case ActivityType::RUNNING:
    case ActivityType::WALKING:
    case ActivityType::SWIMMING:
        newActivity.distance = getDoubleInput("Distance (kilometers, >= 0): ", 0.0, true);
        break;
    case ActivityType::STRENGTH:
        newActivity.repetitions = getIntegerInput("Number of repetitions (>= 0): ", 0, 100000);
        break;
    case ActivityType::CARDIO:
    case ActivityType::UNKNOWN: // Should not happen via menu
        // No extra input needed
        break;
    }

    activities.push_back(newActivity); // Add to the vector

    std::cout << std::endl
              << COLOR_GREEN << getActivityTypeName(type) << " activity added successfully!" << COLOR_RESET << std::endl;
    waitForEnter();
}

void Tracker::viewActivities()
{
    std::cout << "===================================" << std::endl;
    std::cout << "        " << COLOR_YELLOW << "VIEW ACTIVITIES" << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;

    if (activities.empty())
    {
        std::cout << "No activities recorded yet." << std::endl;
    }
    else
    {
        std::cout << std::left // Align text to the left
                  << std::setw(3) << "ID" << " | "
                  << std::setw(10) << "Type" << " | "
                  << std::setw(10) << "Date" << " | "
                  << std::setw(10) << "Duration" << " | "
                  << std::setw(10) << "Distance" << " | "
                  << std::setw(5) << "Reps" << std::endl;
        std::cout << "---+------------+------------+------------+------------+-------" << std::endl;

        std::cout << std::fixed << std::setprecision(1); // For duration formatting

        for (size_t i = 0; i < activities.size(); ++i)
        {
            const auto &act = activities[i];
            std::cout << std::left
                      << std::setw(3) << (i + 1) << " | "
                      << std::setw(18) << getActivityTypeName(act.type) << " | " // Increased width for colored type name
                      << std::setw(10) << act.date << " | "
                      << std::setw(10) << act.duration << " | ";

            // Print distance only if applicable
            if (act.type == ActivityType::RUNNING || act.type == ActivityType::WALKING || act.type == ActivityType::SWIMMING)
            {
                std::cout << std::setprecision(2) << std::setw(10) << act.distance << std::setprecision(1) << " | ";
            }
            else
            {
                std::cout << std::setw(10) << "N/A" << " | ";
            }

            // Print repetitions only if applicable
            if (act.type == ActivityType::STRENGTH)
            {
                std::cout << std::setw(5) << act.repetitions;
            }
            else
            {
                std::cout << std::setw(5) << "N/A";
            }
            std::cout << std::endl;
        }
        // Reset precision settings if needed elsewhere
        std::cout.unsetf(std::ios_base::floatfield);
    }
    waitForEnter();
}

void Tracker::viewStatistics()
{
    std::cout << "===================================" << std::endl;
    std::cout << "        " << COLOR_YELLOW << "VIEW STATISTICS" << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;

    if (activities.empty())
    {
        std::cout << "No activities recorded yet." << std::endl;
        waitForEnter();
        return;
    }

    // Use a map to aggregate statistics by type
    std::map<ActivityType, int> counts;
    std::map<ActivityType, double> totalDurations;
    std::map<ActivityType, double> totalDistances;

    for (const auto &act : activities)
    {
        counts[act.type]++;
        totalDurations[act.type] += act.duration;
        if (act.type == ActivityType::RUNNING || act.type == ActivityType::WALKING || act.type == ActivityType::SWIMMING)
        {
            totalDistances[act.type] += act.distance;
        }
    }

    std::cout << std::left
              << std::setw(18) << "Activity Type" << " | "
              << std::setw(7) << "Count" << " | "
              << std::setw(14) << "Avg Duration" << " | "
              << std::setw(12) << "Avg Distance" << std::endl;
    std::cout << "-------------------+---------+----------------+---------------" << std::endl;

    std::cout << std::fixed << std::setprecision(1); // Default precision for duration

    // Iterate through the map (or a predefined list of types to maintain order)
    std::vector<ActivityType> typesInOrder = {ActivityType::RUNNING, ActivityType::WALKING, ActivityType::SWIMMING, ActivityType::CARDIO, ActivityType::STRENGTH};

    for (ActivityType type : typesInOrder)
    {
        if (counts.count(type))
        { // Check if this type exists in our recorded activities
            int count = counts[type];
            double avgDuration = totalDurations[type] / count;

            std::cout << std::left
                      << std::setw(18) << getActivityTypeName(type) << " | "
                      << std::setw(7) << count << " | "
                      << std::setw(14) << avgDuration << " | ";

            if (type == ActivityType::RUNNING || type == ActivityType::WALKING || type == ActivityType::SWIMMING)
            {
                double avgDistance = totalDistances[type] / count;
                std::cout << std::setprecision(2) << std::setw(12) << avgDistance << std::setprecision(1);
            }
            else
            {
                std::cout << std::setw(12) << "N/A";
            }
            std::cout << std::endl;
        }
    }
    std::cout.unsetf(std::ios_base::floatfield);

    waitForEnter();
}

// --- File I/O ---

void Tracker::loadFromFile()
{
    std::ifstream inFile(dataFilename);
    if (!inFile.is_open())
    {
        // File not existing is not an error on first run
        // std::cerr << "Warning: Could not open file " << dataFilename << " for reading." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inFile, line))
    {
        std::stringstream ss(line);
        std::string segment;
        Activity loadedActivity;
        int typeIndex = -1;

        try
        {
            // 1. Type
            if (!std::getline(ss, segment, ',') || !(std::stringstream(segment) >> typeIndex))
            {
                throw std::runtime_error("Failed to parse type");
            }
            if (typeIndex < 0 || typeIndex > static_cast<int>(ActivityType::STRENGTH))
            {
                typeIndex = static_cast<int>(ActivityType::UNKNOWN);
            }
            loadedActivity.type = static_cast<ActivityType>(typeIndex);

            // 2. Date
            if (!std::getline(ss, loadedActivity.date, ','))
            {
                throw std::runtime_error("Failed to parse date");
            }

            // 3. Duration
            if (!std::getline(ss, segment, ',') || !(std::stringstream(segment) >> loadedActivity.duration))
            {
                throw std::runtime_error("Failed to parse duration");
            }

            // 4. Distance
            if (!std::getline(ss, segment, ',') || !(std::stringstream(segment) >> loadedActivity.distance))
            {
                throw std::runtime_error("Failed to parse distance");
            }

            // 5. Repetitions
            if (!std::getline(ss, segment, ',') || !(std::stringstream(segment) >> loadedActivity.repetitions))
            {
                // If repetitions segment is missing (e.g. older format), default to 0
                // This check assumes repetitions is the last element. If not, parsing fails here.
                std::stringstream test_ss(segment);
                if (!(test_ss >> loadedActivity.repetitions))
                {
                    // Check if it was just missing entirely
                    if (ss.eof())
                    { // Check if we reached end-of-stream trying to get segment
                        loadedActivity.repetitions = 0;
                    }
                    else
                    {
                        throw std::runtime_error("Failed to parse repetitions");
                    }
                }
            }

            activities.push_back(loadedActivity);
        }
        catch (const std::exception &e)
        {
            std::cerr << COLOR_RED << "Error reading line: " << line << " -> " << e.what() << COLOR_RESET << std::endl;
            // Optionally skip this line and continue or stop loading
        }
    }

    // No need to explicitly close inFile, RAII handles it when it goes out of scope
    std::cout << "Loaded " << activities.size() << " activities from \'" << dataFilename << "\'." << std::endl;
    waitForEnter();
}

void Tracker::saveToFile()
{
    std::ofstream outFile(dataFilename); // Opens in truncation mode (overwrites)
    if (!outFile.is_open())
    {
        std::cerr << COLOR_RED << "Error: Could not open file " << dataFilename << " for writing." << COLOR_RESET << std::endl;
        return;
    }

    // Set precision for floating point numbers in the file
    outFile << std::fixed << std::setprecision(2);

    for (const auto &act : activities)
    {
        outFile << static_cast<int>(act.type) << ","
                << act.date << ","
                << std::setprecision(1) << act.duration << std::setprecision(2) << "," // Duration with 1 decimal
                << act.distance << ","
                << act.repetitions << std::endl;
    }

    // No need to explicitly close outFile, RAII handles it
    std::cout << "Saved " << activities.size() << " activities to \'" << dataFilename << "\'." << std::endl;
}

void Tracker::loadGoalsFromFile()
{
    std::ifstream inFile(goalsFilename);
    if (!inFile.is_open())
    {
        // File not existing is not an error on first run
        // std::cerr << "Warning: Could not open file " << goalsFilename << " for reading." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inFile, line))
    {
        std::stringstream ss(line);
        std::string segment;
        Goal loadedGoal;
        int typeIndex = -1;

        try
        {
            // 1. Type
            if (!std::getline(ss, segment, ',') || !(std::stringstream(segment) >> typeIndex))
            {
                throw std::runtime_error("Failed to parse type");
            }
            if (typeIndex < 0 || typeIndex > static_cast<int>(ActivityType::STRENGTH))
            {
                typeIndex = static_cast<int>(ActivityType::UNKNOWN);
            }
            loadedGoal.type = static_cast<ActivityType>(typeIndex);

            // 2. Description
            if (!std::getline(ss, loadedGoal.description, ','))
            {
                throw std::runtime_error("Failed to parse description");
            }

            // 3. Deadline
            if (!std::getline(ss, loadedGoal.deadline, ','))
            {
                throw std::runtime_error("Failed to parse deadline");
            }

            // 4. Target Duration
            if (!std::getline(ss, segment, ',') || !(std::stringstream(segment) >> loadedGoal.targetDuration))
            {
                throw std::runtime_error("Failed to parse target duration");
            }

            // 5. Target Distance
            if (!std::getline(ss, segment, ',') || !(std::stringstream(segment) >> loadedGoal.targetDistance))
            {
                throw std::runtime_error("Failed to parse target distance");
            }

            // 6. Target Repetitions
            if (!std::getline(ss, segment, ',') || !(std::stringstream(segment) >> loadedGoal.targetReps))
            {
                // If repetitions segment is missing (e.g. older format), default to 0
                // This check assumes repetitions is the last element. If not, parsing fails here.
                std::stringstream test_ss(segment);
                if (!(test_ss >> loadedGoal.targetReps))
                {
                    // Check if it was just missing entirely
                    if (ss.eof())
                    { // Check if we reached end-of-stream trying to get segment
                        loadedGoal.targetReps = 0;
                    }
                    else
                    {
                        throw std::runtime_error("Failed to parse target repetitions");
                    }
                }
            }

            goals.push_back(loadedGoal);
        }
        catch (const std::exception &e)
        {
            std::cerr << COLOR_RED << "Error reading line: " << line << " -> " << e.what() << COLOR_RESET << std::endl;
            // Optionally skip this line and continue or stop loading
        }
    }

    // No need to explicitly close inFile, RAII handles it when it goes out of scope
    std::cout << "Loaded " << goals.size() << " goals from \'" << goalsFilename << "\'." << std::endl;
    waitForEnter();
}

void Tracker::saveGoalsToFile()
{
    std::ofstream outFile(goalsFilename); // Opens in truncation mode (overwrites)
    if (!outFile.is_open())
    {
        std::cerr << COLOR_RED << "Error: Could not open file " << goalsFilename << " for writing." << COLOR_RESET << std::endl;
        return;
    }

    // Set precision for floating point numbers in the file
    outFile << std::fixed << std::setprecision(2);

    for (const auto &goal : goals)
    {
        outFile << static_cast<int>(goal.type) << ","
                << goal.description << ","
                << goal.deadline << ","
                << std::setprecision(1) << goal.targetDuration << std::setprecision(2) << "," // Duration with 1 decimal
                << goal.targetDistance << ","
                << goal.targetReps << std::endl;
    }

    // No need to explicitly close outFile, RAII handles it
    std::cout << "Saved " << goals.size() << " goals to \'" << goalsFilename << "\'." << std::endl;
}

// --- Utility Functions ---

void Tracker::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    // POSIX-like systems
    system("clear");
#endif
}

void Tracker::waitForEnter()
{
    std::cout << std::endl
              << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear previous potential newline
    std::cin.get();                                                     // Wait for Enter key
}

std::string Tracker::getActivityTypeName(ActivityType type)
{
    switch (type)
    {
    case ActivityType::RUNNING:
        return COLOR_GREEN + "Running" + COLOR_RESET;
    case ActivityType::WALKING:
        return COLOR_GREEN + "Walking" + COLOR_RESET;
    case ActivityType::SWIMMING:
        return COLOR_GREEN + "Swimming" + COLOR_RESET;
    case ActivityType::CARDIO:
        return COLOR_GREEN + "Cardio" + COLOR_RESET;
    case ActivityType::STRENGTH:
        return COLOR_GREEN + "Strength" + COLOR_RESET;
    default:
        return COLOR_RED + "Unknown" + COLOR_RESET;
    }
}

std::string Tracker::getCurrentDate()
{
    time_t t = time(nullptr);
    struct tm *now = localtime(&t);
    std::stringstream ss;
    ss << (now->tm_year + 1900) << '-'
       << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << '-'
       << std::setw(2) << std::setfill('0') << now->tm_mday;
    return ss.str();
}

// --- Input Validation Helpers ---

int Tracker::getIntegerInput(const std::string &prompt, int minVal, int maxVal)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail() || value < minVal || value > maxVal)
        {
            std::cerr << COLOR_RED << "Invalid input. Please enter an integer between "
                      << minVal << " and " << maxVal << "." << COLOR_RESET << std::endl;
            std::cin.clear(); // Clear error flags
            // Ignore the rest of the invalid input line
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            // Consume the leftover newline character before returning
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

double Tracker::getDoubleInput(const std::string &prompt, double minVal, bool allowEqual)
{
    double value;
    while (true)
    {
        std::cout << prompt;
        std::cin >> value;

        bool conditionMet = allowEqual ? (value >= minVal) : (value > minVal);

        if (std::cin.fail() || !conditionMet)
        {
            std::cerr << COLOR_RED << "Invalid input. Please enter a number "
                      << (allowEqual ? ">= " : "> ") << minVal << "." << COLOR_RESET << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

std::string Tracker::getStringInput(const std::string &prompt, const std::string &defaultValue)
{
    std::string value;
    std::cout << prompt << " [" << defaultValue << "]: ";
    std::getline(std::cin, value);

    if (value.empty())
    {
        return defaultValue;
    }
    // Basic validation could be added here (e.g., check format)
    return value;
}

void Tracker::checkGoalAchievements()
{
    if (goals.empty() || activities.empty())
    {
        return;
    }

    bool anyNewAchievements = false;

    // For each goal, check if it's been achieved
    for (auto &goal : goals)
    {
        // Skip already achieved goals
        if (goal.achieved)
        {
            continue;
        }

        // Calculate progress based on activity type
        double totalDuration = 0.0;
        double totalDistance = 0.0;
        int totalReps = 0;

        for (const auto &act : activities)
        {
            // Only count activities of the same type
            if (act.type == goal.type)
            {
                // Only count activities before the deadline
                if (act.date <= goal.deadline)
                {
                    totalDuration += act.duration;

                    if (goal.type == ActivityType::RUNNING || goal.type == ActivityType::WALKING || goal.type == ActivityType::SWIMMING)
                    {
                        totalDistance += act.distance;
                    }

                    if (goal.type == ActivityType::STRENGTH)
                    {
                        totalReps += act.repetitions;
                    }
                }
            }
        }

        // Check if goal is achieved
        bool achieved = true;

        if (goal.targetDuration > 0 && totalDuration < goal.targetDuration)
        {
            achieved = false;
        }

        if (goal.targetDistance > 0 && totalDistance < goal.targetDistance)
        {
            achieved = false;
        }

        if (goal.targetReps > 0 && totalReps < goal.targetReps)
        {
            achieved = false;
        }

        // If goal is achieved, mark it
        if (achieved && !goal.achieved)
        {
            goal.achieved = true;
            anyNewAchievements = true;
        }
    }

    // If any goals were achieved, save goals file
    if (anyNewAchievements)
    {
        saveGoalsToFile();

        clearScreen();
        std::cout << "===================================" << std::endl;
        std::cout << "     " << COLOR_GREEN << "GOAL ACHIEVED!" << COLOR_RESET << std::endl;
        std::cout << "===================================" << std::endl;
        std::cout << "Congratulations! You have achieved one or more of your goals!" << std::endl;
        std::cout << "Check your goals page for details." << std::endl;
        waitForEnter();
    }
}

void Tracker::addGoal()
{
    clearScreen();
    std::cout << "===================================" << std::endl;
    std::cout << "        " << COLOR_YELLOW << "SET NEW GOAL" << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;

    // Get goal description
    std::string description = getStringInput("Goal Description", "Complete a 5K run");

    // Select activity type
    displayAddMenu();
    int typeOption = getIntegerInput("Select activity type: ", 1, 5); // Must choose a type, no back option

    ActivityType type;
    switch (typeOption)
    {
    case 1:
        type = ActivityType::RUNNING;
        break;
    case 2:
        type = ActivityType::WALKING;
        break;
    case 3:
        type = ActivityType::SWIMMING;
        break;
    case 4:
        type = ActivityType::CARDIO;
        break;
    case 5:
        type = ActivityType::STRENGTH;
        break;
    default:
        type = ActivityType::RUNNING; // Shouldn't happen due to validation
    }

    // Get deadline
    std::string currentDate = getCurrentDate();
    std::string deadline = getDateInput("Deadline (YYYY-MM-DD)", currentDate);

    // Get target values based on activity type
    double targetDistance = 0.0;
    double targetDuration = 0.0;
    int targetReps = 0;

    if (type == ActivityType::RUNNING || type == ActivityType::WALKING || type == ActivityType::SWIMMING)
    {
        targetDistance = getDoubleInput("Target distance (kilometers): ", 0.0, true);
    }

    targetDuration = getDoubleInput("Target duration (minutes, 0 for no target): ", 0.0, true);

    if (type == ActivityType::STRENGTH)
    {
        targetReps = getIntegerInput("Target repetitions: ", 0, 1000);
    }

    // Create and add the goal
    Goal newGoal(type, description, deadline, targetDistance, targetDuration, targetReps);
    goals.push_back(newGoal);

    // Save goals to file
    saveGoalsToFile();

    std::cout << std::endl
              << COLOR_GREEN << "New goal added successfully!" << COLOR_RESET << std::endl;
    waitForEnter();
}

void Tracker::viewGoals()
{
    clearScreen();
    std::cout << "===================================" << std::endl;
    std::cout << "      " << COLOR_YELLOW << "YOUR GOALS" << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;

    if (goals.empty())
    {
        std::cout << "No goals set yet." << std::endl;
        waitForEnter();
        return;
    }

    // Display goals
    std::cout << std::left
              << std::setw(3) << "ID" << " | "
              << std::setw(10) << "Type" << " | "
              << std::setw(30) << "Description" << " | "
              << std::setw(10) << "Deadline" << " | "
              << std::setw(10) << "Status" << std::endl;
    std::cout << "---+------------+--------------------------------+------------+------------" << std::endl;

    for (size_t i = 0; i < goals.size(); ++i)
    {
        const auto &goal = goals[i];
        std::cout << std::left
                  << std::setw(3) << (i + 1) << " | "
                  << std::setw(18) << getActivityTypeName(goal.type) << " | "
                  << std::setw(30) << goal.description << " | "
                  << std::setw(10) << goal.deadline << " | "
                  << (goal.achieved ? COLOR_GREEN + std::string("ACHIEVED") + COLOR_RESET : COLOR_YELLOW + std::string("PENDING") + COLOR_RESET) << std::endl;
    }

    // Show goal details
    int goalId = getIntegerInput("Enter goal ID to view details (0 to go back): ", 0, goals.size());

    if (goalId > 0)
    {
        const Goal &goal = goals[goalId - 1];

        clearScreen();
        std::cout << "===================================" << std::endl;
        std::cout << "       " << COLOR_YELLOW << "GOAL DETAILS" << COLOR_RESET << std::endl;
        std::cout << "===================================" << std::endl;
        std::cout << COLOR_CYAN << "Description: " << COLOR_RESET << goal.description << std::endl;
        std::cout << COLOR_CYAN << "Activity Type: " << COLOR_RESET << getActivityTypeName(goal.type) << std::endl;
        std::cout << COLOR_CYAN << "Deadline: " << COLOR_RESET << goal.deadline << std::endl;
        std::cout << COLOR_CYAN << "Status: " << COLOR_RESET
                  << (goal.achieved ? COLOR_GREEN + std::string("ACHIEVED") + COLOR_RESET : COLOR_YELLOW + std::string("PENDING") + COLOR_RESET) << std::endl;

        // Display targets
        std::cout << std::endl
                  << COLOR_CYAN << "Target Values:" << COLOR_RESET << std::endl;

        if (goal.targetDuration > 0)
        {
            std::cout << "Duration: " << std::fixed << std::setprecision(1) << goal.targetDuration << " minutes" << std::endl;
        }

        if (goal.type == ActivityType::RUNNING || goal.type == ActivityType::WALKING || goal.type == ActivityType::SWIMMING)
        {
            if (goal.targetDistance > 0)
            {
                std::cout << "Distance: " << std::fixed << std::setprecision(2) << goal.targetDistance << " kilometers" << std::endl;
            }
        }

        if (goal.type == ActivityType::STRENGTH && goal.targetReps > 0)
        {
            std::cout << "Repetitions: " << goal.targetReps << std::endl;
        }

        // Show progress toward goal
        std::cout << std::endl
                  << COLOR_CYAN << "Current Progress:" << COLOR_RESET << std::endl;

        // Calculate progress based on activity type
        double totalDuration = 0.0;
        double totalDistance = 0.0;
        int totalReps = 0;
        int matchingActivities = 0;

        for (const auto &act : activities)
        {
            // Only count activities of the same type
            if (act.type == goal.type)
            {
                // Only count activities before the deadline
                if (act.date <= goal.deadline)
                {
                    matchingActivities++;
                    totalDuration += act.duration;

                    if (goal.type == ActivityType::RUNNING || goal.type == ActivityType::WALKING || goal.type == ActivityType::SWIMMING)
                    {
                        totalDistance += act.distance;
                    }

                    if (goal.type == ActivityType::STRENGTH)
                    {
                        totalReps += act.repetitions;
                    }
                }
            }
        }

        std::cout << "Matching Activities: " << matchingActivities << std::endl;
        std::cout << "Total Duration: " << std::fixed << std::setprecision(1) << totalDuration << " minutes" << std::endl;

        if (goal.targetDuration > 0)
        {
            double durationPercentage = (totalDuration / goal.targetDuration) * 100;
            std::cout << "Duration Progress: " << std::min(100.0, durationPercentage) << "%" << std::endl;
        }

        if (goal.type == ActivityType::RUNNING || goal.type == ActivityType::WALKING || goal.type == ActivityType::SWIMMING)
        {
            std::cout << "Total Distance: " << std::fixed << std::setprecision(2) << totalDistance << " kilometers" << std::endl;

            if (goal.targetDistance > 0)
            {
                double distancePercentage = (totalDistance / goal.targetDistance) * 100;
                std::cout << "Distance Progress: " << std::min(100.0, distancePercentage) << "%" << std::endl;
            }
        }

        if (goal.type == ActivityType::STRENGTH)
        {
            std::cout << "Total Repetitions: " << totalReps << std::endl;

            if (goal.targetReps > 0)
            {
                double repsPercentage = (static_cast<double>(totalReps) / goal.targetReps) * 100;
                std::cout << "Repetitions Progress: " << std::min(100.0, repsPercentage) << "%" << std::endl;
            }
        }
    }

    waitForEnter();
}

void Tracker::backupData()
{
    // Implementation of backupData method
}

// Enhanced date input with validation
std::string Tracker::getDateInput(const std::string &prompt, const std::string &defaultValue)
{
    while (true)
    {
        std::string date = getStringInput(prompt, defaultValue);

        if (isDateValid(date))
        {
            return date;
        }

        std::cerr << COLOR_RED << "Invalid date format or values. Please use YYYY-MM-DD format with valid values." << COLOR_RESET << std::endl;
    }
}

// Validate date format and values
bool Tracker::isDateValid(const std::string &date)
{
    // Check basic format with regex
    std::regex dateRegex("^\\d{4}-\\d{2}-\\d{2}$");
    if (!std::regex_match(date, dateRegex))
    {
        return false;
    }

    // Parse and validate components
    int year, month, day;
    if (sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day) != 3)
    {
        return false;
    }

    // Basic range checks
    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1)
    {
        return false;
    }

    // Check days in month (including leap years)
    int maxDay = 31; // Default for months with 31 days

    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        maxDay = 30;
    }
    else if (month == 2)
    {
        // February - check for leap year
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        maxDay = isLeapYear ? 29 : 28;
    }

    return day <= maxDay;
}

// Check if a date is within a specified range
bool Tracker::isDateInRange(const std::string &date, const std::string &startDate, const std::string &endDate)
{
    // Simple string comparison works for YYYY-MM-DD format
    return date >= startDate && date <= endDate;
}

// Calculate days between two dates
int Tracker::calculateDaysBetween(const std::string &dateStart, const std::string &dateEnd)
{
    struct std::tm startTm = {}, endTm = {};

    // Parse start date
    int startYear, startMonth, startDay;
    sscanf(dateStart.c_str(), "%d-%d-%d", &startYear, &startMonth, &startDay);
    startTm.tm_year = startYear - 1900; // Years since 1900
    startTm.tm_mon = startMonth - 1;    // Months since January (0-11)
    startTm.tm_mday = startDay;

    // Parse end date
    int endYear, endMonth, endDay;
    sscanf(dateEnd.c_str(), "%d-%d-%d", &endYear, &endMonth, &endDay);
    endTm.tm_year = endYear - 1900;
    endTm.tm_mon = endMonth - 1;
    endTm.tm_mday = endDay;

    // Convert to time_t (seconds since epoch)
    std::time_t startTime = std::mktime(&startTm);
    std::time_t endTime = std::mktime(&endTm);

    // Calculate difference in seconds, convert to days
    if (startTime != -1 && endTime != -1)
    {
        return static_cast<int>(std::difftime(endTime, startTime) / (60 * 60 * 24));
    }

    return 0; // Default if conversion fails
}

// Format duration from minutes to hours and minutes
std::string Tracker::formatDuration(double minutes)
{
    int hours = static_cast<int>(minutes) / 60;
    int mins = static_cast<int>(minutes) % 60;

    std::stringstream ss;
    if (hours > 0)
    {
        ss << hours << "h ";
    }
    ss << mins << "m";

    return ss.str();
}

// --- Search and Filter Functions ---

void Tracker::searchActivities()
{
    if (activities.empty())
    {
        std::cout << COLOR_YELLOW << "No activities recorded yet." << COLOR_RESET << std::endl;
        waitForEnter();
        return;
    }

    displaySearchMenu();
    int option = getIntegerInput("Enter option: ", 0, 2);

    switch (option)
    {
    case 0: // Back to main menu
        return;
    case 1:
    { // Search by keyword
        std::string keyword = getStringInput("Enter search keyword", "");
        if (!keyword.empty())
        {
            searchByKeyword(keyword);
        }
        break;
    }
    case 2:
    { // Search by date
        std::string date = getDateInput("Enter date (YYYY-MM-DD)", getCurrentDate());
        searchByDate(date);
        break;
    }
    }
}

void Tracker::searchByKeyword(const std::string &keyword)
{
    clearScreen();
    std::cout << "===================================" << std::endl;
    std::cout << "  " << COLOR_YELLOW << "SEARCH RESULTS FOR: " << COLOR_GREEN << keyword << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;

    std::vector<Activity> results;
    std::string lowerKeyword = keyword;
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);

    // Check each activity
    for (const auto &act : activities)
    {
        // Check if keyword matches activity type
        std::string typeName = getActivityTypeName(act.type);
        std::string lowerTypeName = typeName;
        std::transform(lowerTypeName.begin(), lowerTypeName.end(), lowerTypeName.begin(), ::tolower);

        // Remove color codes for comparison
        lowerTypeName = std::regex_replace(lowerTypeName, std::regex("\033\\[[0-9;]*m"), "");

        if (lowerTypeName.find(lowerKeyword) != std::string::npos ||
            act.date.find(keyword) != std::string::npos)
        {
            results.push_back(act);
        }
    }

    if (results.empty())
    {
        std::cout << "No activities found matching '" << keyword << "'." << std::endl;
    }
    else
    {
        std::cout << "Found " << results.size() << " matching activities:" << std::endl
                  << std::endl;

        std::cout << std::left
                  << std::setw(3) << "ID" << " | "
                  << std::setw(10) << "Type" << " | "
                  << std::setw(10) << "Date" << " | "
                  << std::setw(10) << "Duration" << " | "
                  << std::setw(10) << "Distance" << " | "
                  << std::setw(5) << "Reps" << std::endl;
        std::cout << "---+------------+------------+------------+------------+-------" << std::endl;

        std::cout << std::fixed << std::setprecision(1);

        for (size_t i = 0; i < results.size(); ++i)
        {
            const auto &act = results[i];
            std::cout << std::left
                      << std::setw(3) << (i + 1) << " | "
                      << std::setw(18) << getActivityTypeName(act.type) << " | "
                      << std::setw(10) << act.date << " | "
                      << std::setw(10) << act.duration << " | ";

            if (act.type == ActivityType::RUNNING || act.type == ActivityType::WALKING || act.type == ActivityType::SWIMMING)
            {
                std::cout << std::setprecision(2) << std::setw(10) << act.distance << std::setprecision(1) << " | ";
            }
            else
            {
                std::cout << std::setw(10) << "N/A" << " | ";
            }

            if (act.type == ActivityType::STRENGTH)
            {
                std::cout << std::setw(5) << act.repetitions;
            }
            else
            {
                std::cout << std::setw(5) << "N/A";
            }
            std::cout << std::endl;
        }
    }

    waitForEnter();
}

void Tracker::searchByDate(const std::string &date)
{
    clearScreen();
    std::cout << "===================================" << std::endl;
    std::cout << "  " << COLOR_YELLOW << "ACTIVITIES ON: " << COLOR_GREEN << date << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;

    std::vector<Activity> results;

    for (const auto &act : activities)
    {
        if (act.date == date)
        {
            results.push_back(act);
        }
    }

    if (results.empty())
    {
        std::cout << "No activities found on " << date << "." << std::endl;
    }
    else
    {
        std::cout << "Found " << results.size() << " activities on " << date << ":" << std::endl
                  << std::endl;

        std::cout << std::left
                  << std::setw(3) << "ID" << " | "
                  << std::setw(10) << "Type" << " | "
                  << std::setw(10) << "Duration" << " | "
                  << std::setw(10) << "Distance" << " | "
                  << std::setw(5) << "Reps" << std::endl;
        std::cout << "---+------------+------------+------------+-------" << std::endl;

        std::cout << std::fixed << std::setprecision(1);

        for (size_t i = 0; i < results.size(); ++i)
        {
            const auto &act = results[i];
            std::cout << std::left
                      << std::setw(3) << (i + 1) << " | "
                      << std::setw(18) << getActivityTypeName(act.type) << " | "
                      << std::setw(10) << act.duration << " | ";

            if (act.type == ActivityType::RUNNING || act.type == ActivityType::WALKING || act.type == ActivityType::SWIMMING)
            {
                std::cout << std::setprecision(2) << std::setw(10) << act.distance << std::setprecision(1) << " | ";
            }
            else
            {
                std::cout << std::setw(10) << "N/A" << " | ";
            }

            if (act.type == ActivityType::STRENGTH)
            {
                std::cout << std::setw(5) << act.repetitions;
            }
            else
            {
                std::cout << std::setw(5) << "N/A";
            }
            std::cout << std::endl;
        }
    }

    waitForEnter();
}

void Tracker::filterActivities()
{
    if (activities.empty())
    {
        std::cout << COLOR_YELLOW << "No activities recorded yet." << COLOR_RESET << std::endl;
        waitForEnter();
        return;
    }

    displayFilterMenu();
    int option = getIntegerInput("Enter option: ", 0, 3);

    switch (option)
    {
    case 0: // Back to main menu
        return;
    case 1:
    {                     // Filter by type
        displayAddMenu(); // Reuse Add menu to select activity type
        int typeOption = getIntegerInput("Select activity type: ", 0, 5);
        if (typeOption == 0)
            return;

        ActivityType type;
        switch (typeOption)
        {
        case 1:
            type = ActivityType::RUNNING;
            break;
        case 2:
            type = ActivityType::WALKING;
            break;
        case 3:
            type = ActivityType::SWIMMING;
            break;
        case 4:
            type = ActivityType::CARDIO;
            break;
        case 5:
            type = ActivityType::STRENGTH;
            break;
        default:
            return;
        }

        filterByType(type);
        break;
    }
    case 2:
    { // Filter by date range
        std::string startDate = getDateInput("Enter start date (YYYY-MM-DD)", "2000-01-01");
        std::string endDate = getDateInput("Enter end date (YYYY-MM-DD)", getCurrentDate());

        if (startDate > endDate)
        {
            std::swap(startDate, endDate);
            std::cout << COLOR_YELLOW << "Dates were swapped to ensure correct range." << COLOR_RESET << std::endl;
        }

        filterByDateRange(startDate, endDate);
        break;
    }
    case 3:
    { // Filter by duration
        double minDuration = getDoubleInput("Enter minimum duration (minutes): ", 0.0, true);
        double maxDuration = getDoubleInput("Enter maximum duration (minutes, 0 for no maximum): ", 0.0, true);

        if (maxDuration > 0 && minDuration > maxDuration)
        {
            std::swap(minDuration, maxDuration);
            std::cout << COLOR_YELLOW << "Duration values were swapped to ensure correct range." << COLOR_RESET << std::endl;
        }

        filterByDuration(minDuration, maxDuration);
        break;
    }
    }
}

void Tracker::filterByType(ActivityType type)
{
    clearScreen();
    std::cout << "===================================" << std::endl;
    std::cout << "  " << COLOR_YELLOW << "FILTERED BY TYPE: " << getActivityTypeName(type) << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;

    std::vector<Activity> results;

    for (const auto &act : activities)
    {
        if (act.type == type)
        {
            results.push_back(act);
        }
    }

    if (results.empty())
    {
        std::cout << "No activities found of type " << getActivityTypeName(type) << "." << std::endl;
    }
    else
    {
        std::cout << "Found " << results.size() << " " << getActivityTypeName(type) << " activities:" << std::endl
                  << std::endl;

        std::cout << std::left
                  << std::setw(3) << "ID" << " | "
                  << std::setw(10) << "Date" << " | "
                  << std::setw(10) << "Duration" << " | ";

        if (type == ActivityType::RUNNING || type == ActivityType::WALKING || type == ActivityType::SWIMMING)
        {
            std::cout << std::setw(10) << "Distance" << " | ";
        }

        if (type == ActivityType::STRENGTH)
        {
            std::cout << std::setw(5) << "Reps";
        }

        std::cout << std::endl;
        std::cout << "---+------------+------------+";

        if (type == ActivityType::RUNNING || type == ActivityType::WALKING || type == ActivityType::SWIMMING)
        {
            std::cout << "------------+";
        }

        if (type == ActivityType::STRENGTH)
        {
            std::cout << "-------";
        }

        std::cout << std::endl;

        std::cout << std::fixed << std::setprecision(1);

        for (size_t i = 0; i < results.size(); ++i)
        {
            const auto &act = results[i];
            std::cout << std::left
                      << std::setw(3) << (i + 1) << " | "
                      << std::setw(10) << act.date << " | "
                      << std::setw(10) << act.duration << " | ";

            if (type == ActivityType::RUNNING || type == ActivityType::WALKING || type == ActivityType::SWIMMING)
            {
                std::cout << std::setprecision(2) << std::setw(10) << act.distance << std::setprecision(1);
            }

            if (type == ActivityType::STRENGTH)
            {
                std::cout << std::setw(5) << act.repetitions;
            }

            std::cout << std::endl;
        }

        // Show average statistics
        double totalDuration = 0.0;
        double totalDistance = 0.0;
        int totalReps = 0;

        for (const auto &act : results)
        {
            totalDuration += act.duration;
            if (type == ActivityType::RUNNING || type == ActivityType::WALKING || type == ActivityType::SWIMMING)
            {
                totalDistance += act.distance;
            }
            if (type == ActivityType::STRENGTH)
            {
                totalReps += act.repetitions;
            }
        }

        std::cout << std::endl
                  << COLOR_CYAN << "Summary Statistics:" << COLOR_RESET << std::endl;
        std::cout << "Average Duration: " << (totalDuration / results.size()) << " minutes" << std::endl;

        if (type == ActivityType::RUNNING || type == ActivityType::WALKING || type == ActivityType::SWIMMING)
        {
            std::cout << "Average Distance: " << std::setprecision(2) << (totalDistance / results.size()) << " kilometers" << std::endl;
            std::cout << "Total Distance: " << totalDistance << " kilometers" << std::endl;
        }

        if (type == ActivityType::STRENGTH)
        {
            std::cout << "Average Repetitions: " << (totalReps / results.size()) << std::endl;
            std::cout << "Total Repetitions: " << totalReps << std::endl;
        }
    }

    waitForEnter();
}

void Tracker::filterByDateRange(const std::string &startDate, const std::string &endDate)
{
    clearScreen();
    std::cout << "===================================" << std::endl;
    std::cout << "  " << COLOR_YELLOW << "FILTERED BY DATE RANGE" << COLOR_RESET << std::endl;
    std::cout << "  " << COLOR_CYAN << startDate << " to " << endDate << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;

    std::vector<Activity> results;

    for (const auto &act : activities)
    {
        if (isDateInRange(act.date, startDate, endDate))
        {
            results.push_back(act);
        }
    }

    if (results.empty())
    {
        std::cout << "No activities found in the date range." << std::endl;
    }
    else
    {
        std::cout << "Found " << results.size() << " activities in the date range:" << std::endl
                  << std::endl;

        // Display activities (similar to viewActivities but with the filtered results)
        std::cout << std::left
                  << std::setw(3) << "ID" << " | "
                  << std::setw(10) << "Type" << " | "
                  << std::setw(10) << "Date" << " | "
                  << std::setw(10) << "Duration" << " | "
                  << std::setw(10) << "Distance" << " | "
                  << std::setw(5) << "Reps" << std::endl;
        std::cout << "---+------------+------------+------------+------------+-------" << std::endl;

        std::cout << std::fixed << std::setprecision(1);

        for (size_t i = 0; i < results.size(); ++i)
        {
            const auto &act = results[i];
            std::cout << std::left
                      << std::setw(3) << (i + 1) << " | "
                      << std::setw(18) << getActivityTypeName(act.type) << " | "
                      << std::setw(10) << act.date << " | "
                      << std::setw(10) << act.duration << " | ";

            if (act.type == ActivityType::RUNNING || act.type == ActivityType::WALKING || act.type == ActivityType::SWIMMING)
            {
                std::cout << std::setprecision(2) << std::setw(10) << act.distance << std::setprecision(1) << " | ";
            }
            else
            {
                std::cout << std::setw(10) << "N/A" << " | ";
            }

            if (act.type == ActivityType::STRENGTH)
            {
                std::cout << std::setw(5) << act.repetitions;
            }
            else
            {
                std::cout << std::setw(5) << "N/A";
            }
            std::cout << std::endl;
        }

        // Show summary statistics for the date range
        int days = calculateDaysBetween(startDate, endDate) + 1; // +1 to include both start and end dates
        std::cout << std::endl
                  << COLOR_CYAN << "Summary Statistics for the " << days << " day period:" << COLOR_RESET << std::endl;
        std::cout << "Total Activities: " << results.size() << std::endl;
        std::cout << "Activities per Day: " << std::fixed << std::setprecision(1) << (static_cast<double>(results.size()) / days) << std::endl;

        // Calculate total duration by type
        std::map<ActivityType, double> durationByType;
        std::map<ActivityType, double> distanceByType;
        std::map<ActivityType, int> countByType;

        for (const auto &act : results)
        {
            durationByType[act.type] += act.duration;
            countByType[act.type]++;

            if (act.type == ActivityType::RUNNING || act.type == ActivityType::WALKING || act.type == ActivityType::SWIMMING)
            {
                distanceByType[act.type] += act.distance;
            }
        }

        for (const auto &pair : countByType)
        {
            std::cout << getActivityTypeName(pair.first) << ": " << pair.second << " activities, ";
            std::cout << "Avg Duration: " << (durationByType[pair.first] / pair.second) << " min";

            if (pair.first == ActivityType::RUNNING || pair.first == ActivityType::WALKING || pair.first == ActivityType::SWIMMING)
            {
                std::cout << ", Total Distance: " << std::setprecision(2) << distanceByType[pair.first] << " km";
            }

            std::cout << std::endl;
        }
    }

    waitForEnter();
}

void Tracker::filterByDuration(double minDuration, double maxDuration)
{
    clearScreen();
    std::cout << "===================================" << std::endl;
    std::cout << "  " << COLOR_YELLOW << "FILTERED BY DURATION" << COLOR_RESET << std::endl;
    if (maxDuration > 0)
    {
        std::cout << "  " << COLOR_CYAN << "Between " << minDuration << " and " << maxDuration << " minutes" << COLOR_RESET << std::endl;
    }
    else
    {
        std::cout << "  " << COLOR_CYAN << "Minimum " << minDuration << " minutes" << COLOR_RESET << std::endl;
    }
    std::cout << "===================================" << std::endl;

    std::vector<Activity> results;

    for (const auto &act : activities)
    {
        if (act.duration >= minDuration && (maxDuration <= 0 || act.duration <= maxDuration))
        {
            results.push_back(act);
        }
    }

    if (results.empty())
    {
        std::cout << "No activities found within the duration range." << std::endl;
    }
    else
    {
        std::cout << "Found " << results.size() << " activities within the duration range:" << std::endl
                  << std::endl;

        // Display activities
        std::cout << std::left
                  << std::setw(3) << "ID" << " | "
                  << std::setw(10) << "Type" << " | "
                  << std::setw(10) << "Date" << " | "
                  << std::setw(10) << "Duration" << " | "
                  << std::setw(10) << "Distance" << " | "
                  << std::setw(5) << "Reps" << std::endl;
        std::cout << "---+------------+------------+------------+------------+-------" << std::endl;

        std::cout << std::fixed << std::setprecision(1);

        for (size_t i = 0; i < results.size(); ++i)
        {
            const auto &act = results[i];
            std::cout << std::left
                      << std::setw(3) << (i + 1) << " | "
                      << std::setw(18) << getActivityTypeName(act.type) << " | "
                      << std::setw(10) << act.date << " | "
                      << std::setw(10) << act.duration << " | ";

            if (act.type == ActivityType::RUNNING || act.type == ActivityType::WALKING || act.type == ActivityType::SWIMMING)
            {
                std::cout << std::setprecision(2) << std::setw(10) << act.distance << std::setprecision(1) << " | ";
            }
            else
            {
                std::cout << std::setw(10) << "N/A" << " | ";
            }

            if (act.type == ActivityType::STRENGTH)
            {
                std::cout << std::setw(5) << act.repetitions;
            }
            else
            {
                std::cout << std::setw(5) << "N/A";
            }
            std::cout << std::endl;
        }

        // Calculate average
        double totalDuration = 0.0;
        for (const auto &act : results)
        {
            totalDuration += act.duration;
        }

        std::cout << std::endl
                  << COLOR_CYAN << "Average Duration: " << (totalDuration / results.size()) << " minutes" << COLOR_RESET << std::endl;
    }

    waitForEnter();
}

// --- Data Visualization Functions ---

void Tracker::showProgressChart()
{
    if (activities.empty())
    {
        std::cout << COLOR_YELLOW << "No activities recorded yet." << COLOR_RESET << std::endl;
        waitForEnter();
        return;
    }

    clearScreen();
    std::cout << "===================================" << std::endl;
    std::cout << "        " << COLOR_YELLOW << "PROGRESS CHART" << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;

    // Group activities by date
    std::map<std::string, double> dailyDuration;
    std::map<std::string, double> dailyDistance;
    std::set<std::string> dates;

    for (const auto &act : activities)
    {
        dailyDuration[act.date] += act.duration;
        dates.insert(act.date);

        if (act.type == ActivityType::RUNNING ||
            act.type == ActivityType::WALKING ||
            act.type == ActivityType::SWIMMING)
        {
            dailyDistance[act.date] += act.distance;
        }
    }

    // Sort dates for chronological display
    std::vector<std::string> sortedDates(dates.begin(), dates.end());
    std::sort(sortedDates.begin(), sortedDates.end());

    // Find max values for scaling
    double maxDuration = 0.0;
    double maxDistance = 0.0;

    for (const auto &date : sortedDates)
    {
        maxDuration = std::max(maxDuration, dailyDuration[date]);
        maxDistance = std::max(maxDistance, dailyDistance[date]);
    }

    // Display header
    std::cout << COLOR_CYAN << "Activity Duration Over Time:" << COLOR_RESET << std::endl;
    std::cout << "(Each " << COLOR_GREEN << "█" << COLOR_RESET << " represents approximately "
              << std::fixed << std::setprecision(1) << (maxDuration / 40.0) << " minutes)" << std::endl
              << std::endl;

    // Display chart
    const int chartWidth = 40;

    for (const auto &date : sortedDates)
    {
        std::cout << date << " | ";

        // Calculate bar length based on duration
        int barLength = (maxDuration > 0) ? std::max(1, static_cast<int>((dailyDuration[date] / maxDuration) * chartWidth)) : 0;

        // Print the bar
        for (int i = 0; i < barLength; i++)
        {
            std::cout << COLOR_GREEN << "█" << COLOR_RESET;
        }

        std::cout << " " << std::fixed << std::setprecision(1) << dailyDuration[date] << " min";
        std::cout << std::endl;
    }

    // Now do the same for distance if applicable
    if (maxDistance > 0)
    {
        std::cout << std::endl
                  << COLOR_CYAN << "Distance Covered Over Time:" << COLOR_RESET << std::endl;
        std::cout << "(Each " << COLOR_BLUE << "█" << COLOR_RESET << " represents approximately "
                  << std::fixed << std::setprecision(2) << (maxDistance / 40.0) << " km)" << std::endl
                  << std::endl;

        for (const auto &date : sortedDates)
        {
            if (dailyDistance[date] > 0)
            {
                std::cout << date << " | ";

                // Calculate bar length based on distance
                int barLength = std::max(1, static_cast<int>((dailyDistance[date] / maxDistance) * chartWidth));

                // Print the bar
                for (int i = 0; i < barLength; i++)
                {
                    std::cout << COLOR_BLUE << "█" << COLOR_RESET;
                }

                std::cout << " " << std::fixed << std::setprecision(2) << dailyDistance[date] << " km";
                std::cout << std::endl;
            }
        }
    }

    waitForEnter();
}

void Tracker::showActivityDistribution()
{
    if (activities.empty())
    {
        std::cout << COLOR_YELLOW << "No activities recorded yet." << COLOR_RESET << std::endl;
        waitForEnter();
        return;
    }

    clearScreen();
    std::cout << "===================================" << std::endl;
    std::cout << "    " << COLOR_YELLOW << "ACTIVITY DISTRIBUTION" << COLOR_RESET << std::endl;
    std::cout << "===================================" << std::endl;

    // Count each activity type
    std::map<ActivityType, int> typeCounts;
    int totalActivities = 0;

    for (const auto &act : activities)
    {
        typeCounts[act.type]++;
        totalActivities++;
    }

    // Calculate percentages and display
    std::cout << COLOR_CYAN << "Distribution of Activity Types:" << COLOR_RESET << std::endl
              << std::endl;

    const int chartWidth = 40;

    for (int i = 0; i < 5; i++)
    {
        ActivityType type = static_cast<ActivityType>(i);
        int count = typeCounts[type];
        double percentage = (totalActivities > 0) ? (static_cast<double>(count) / totalActivities) * 100 : 0;

        // Calculate bar length
        int barLength = (percentage > 0) ? std::max(1, static_cast<int>((percentage / 100) * chartWidth)) : 0;

        // Choose color based on type
        std::string barColor;
        switch (type)
        {
        case ActivityType::RUNNING:
            barColor = COLOR_GREEN;
            break;
        case ActivityType::WALKING:
            barColor = COLOR_BLUE;
            break;
        case ActivityType::SWIMMING:
            barColor = COLOR_CYAN;
            break;
        case ActivityType::CARDIO:
            barColor = COLOR_MAGENTA;
            break;
        case ActivityType::STRENGTH:
            barColor = COLOR_YELLOW;
            break;
        default:
            barColor = COLOR_RESET;
        }

        // Print type name and bar
        std::cout << std::left << std::setw(10) << getActivityTypeName(type) << " | ";

        for (int j = 0; j < barLength; j++)
        {
            std::cout << barColor << "█" << COLOR_RESET;
        }

        std::cout << " " << count << " (" << std::fixed << std::setprecision(1) << percentage << "%)" << std::endl;
    }

    // Display additional statistics
    std::cout << std::endl
              << COLOR_CYAN << "Total Activities: " << totalActivities << COLOR_RESET << std::endl;

    // Calculate and display total duration and distance by type
    std::map<ActivityType, double> totalDurations;
    std::map<ActivityType, double> totalDistances;

    for (const auto &act : activities)
    {
        totalDurations[act.type] += act.duration;

        if (act.type == ActivityType::RUNNING ||
            act.type == ActivityType::WALKING ||
            act.type == ActivityType::SWIMMING)
        {
            totalDistances[act.type] += act.distance;
        }
    }

    std::cout << std::endl
              << COLOR_CYAN << "Total Time Spent by Activity Type:" << COLOR_RESET << std::endl;

    for (int i = 0; i < 5; i++)
    {
        ActivityType type = static_cast<ActivityType>(i);
        if (typeCounts[type] > 0)
        {
            std::cout << getActivityTypeName(type) << ": "
                      << std::fixed << std::setprecision(1) << totalDurations[type] << " minutes" << std::endl;
        }
    }

    std::cout << std::endl
              << COLOR_CYAN << "Total Distance by Activity Type:" << COLOR_RESET << std::endl;

    bool hasDistanceData = false;
    for (int i = 0; i < 3; i++)
    { // Only first three types have distance
        ActivityType type = static_cast<ActivityType>(i);
        if (typeCounts[type] > 0)
        {
            std::cout << getActivityTypeName(type) << ": "
                      << std::fixed << std::setprecision(2) << totalDistances[type] << " kilometers" << std::endl;
            hasDistanceData = true;
        }
    }

    if (!hasDistanceData)
    {
        std::cout << "No distance data available." << std::endl;
    }

    waitForEnter();
}

// --- Goal Management Functions ---

void Tracker::manageGoals()
{
    displayGoalsMenu();
    int option = getIntegerInput("Enter option: ", 0, 4);

    switch (option)
    {
    case 0: // Back to main menu
        return;
    case 1: // Add new goal
        addGoal();
        break;
    case 2: // View all goals
        viewGoals();
        break;
    case 3: // View achieved goals
        // For achieved goals, update implementation to filter in the viewGoals function
        std::cout << "This feature is not yet implemented." << std::endl;
        waitForEnter();
        break;
    case 4: // View pending goals
        // For pending goals, update implementation to filter in the viewGoals function
        std::cout << "This feature is not yet implemented." << std::endl;
        waitForEnter();
        break;
    }
}
