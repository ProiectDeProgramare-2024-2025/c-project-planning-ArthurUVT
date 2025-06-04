/**
 * Sports Activity Tracking System
 * This application allows users to record various sports activities
 * and view statistics about their performance.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ANSI Color Codes
#define COLOR_RESET "\033[0m"
#define COLOR_CYAN "\033[0;36m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\033[0;31m"

#define MAX_ACTIVITIES 100
#define MAX_NAME_LENGTH 50
#define MAX_ACTIVITY_TYPES 5

// Activity types
typedef enum
{
    RUNNING = 0,
    WALKING = 1,
    SWIMMING = 2,
    CARDIO = 3,
    STRENGTH = 4
} ActivityType;

// Struct to hold activity data
typedef struct
{
    ActivityType type;
    char date[11];   // Format: YYYY-MM-DD
    float duration;  // in minutes
    float distance;  // in kilometers (for applicable activities)
    int repetitions; // for strength activities
} Activity;

// Global data
Activity activities[MAX_ACTIVITIES];
int activityCount = 0;

// Statistics for each activity type
int activityTypeCounts[MAX_ACTIVITY_TYPES] = {0};
float activityTypeTotalDurations[MAX_ACTIVITY_TYPES] = {0};
float activityTypeTotalDistances[MAX_ACTIVITY_TYPES] = {0};

// Function prototypes
void clearScreen();
void displayMainMenu();
void displayAddMenu();
void displayViewMenu();
void displayStatsMenu();
void addActivity();
void addRunning();
void addWalking();
void addSwimming();
void addCardio();
void addStrength();
void viewActivities();
void viewStatistics();
void updateStatistics(Activity newActivity);
const char *getActivityTypeName(ActivityType type);
void getCurrentDate(char *dateStr);
void waitForEnter();
void loadActivitiesFromFile(const char *filename);
void saveActivitiesToFile(const char *filename);
int getIntegerInput(const char *prompt, int minVal, int maxVal);
float getFloatInput(const char *prompt, float minVal);
void getStringInput(const char *prompt, char *buffer, int bufferSize, const char *defaultValue);

int main()
{
    int option;
    int inSubmenu = 0;
    int submenuType = 0;

    const char *dataFilename = "activities.csv";
    loadActivitiesFromFile(dataFilename);

    do
    {
        if (!inSubmenu)
        {
            clearScreen();
            displayMainMenu();
            option = getIntegerInput("Enter option: ", 0, 3);

            switch (option)
            {
            case 1: // Add activity
                inSubmenu = 1;
                submenuType = 1;
                break;
            case 2: // View activities
                inSubmenu = 1;
                submenuType = 2;
                break;
            case 3: // View statistics
                inSubmenu = 1;
                submenuType = 3;
                break;
            case 0: // Exit
                printf("Exiting the application. Goodbye!\n");
                break;
            }
        }
        else
        {
            clearScreen();

            switch (submenuType)
            {
            case 1: // Add menu
                displayAddMenu();
                option = getIntegerInput("Enter option: ", 0, 5);

                switch (option)
                {
                case 1: // Running
                    addRunning();
                    break;
                case 2: // Walking
                    addWalking();
                    break;
                case 3: // Swimming
                    addSwimming();
                    break;
                case 4: // Cardio
                    addCardio();
                    break;
                case 5: // Strength
                    addStrength();
                    break;
                case 0: // Back to main menu
                    inSubmenu = 0;
                    break;
                }
                break;

            case 2: // View activities
                viewActivities();
                inSubmenu = 0;
                break;

            case 3: // View statistics
                viewStatistics();
                inSubmenu = 0;
                break;
            }
        }

    } while (option != 0 || inSubmenu);

    saveActivitiesToFile(dataFilename);

    return 0;
}

// Clear the console screen
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Display the main menu
void displayMainMenu()
{
    printf("===================================\n");
    printf("    " COLOR_CYAN "SPORTS ACTIVITY TRACKER" COLOR_RESET "\n");
    printf("===================================\n");
    printf("1 - Add Activity\n");
    printf("2 - View Activities\n");
    printf("3 - View Statistics\n");
    printf("0 - Exit\n");
    printf("===================================\n");
}

// Display the add activity submenu
void displayAddMenu()
{
    printf("===================================\n");
    printf("        " COLOR_YELLOW "ADD NEW ACTIVITY" COLOR_RESET "\n");
    printf("===================================\n");
    printf("1 - " COLOR_GREEN "Running" COLOR_RESET "\n");
    printf("2 - " COLOR_GREEN "Walking" COLOR_RESET "\n");
    printf("3 - " COLOR_GREEN "Swimming" COLOR_RESET "\n");
    printf("4 - " COLOR_GREEN "Cardio" COLOR_RESET "\n");
    printf("5 - " COLOR_GREEN "Strength" COLOR_RESET "\n");
    printf("0 - Back to Main Menu\n");
    printf("===================================\n");
}

// Get current date in YYYY-MM-DD format
void getCurrentDate(char *dateStr)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(dateStr, "%04d-%02d-%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
}

// Wait for user to press enter
void waitForEnter()
{
    printf("\nPress Enter to continue...");
    getchar();
}

// Get string name of activity type
const char *getActivityTypeName(ActivityType type)
{
    switch (type)
    {
    case RUNNING:
        return COLOR_GREEN "Running" COLOR_RESET;
    case WALKING:
        return COLOR_GREEN "Walking" COLOR_RESET;
    case SWIMMING:
        return COLOR_GREEN "Swimming" COLOR_RESET;
    case CARDIO:
        return COLOR_GREEN "Cardio" COLOR_RESET;
    case STRENGTH:
        return COLOR_GREEN "Strength" COLOR_RESET;
    default:
        return COLOR_RED "Unknown" COLOR_RESET;
    }
}

// Update statistics with new activity
void updateStatistics(Activity newActivity)
{
    activityTypeCounts[newActivity.type]++;
    activityTypeTotalDurations[newActivity.type] += newActivity.duration;

    // Only update distance for relevant activities
    if (newActivity.type == RUNNING || newActivity.type == WALKING || newActivity.type == SWIMMING)
    {
        activityTypeTotalDistances[newActivity.type] += newActivity.distance;
    }
}

// Add a running activity
void addRunning()
{
    clearScreen();
    printf("===================================\n");
    printf("          ADD " COLOR_GREEN "RUNNING" COLOR_RESET "\n");
    printf("===================================\n");

    if (activityCount >= MAX_ACTIVITIES)
    {
        printf(COLOR_RED "Error: Maximum number of activities reached.\n" COLOR_RESET);
        waitForEnter();
        return;
    }

    Activity newActivity;
    newActivity.type = RUNNING;

    char defaultDate[11];
    getCurrentDate(defaultDate);
    getStringInput("Date (YYYY-MM-DD)", newActivity.date, sizeof(newActivity.date), defaultDate);

    newActivity.duration = getFloatInput("Duration (minutes, > 0): ", 0.0f);
    newActivity.distance = getFloatInput("Distance (kilometers, >= 0): ", 0.0f);

    newActivity.repetitions = 0; // Not relevant for running

    activities[activityCount++] = newActivity;
    updateStatistics(newActivity);

    printf("\n" COLOR_GREEN "Running activity added successfully!" COLOR_RESET "\n");
    waitForEnter();
}

// Adding a walking activity
void addWalking()
{
    clearScreen();
    printf("===================================\n");
    printf("          ADD " COLOR_GREEN "WALKING" COLOR_RESET "\n");
    printf("===================================\n");

    if (activityCount >= MAX_ACTIVITIES)
    {
        printf(COLOR_RED "Error: Maximum number of activities reached.\n" COLOR_RESET);
        waitForEnter();
        return;
    }

    Activity newActivity;
    newActivity.type = WALKING;

    char defaultDate[11];
    getCurrentDate(defaultDate);
    getStringInput("Date (YYYY-MM-DD)", newActivity.date, sizeof(newActivity.date), defaultDate);

    newActivity.duration = getFloatInput("Duration (minutes, > 0): ", 0.0f);
    newActivity.distance = getFloatInput("Distance (kilometers, >= 0): ", 0.0f);

    newActivity.repetitions = 0; // Not relevant for walking

    activities[activityCount++] = newActivity;
    updateStatistics(newActivity);

    printf("\n" COLOR_GREEN "Walking activity added successfully!" COLOR_RESET "\n");
    waitForEnter();
}

// Add a swimming activity
void addSwimming()
{
    clearScreen();
    printf("===================================\n");
    printf("          ADD " COLOR_GREEN "SWIMMING" COLOR_RESET "\n");
    printf("===================================\n");

    if (activityCount >= MAX_ACTIVITIES)
    {
        printf(COLOR_RED "Error: Maximum number of activities reached.\n" COLOR_RESET);
        waitForEnter();
        return;
    }

    Activity newActivity;
    newActivity.type = SWIMMING;

    char defaultDate[11];
    getCurrentDate(defaultDate);
    getStringInput("Date (YYYY-MM-DD)", newActivity.date, sizeof(newActivity.date), defaultDate);

    newActivity.duration = getFloatInput("Duration (minutes, > 0): ", 0.0f);
    newActivity.distance = getFloatInput("Distance (kilometers, >= 0): ", 0.0f);

    newActivity.repetitions = 0; // Not relevant for swimming

    activities[activityCount++] = newActivity;
    updateStatistics(newActivity);

    printf("\n" COLOR_GREEN "Swimming activity added successfully!" COLOR_RESET "\n");
    waitForEnter();
}

// Adding a cardio activity
void addCardio()
{
    clearScreen();
    printf("===================================\n");
    printf("          ADD " COLOR_GREEN "CARDIO" COLOR_RESET "\n");
    printf("===================================\n");

    if (activityCount >= MAX_ACTIVITIES)
    {
        printf(COLOR_RED "Error: Maximum number of activities reached.\n" COLOR_RESET);
        waitForEnter();
        return;
    }

    Activity newActivity;
    newActivity.type = CARDIO;

    char defaultDate[11];
    getCurrentDate(defaultDate);
    getStringInput("Date (YYYY-MM-DD)", newActivity.date, sizeof(newActivity.date), defaultDate);

    newActivity.duration = getFloatInput("Duration (minutes, > 0): ", 0.0f);

    newActivity.distance = 0; // Not relevant for cardio
    newActivity.repetitions = 0;

    activities[activityCount++] = newActivity;
    updateStatistics(newActivity);

    printf("\n" COLOR_GREEN "Cardio activity added successfully!" COLOR_RESET "\n");
    waitForEnter();
}

// Adding a strength activity
void addStrength()
{
    clearScreen();
    printf("===================================\n");
    printf("          ADD " COLOR_GREEN "STRENGTH" COLOR_RESET "\n");
    printf("===================================\n");

    if (activityCount >= MAX_ACTIVITIES)
    {
        printf(COLOR_RED "Error: Maximum number of activities reached.\n" COLOR_RESET);
        waitForEnter();
        return;
    }

    Activity newActivity;
    newActivity.type = STRENGTH;

    char defaultDate[11];
    getCurrentDate(defaultDate);
    getStringInput("Date (YYYY-MM-DD)", newActivity.date, sizeof(newActivity.date), defaultDate);

    newActivity.duration = getFloatInput("Duration (minutes, > 0): ", 0.0f);
    newActivity.repetitions = getIntegerInput("Number of repetitions (>= 0): ", 0, 10000); // Example max value

    newActivity.distance = 0; // Not relevant for strength

    activities[activityCount++] = newActivity;
    updateStatistics(newActivity);

    printf("\n" COLOR_GREEN "Strength activity added successfully!" COLOR_RESET "\n");
    waitForEnter();
}

// View all recorded activities
void viewActivities()
{
    clearScreen();
    printf("===================================\n");
    printf("        " COLOR_YELLOW "VIEW ACTIVITIES" COLOR_RESET "\n");
    printf("===================================\n");

    if (activityCount == 0)
    {
        printf("No activities recorded yet.\n");
    }
    else
    {
        printf("ID | Type     | Date       | Duration | Distance | Reps\n");
        printf("---+----------+------------+----------+----------+------\n");

        for (int i = 0; i < activityCount; i++)
        {
            printf("%-2d | %-8s | %-10s | %-8.1f | ",
                   i + 1,
                   getActivityTypeName(activities[i].type),
                   activities[i].date,
                   activities[i].duration);

            if (activities[i].type == RUNNING || activities[i].type == WALKING || activities[i].type == SWIMMING)
            {
                printf("%-8.2f | ", activities[i].distance);
            }
            else
            {
                printf("N/A      | ");
            }

            if (activities[i].type == STRENGTH)
            {
                printf("%-4d", activities[i].repetitions);
            }
            else
            {
                printf("N/A ");
            }

            printf("\n");
        }
    }

    waitForEnter();
}

// View statistics about recorded activities
void viewStatistics()
{
    clearScreen();
    printf("===================================\n");
    printf("        " COLOR_YELLOW "VIEW STATISTICS" COLOR_RESET "\n");
    printf("===================================\n");

    if (activityCount == 0)
    {
        printf("No activities recorded yet.\n");
    }
    else
    {
        printf("Activity Type | Count | Avg Duration | Avg Distance\n");
        printf("-------------+-------+--------------+-------------\n");

        for (int i = 0; i < MAX_ACTIVITY_TYPES; i++)
        {
            if (activityTypeCounts[i] > 0)
            {
                float avgDuration = activityTypeTotalDurations[i] / activityTypeCounts[i];

                printf("%-12s | %-5d | %-12.1f | ",
                       getActivityTypeName((ActivityType)i),
                       activityTypeCounts[i],
                       avgDuration);

                if (i == RUNNING || i == WALKING || i == SWIMMING)
                {
                    float avgDistance = activityTypeTotalDistances[i] / activityTypeCounts[i];
                    printf("%-11.2f\n", avgDistance);
                }
                else
                {
                    printf("N/A\n");
                }
            }
        }
    }

    waitForEnter();
}

// Load activities from a CSV file
void loadActivitiesFromFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        // If the file doesn't exist, it's okay, maybe it's the first run.
        printf("Data file '%s' not found. Starting with no activities.\n", filename);
        waitForEnter();
        return;
    }

    int typeIndex;
    char date[11];
    float duration, distance;
    int repetitions;

    // Read activities line by line
    while (fscanf(file, "%d,%10[^,],%f,%f,%d\n",
                  &typeIndex, date, &duration, &distance, &repetitions) == 5)
    {
        if (activityCount >= MAX_ACTIVITIES)
        {
            printf("Warning: Maximum activity limit reached while loading. Some activities might not be loaded.\n");
            break;
        }

        Activity loadedActivity;
        loadedActivity.type = (ActivityType)typeIndex;
        strcpy(loadedActivity.date, date);
        loadedActivity.duration = duration;
        loadedActivity.distance = distance;
        loadedActivity.repetitions = repetitions;

        activities[activityCount++] = loadedActivity;
        updateStatistics(loadedActivity); // Update stats for loaded activities
    }

    fclose(file);
    printf("Loaded %d activities from '%s'.\n", activityCount, filename);
    waitForEnter();
}

// Save activities to a CSV file
void saveActivitiesToFile(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file for saving");
        return;
    }

    for (int i = 0; i < activityCount; i++)
    {
        fprintf(file, "%d,%s,%.1f,%.2f,%d\n",
                (int)activities[i].type,
                activities[i].date,
                activities[i].duration,
                activities[i].distance,
                activities[i].repetitions);
    }

    fclose(file);
    printf("Saved %d activities to '%s'.\n", activityCount, filename);
}

// Helper function to get validated integer input
int getIntegerInput(const char *prompt, int minVal, int maxVal)
{
    int value;
    int result;
    char buffer[100]; // Buffer to consume extra input

    while (1)
    {
        printf("%s", prompt);
        result = scanf("%d", &value);
        // Clear the input buffer
        fgets(buffer, sizeof(buffer), stdin);

        if (result == 1 && value >= minVal && value <= maxVal)
        {
            return value;
        }
        else
        {
            printf(COLOR_RED "Invalid input. Please enter an integer between %d and %d.\n" COLOR_RESET, minVal, maxVal);
        }
    }
}

// Helper function to get validated float input (greater than minVal)
float getFloatInput(const char *prompt, float minVal)
{
    float value;
    int result;
    char buffer[100];

    while (1)
    {
        printf("%s", prompt);
        result = scanf("%f", &value);
        // Clear the input buffer
        fgets(buffer, sizeof(buffer), stdin);

        if (result == 1 && value > minVal)
        { // Use > for duration, >= for distance/reps
            if (prompt[0] == 'D' && prompt[1] == 'i')
            { // Quick check for Distance prompt
                if (value >= minVal)
                    return value;
            }
            else
            {
                return value;
            }
        }

        // Adjust error message based on comparison type
        if (prompt[0] == 'D' && prompt[1] == 'i')
        { // Distance prompt
            printf(COLOR_RED "Invalid input. Please enter a number greater than or equal to %.1f.\n" COLOR_RESET, minVal);
        }
        else
        { // Duration prompt
            printf(COLOR_RED "Invalid input. Please enter a number greater than %.1f.\n" COLOR_RESET, minVal);
        }
    }
}

// Helper function to get string input with default value support
void getStringInput(const char *prompt, char *buffer, int bufferSize, const char *defaultValue)
{
    printf("%s [%s]: ", prompt, defaultValue);
    fgets(buffer, bufferSize, stdin);

    // Check if the user just pressed Enter (input is only newline)
    if (buffer[0] == '\n')
    {
        // Use default value
        strncpy(buffer, defaultValue, bufferSize - 1);
        buffer[bufferSize - 1] = '\0'; // Ensure null termination
    }
    else
    {
        // Remove trailing newline character if present
        buffer[strcspn(buffer, "\n")] = 0;
    }
    // Basic validation: ensure buffer is not empty if no default was used
    if (buffer[0] == '\0')
    {
        printf(COLOR_RED "Input cannot be empty. Using default: %s\n" COLOR_RESET, defaultValue);
        strncpy(buffer, defaultValue, bufferSize - 1);
        buffer[bufferSize - 1] = '\0';
    }
}