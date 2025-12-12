#pragma once
#include <string>
#include <vector>

struct Process {
    std::string name;
    int arrival = 0;
    int service = 0;
    int priority = 0;

    // runtime fields
    int remaining = 0;
    int finish_time = -1;
};

struct SchedulerResult {
    std::string algorithm_name;
    int last_instant = 0;
    std::vector<Process> processes; 
    
    std::vector<int> timeline;
};
