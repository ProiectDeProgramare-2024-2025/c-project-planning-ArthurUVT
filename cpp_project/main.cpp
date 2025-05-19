#include "tracker.h"
#include <iostream>

int main()
{
    // Define the data file name
    const std::string dataFilename = "activities_cpp.csv";

    try
    {
        Tracker trackerApp(dataFilename);
        trackerApp.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return 1; // Indicate error
    }
    catch (...)
    {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }

    return 0; // Indicate success
}
