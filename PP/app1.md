# `app_1`: Activity & Goal Management

## 1. Purpose

`app_1` is the primary interface for managing sports activity data and personal fitness goals. It focuses on data entry, modification, and basic retrieval, ensuring that all information is accurately recorded and stored.

## 2. Core Functionalities (Command-Line Operations)

`app_1` will be invoked with specific commands and arguments to perform operations. Example command structure: `app_1 <command> [options] [arguments]`

### 2.1. Activity Management

*   **Add New Activity:**
    *   Command: `app_1 add_activity --type <type> --date <YYYY-MM-DD> --duration <minutes> [--distance <km>] [--calories <kcal>] [--notes "details"]`
    *   Prompts for mandatory fields if not provided via arguments.
    *   Validates input data (e.g., date format, positive numbers for duration/distance/calories).
    *   Appends the new activity to `activities.csv`.

*   **View Activities:**
    *   Command: `app_1 view_activities [--all | --id <activity_id> | --date <YYYY-MM-DD>]`
    *   Displays all activities, a specific activity by ID, or activities for a specific date.
    *   Formatted output for readability.

*   **Update Activity:**
    *   Command: `app_1 update_activity --id <activity_id> [--type <new_type>] [--date <new_date>] ...`
    *   Allows modification of one or more fields of an existing activity.
    *   Validates new data before updating `activities.csv`.

*   **Delete Activity:**
    *   Command: `app_1 delete_activity --id <activity_id>`
    *   Removes a specific activity from `activities.csv` after confirmation.

### 2.2. Goal Management

*   **Set New Goal:**
    *   Command: `app_1 add_goal --type <activity_type> --target_metric <distance/duration/calories> --target_value <value> --deadline <YYYY-MM-DD>`
    *   Allows users to define specific fitness goals (e.g., run 50km by 2023-12-31).
    *   Stores goals in `goals.csv`.

*   **View Goals:**
    *   Command: `app_1 view_goals [--all | --active | --completed]`
    *   Displays all goals, only active goals, or completed goals.
    *   May show progress towards active goals based on recorded activities.

*   **Update Goal:**
    *   Command: `app_1 update_goal --id <goal_id> [--target_value <new_value>] [--deadline <new_deadline>]`
    *   Modifies existing goal parameters.

*   **Delete Goal:**
    *   Command: `app_1 delete_goal --id <goal_id>`
    *   Removes a goal from `goals.csv`.

## 3. Data Persistence

*   All activity data is stored in `activities.csv`.
*   All goal data is stored in `goals.csv`.
*   Data is loaded at the start of an operation and saved upon successful completion of commands that modify data.
*   Robust error handling for file I/O operations (e.g., file not found, permission issues).

## 4. Input Validation

*   Strict validation for all user inputs:
    *   Date formats (YYYY-MM-DD).
    *   Numerical inputs (positive values for duration, distance, calories, goal targets).
    *   Activity types (from a predefined list or flexible string).
*   Clear error messages and guidance for correcting inputs. 

## 5. Technical Implementation Approach

*   **Command-Line Argument Parsing:**
    *   The `main` function will parse `argc` and `argv` to identify the command (e.g., `add_activity`, `view_goals`) and its associated options/arguments.
    *   A series of `if-else if` statements or a map of command strings to handler functions can be used to route to the appropriate logic.
    *   Option parsing (e.g., `--type`, `--id`) will involve iterating through arguments, identifying flags, and extracting their values.

*   **Data Structures:**
    *   `struct Activity` and `struct Goal` (likely defined in a shared `.h` file, e.g., `shared_structs.h` or `activity.h`) will be used to represent individual records. These structs will use `std::string` for text fields and appropriate numeric types (`int`, `float`, `double`) for other data.
    *   `std::vector<Activity>` and `std::vector<Goal>` will hold collections of these records in memory after loading from or before saving to files.

*   **CSV File Handling (Persistence):**
    *   **Loading:** `std::ifstream` will be used to open `activities.csv` and `goals.csv`.
        *   `std::getline` will read each line of the CSV.
        *   `std::stringstream` will be used to parse each line, with `std::getline(ss, field, ',')` to extract comma-separated values into the fields of the `Activity` or `Goal` structs.
    *   **Saving:** `std::ofstream` will be used to write to the CSV files.
        *   For adding new records, data can be appended.
        *   For updates or deletes, a common approach is to read all data into memory (`std::vector`), perform modifications on the vector, and then overwrite the entire file with the updated vector's contents.
    *   Data integrity for CSV fields containing commas or quotes might require handling (e.g., by enclosing fields in quotes and escaping internal quotes).

*   **Input Validation Logic:**
    *   **Date Validation:** Can be done using string manipulation and conversion to numeric components (year, month, day), followed by checks for valid ranges (e.g., month 1-12, day 1-31 respecting month/leap year). Regular expressions (`<regex>`) can also be used for format validation.
    *   **Numeric Validation:** `std::stoi`, `std::stof`, `std::stod` for conversions, wrapped in `try-catch` blocks to handle `std::invalid_argument` or `std::out_of_range`. Further checks for positivity or specific ranges.
    *   **String/Type Validation:** Comparisons against a predefined set of allowed activity types (if applicable) or ensuring strings are not empty.

*   **Core Operation Logic:**
    *   Each command (add, view, update, delete) will typically involve:
        1.  Loading relevant data from CSV into `std::vector`s (if not already loaded or if data might have changed).
        2.  Performing the operation on the in-memory `std::vector`(s) (e.g., adding an element, finding and modifying an element, removing an element).
        3.  If data was modified, saving the updated `std::vector`(s) back to the CSV file(s).
        4.  Displaying appropriate messages or data to the console.

*   **Error Handling:**
    *   Extensive use of `try-catch` blocks for file I/O (`std::ios_base::failure`) and data conversions.
    *   Clear, user-friendly error messages printed to `std::cerr` indicating the problem and how to potentially resolve it (e.g., incorrect command usage, file access issues). 