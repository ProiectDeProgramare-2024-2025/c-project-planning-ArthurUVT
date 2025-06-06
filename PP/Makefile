CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

all: app_1 app_2

app_1: app_1.o CoreTracker.o activity.o Color.o
	$(CXX) $(CXXFLAGS) -o app_1 app_1.o CoreTracker.o activity.o Color.o

app_2: app_2.o AdvancedTracker.o activity.o Color.o
	$(CXX) $(CXXFLAGS) -o app_2 app_2.o AdvancedTracker.o activity.o Color.o

app_1.o: app_1.cpp CoreTracker.h activity.h Color.h
	$(CXX) $(CXXFLAGS) -c app_1.cpp

app_2.o: app_2.cpp AdvancedTracker.h activity.h
	$(CXX) $(CXXFLAGS) -c app_2.cpp

CoreTracker.o: CoreTracker.cpp CoreTracker.h activity.h Color.h
	$(CXX) $(CXXFLAGS) -c CoreTracker.cpp

AdvancedTracker.o: AdvancedTracker.cpp AdvancedTracker.h activity.h
	$(CXX) $(CXXFLAGS) -c AdvancedTracker.cpp

activity.o: activity.cpp activity.h
	$(CXX) $(CXXFLAGS) -c activity.cpp

Color.o: Color.cpp Color.h
	$(CXX) $(CXXFLAGS) -c Color.cpp

clean:
	rm -f app_1 app_2 *.o

.PHONY: all clean 