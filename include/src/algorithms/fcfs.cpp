#include "fcfs.h"
#include <vector>
#include <algorithm>

SchedulerResult runFCFS(const std::vector<Process>& proc, int last_instant, int /*quantum_unused*/) {
    SchedulerResult res;
    res.last_instant = last_instant;
    res.processes = proc;
    res.timeline.assign(last_instant, -1);

    // Sort processes by arrival time but keep original index mapping
    std::vector<int> idx(res.processes.size());
    for(size_t i=0;i<idx.size();i++) 
            idx[i] = (int)i;
            
    std::sort(idx.begin(), idx.end(), [&](int a, int b){
        if(res.processes[a].arrival != res.processes[b].arrival)
            return res.processes[a].arrival < res.processes[b].arrival;
        return a < b;
    });

    int time = 0;
    for(int id : idx) {
        const Process& p = res.processes[id];
        if(time < p.arrival) time = p.arrival;
        // run from time to time+service-1
        for(int t=time; t<time + p.service && t<last_instant;t++){
            res.timeline[t] = id;
        }
        res.processes[id].finish_time = time + p.service;
        res.processes[id].remaining = 0;
        time += p.service;
    }
    return res;
}
