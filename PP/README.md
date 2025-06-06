# Sports Tracker - Two Application System

This project implements a sports activity tracking system split into two separate applications as per the specifications:

## Applications Overview

### Application 1 (app_1) - Core Application Management
**Student 1 responsibilities:**
- Adding, modifying, deleting sports activities
- Viewing all recorded activities and detailed statistics
- Managing data persistence (load/save to CSV)
- Implementing input validation and terminal UI

### Application 2 (app_2) - User Interaction Features
**Student 2 responsibilities:**
- Implementing search and filter options
- Generating ASCII-based visual progress charts
- Creating and tracking fitness goals
- Managing goal notifications and reports

## Building the Applications

```bash
# Compile both applications
g++ -std=c++11 -Wall -Wextra -o app_1 app_1.cpp
g++ -std=c++11 -Wall -Wextra -o app_2 app_2.cpp

# Or using CMake (if available)
mkdir build && cd build
cmake .. && make
```

## Application 1 Commands

```bash
# Add a new activity (interactive)
./app_1 add_activity <activity_ID>

# View specific activity by ID
./app_1 view_activity <activity_ID>

# View all activities
./app_1 view_activities

# Add a new goal
./app_1 add_goal <goal_ID> <activity_ID> <description> <deadline> <target_reps> <target_duration> <target_distance>

# View specific goal by ID
./app_1 view_goal <goal_ID>

# View all goals
./app_1 view_goals

# Modify existing goal
./app_1 modify_goal <goal_ID> <activity_ID> <description> <deadline> <target_reps> <target_duration> <target_distance>
```

## Application 2 Commands

```bash
# View general statistics
./app_2 view_statistics

# Filter statistics by activity and goal
./app_2 filter_statistics <activity_ID> <goal_ID>

# View progress visualization for a goal
./app_2 view_progres <goal_ID>

# Delete a goal
./app_2 delete_goal <goal_ID>

# Backup data to file
./app_2 backup <file_path>
```

## Activity Types (ID Mapping)
- 0: Running
- 1: Walking
- 2: Swimming
- 3: Cardio
- 4: Strength

## Data Files
- `activities_cpp.csv` - Stores activity data in format: `<activity_type>,<date>,<duration>,<distance>,<repetitions>`
- `activities_goals_cpp.csv` - Stores goal data in format: `<activity_type>,<description>,<deadline>,<target_distance>,<target_duration>,<target_reps>,<achieved>`

## Example Usage

```bash
# Add a running activity
./app_1 add_activity 1
# (Follow interactive prompts)

# Add a goal to run 100km this month
./app_1 add_goal 0 0 "Run_100km_this_month" "2024-01-31" 0 0 100

# View overall statistics
./app_2 view_statistics

# View progress on goal 0 with ASCII visualization
./app_2 view_progres 0

# Backup all data
./app_2 backup my_backup.txt
```

## Features

### Application 1 Features:
- ✅ Interactive activity input with validation
- ✅ Activity type selection (Running, Walking, Swimming, Cardio, Strength)
- ✅ Goal creation and modification
- ✅ CSV data persistence
- ✅ Input validation and error handling

### Application 2 Features:
- ✅ Comprehensive statistics display
- ✅ Activity filtering and goal comparison
- ✅ ASCII progress bar visualization
- ✅ Goal deletion functionality
- ✅ Data backup with formatted output

Both applications share the same CSV data files and work together to provide a complete sports tracking solution. 