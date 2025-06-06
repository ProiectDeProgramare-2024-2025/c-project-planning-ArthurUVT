# Project Overview: Sports Activity Tracking System

## 1. Introduction

The Sports Activity Tracking System is a C++ application designed to help users log, manage, and analyze their physical activities. It provides a robust command-line interface for interacting with the system, allowing for efficient data entry and retrieval.

## 2. Core Purpose

*   **Record Keeping:** Allow users to meticulously record various types of sports activities (e.g., running, swimming, cycling, strength training) with relevant details like date, duration, distance, and calories burned.
*   **Data Management:** Provide functionalities to add, view, update, and delete activity records.
*   **Progress Tracking & Analysis:** Offer tools to view activity history, generate statistics, filter data based on various criteria, and visualize progress.
*   **Goal Setting:** Enable users to set personal fitness goals and track their progress towards achieving them.

## 3. System Architecture

The system is designed as a two-application solution to separate concerns and streamline functionality:

*   **`app_1` (Activity Management Application):**
    *   Handles the core operations of adding, viewing, updating, and deleting activity records.
    *   Manages goal setting and updates.
    *   Ensures data integrity and persistence through CSV file storage (`activities.csv`, `goals.csv`).

*   **`app_2` (Data Analysis & Visualization Application):**
    *   Focuses on providing insights from the stored activity data.
    *   Offers features like statistical summaries, activity filtering (by type, date range, etc.), and basic data visualization (e.g., ASCII charts).
    *   Reads data from the CSV files maintained by `app_1`.

## 4. Key Technologies

*   **Programming Language:** C++ (leveraging modern C++ features for efficiency and robustness)
*   **Data Storage:** CSV files for simplicity and interoperability.
*   **Interface:** Command-Line Interface (CLI) for both applications, ensuring cross-platform compatibility and scriptability.

## 5. Next Steps in Presentation

*   Detailed breakdown of `app_1` functionalities.
*   Detailed breakdown of `app_2` functionalities. 