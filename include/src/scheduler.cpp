#include "scheduler.h"
#include <stdexcept>

Scheduler::Scheduler(const std::vector<Process>& processes, int last_instant)
    : base_processes_(processes), last_instant_(last_instant) { }

void Scheduler::registerAlgorithm(const std::string& key, const AlgFn& fn, const std::string& prettyName) {
    registry_[key] = AlgEntry{fn, prettyName};
}

SchedulerResult Scheduler::runAlgorithm(const std::string& key, int quantum) const {
    auto it = registry_.find(key);
    if(it == registry_.end()) {
        throw std::runtime_error("Algorithm not registered: " + key);
    }
    SchedulerResult res = it->second.fn(base_processes_, last_instant_, quantum);
    res.algorithm_name = it->second.prettyName;
    return res;
}
