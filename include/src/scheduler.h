#pragma once
#include "../include/model.h"
#include <functional>
#include <string>
#include <unordered_map>

class Scheduler {
public:
    using AlgFn = std::function<SchedulerResult(const std::vector<Process>&, int last_instant, int quantum)>;

    Scheduler(const std::vector<Process>& processes, int last_instant);

    // register algorithm with key like "1" or "FCFS"
    void registerAlgorithm(const std::string& key, const AlgFn& fn, const std::string& prettyName);

    // run algorithm by key, with quantum (time quantum is ignored by algorithms that don't need it)
    SchedulerResult runAlgorithm(const std::string& key, int quantum = -1) const;

private:
    std::vector<Process> base_processes_;
    int last_instant_;
    struct AlgEntry { AlgFn fn; std::string prettyName; };
    std::unordered_map<std::string, AlgEntry> registry_;
};
