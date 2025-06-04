# Sports Activity Tracking System (C++)

A comprehensive command-line based application for tracking various sports activities with detailed analytics, goal setting, and progress visualization.

## Project Structure

The system consists of two separate applications:

1. **app_1** - Core application for managing activities and goals:
   - Adding, modifying, and viewing activities
   - Setting and managing goals
   - Handling data persistence

2. **app_2** - User interaction features:
   - Statistics generation and filtering
   - ASCII-based visual progress charts
   - Goal progress tracking
   - Data backup functionality

## Features

- **Multiple Activity Types**: Track running, walking, swimming, cardio, and strength training.
- **Data Persistence**: Automatically saves and loads activities to/from CSV files.
- **Detailed Statistics**: View averages and totals for each activity type.
- **Data Visualization**: ASCII-based charts to visualize progress over time.
- **Goal Setting & Tracking**: Set personal fitness goals and monitor progress.
- **Activity Summary Reports**: Get detailed breakdowns by activity type, date, and more.
- **Input Validation**: Robust validation for dates and numeric inputs.

## Setup & Installation

### Prerequisites
- C++ compiler with C++11 support
- CMake (version 3.10 or later)

### Compilation
Build the applications using CMake:

```bash
mkdir build
cd build
cmake ..
make
```

This will create two executables in the `bin` directory: `app_1` and `app_2`.

## Usage Guide

### app_1 Commands

```bash
./app_1 add_activity <activity ID>
```
Add a new activity with the specified activity type ID (0=Running, 1=Walking, 2=Swimming, 3=Cardio, 4=Strength).

```bash
./app_1 view_activity <activity ID>
```
View details of a specific activity by ID.

```bash
./app_1 view_activities
```
View all recorded activities.

```bash
./app_1 add_goal <goal ID> <activity ID> <description> <deadline> <target repetitions> <target duration> <target distance>
```
Add a new goal with specified parameters.

```bash
./app_1 view_goal <goal ID>
```
View details of a specific goal by ID.

```bash
./app_1 view_goals
```
View all recorded goals.

```bash
./app_1 modify_goal <goal ID> <activity ID> <description> <deadline> <target repetitions> <target duration> <target distance>
```
Modify an existing goal.

### app_2 Commands

```bash
./app_2 view_statistics
```
View general statistics about all activities.

```bash
./app_2 filter_statistics <activity ID> <goal ID>
```
View filtered statistics for a specific activity and goal.

```bash
./app_2 view_progress <goal ID>
```
View detailed progress for a specific goal, including ASCII visualization.

```bash
./app_2 delete_goal <goal ID>
```
Delete a goal by ID.

```bash
./app_2 backup <file path>
```
Backup activity and goal data to the specified file path.

## Data Storage

Activities and goals are stored in CSV files:
- Activities: `activities_cpp.csv`
- Goals: `activities_goals_cpp.csv`

These files are automatically loaded when the programs start and saved when necessary.

## Technical Details

### Activity Types
- Running (ID: 0)
- Walking (ID: 1)
- Swimming (ID: 2)
- Cardio (ID: 3)
- Strength (ID: 4)

### Data Structure
Each activity stores:
- Activity type
- Date (YYYY-MM-DD)
- Duration (minutes)
- Distance (km) for applicable activities
- Repetitions for strength training

Each goal stores:
- Activity type
- Description
- Deadline (YYYY-MM-DD)
- Target repetitions
- Target duration
- Target distance
- Achievement status

### Implementation Details
- Written in C++ with C++11 features
- Uses STL containers (vector, map)
- File I/O for data persistence
- Comprehensive input validation
- Command-line interface

## License

MIT License

Copyright (c) 2023

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 