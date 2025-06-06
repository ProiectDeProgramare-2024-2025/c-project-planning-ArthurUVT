# `app_2`: Data Analysis & Visualization

## 1. Purpose

`app_2` provides users with tools to analyze their recorded sports activities, derive insights, and visualize their progress. It operates on the data generated and managed by `app_1` (i.e., `activities.csv` and potentially `goals.csv`).

## 2. Core Functionalities (Command-Line Operations)

`app_2` will also be invoked with specific commands and arguments. Example command structure: `app_2 <command> [options] [arguments]`

### 2.1. Statistical Reporting

*   **Generate Summary Statistics:**
    *   Command: `app_2 stats [--period <week|month|year|all>] [--activity_type <type>]`
    *   Calculates and displays key statistics such as:
        *   Total activities logged.
        *   Total duration.
        *   Total distance covered (if applicable).
        *   Total calories burned.
        *   Average duration/distance per activity.
    *   Statistics can be filtered by time period and/or activity type.

*   **View Streaks/Consistency:**
    *   Command: `app_2 streaks [--activity_type <type>]`
    *   Identifies and displays consecutive days/weeks of activity.

### 2.2. Activity Filtering & Searching

*   **Filter Activities:**
    *   Command: `app_2 filter --activity_type <type> [--date_range <start_YYYY-MM-DD> <end_YYYY-MM-DD>] [--min_duration <minutes>] [--min_distance <km>]`
    *   Displays a list of activities matching complex criteria.
    *   Useful for finding specific types of workouts or workouts within a certain performance range.

*   **Search Activities by Notes/Keywords:**
    *   Command: `app_2 search_notes --keyword "<text>"`
    *   Searches the 'notes' field of activities for specific keywords.

### 2.3. Data Visualization (ASCII Charts)

*   **Progress Charts:**
    *   Command: `app_2 chart_progress --metric <duration|distance|calories> --period <week|month> [--activity_type <type>]`
    *   Generates simple ASCII bar charts or line graphs to show trends in chosen metrics over time (e.g., total distance run per week for the last month).

*   **Activity Type Distribution:**
    *   Command: `app_2 chart_distribution --period <month|year|all>`
    *   Displays an ASCII pie chart or bar chart showing the proportion of different activity types.

### 2.4. Goal Progress (Read-Only)

*   **View Goal Progress:**
    *   Command: `app_2 view_goal_progress [--goal_id <id> | --all_active]`
    *   Reads `goals.csv` and `activities.csv` to calculate and display progress towards one or all active goals.
    *   Presents data like: Goal target, current progress, percentage complete, days remaining until deadline.

## 3. Data Source

*   Primarily reads from `activities.csv`.
*   May also read from `goals.csv` for progress tracking features.
*   `app_2` does not modify these files; it only performs read operations.

## 4. Output

*   All output is displayed on the command line.
*   Clear, well-formatted text for reports and statistics.
*   Simple ASCII-based charts for visualization. 

## 5. Technical Implementation Approach

*   **Command-Line Argument Parsing:**
    *   Similar to `app_1`, `main` function in `app_2` will parse `argc` and `argv` to determine the requested command (e.g., `stats`, `filter`, `chart_progress`) and its options.
    *   Logic will branch based on the identified command to call specific handler functions.

*   **Data Loading (Read-Only):**
    *   `app_2` will read `activities.csv` and, for goal-related features, `goals.csv`.
    *   `std::ifstream`, `std::getline`, and `std::stringstream` will be used for parsing CSV data into `std::vector<Activity>` and `std::vector<Goal>` (using shared `Activity` and `Goal` struct definitions).
    *   Crucially, these file operations are strictly read-only; `app_2` does not write back to these CSV files.

*   **Statistical Calculation Logic:**
    *   Operations typically involve iterating over the loaded `std::vector<Activity>`.
    *   Filters (e.g., by date range, activity type) are applied by checking conditions within the loop or by creating temporary filtered vectors.
    *   Metrics like total duration, distance, and calories are accumulated. Averages are calculated from these sums and counts.
    *   For streaks: Activities would first be sorted by date. Then, iterate through the sorted list, comparing dates to identify consecutive days/weeks of activity, possibly using `std::chrono` for date arithmetic if precise day/week differences are needed.

*   **Filtering and Searching Implementation:**
    *   **General Filtering:** Iterate through the `std::vector<Activity>`. For each activity, a series of conditional checks (e.g., `activity.type == chosen_type && activity.date >= start_date`) determines if it matches the user-specified criteria.
    *   **Keyword Search:** For each activity, use `std::string::find()` on its `notes` field to check for the presence of the search keyword.

*   **ASCII Chart Generation:**
    *   **Data Aggregation:**
        *   Group activity data into categories or time bins relevant to the chart (e.g., sum of distances per week). `std::map<std::string, double>` can store aggregated values (e.g., week identifier mapped to total distance).
        *   Date components might be extracted (e.g., using `std::tm` after parsing date strings) to form keys for aggregation (e.g., week number, month).
    *   **Scaling for Display:**
        *   Determine the maximum aggregated value to be plotted.
        *   Calculate a scale factor: `value_per_character = max_value / max_chart_width_characters`. This maps data values to the limited width of an ASCII chart.
    *   **Rendering ASCII:**
        *   **Bar Charts:** For each aggregated data point, print its label followed by a number of repeated characters (e.g., `'*'`) proportional to its value (`(int)(value / value_per_character)`).
        *   **Distribution/Pie Charts (Simple):** Calculate the percentage for each activity type. Display these percentages or represent them as segments in a simple character-based representation.

*   **Goal Progress Calculation:**
    *   Load all goals from `goals.csv` and all activities from `activities.csv`.
    *   For each goal (or a specific one if an ID is provided), iterate through the activities.
    *   Filter activities that match the goal's criteria (e.g., type, date before deadline) and contribute to its metric.
    *   Sum the relevant metric (e.g., total distance run for a running distance goal) from these filtered activities.
    *   Compare the accumulated sum against the goal's target to show progress.

*   **Output Formatting:**
    *   Use `std::cout` for all output.
    *   Employ `<iomanip>` manipulators (`std::setw`, `std::left`, `std::fixed`, `std::setprecision`) to format tables, statistics, and chart labels for clarity and readability in the console.

*   **Code Structure:**
    *   Organize code into functions for each main command (e.g., `handle_stats_command`, `handle_filter_command`).
    *   Create helper functions for common tasks like date parsing/comparison, specific calculations, and individual chart drawing routines to promote modularity and reusability. 