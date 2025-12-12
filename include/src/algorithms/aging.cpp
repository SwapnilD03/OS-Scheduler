#include "aging.h"
#include <vector>
#include <algorithm>

SchedulerResult runAging(const std::vector<Process>& procs, int last_instant, int baseQuantum) {
    if(baseQuantum <= 0) baseQuantum = 1;

    SchedulerResult res;
    res.last_instant = last_instant;
    res.processes = procs;
    res.timeline.assign(last_instant, -1);
    int n = res.processes.size();

    struct PState { int id, priority, waiting; };
    std::vector<PState> ready;

    for(auto &p: res.processes){
        p.remaining = p.service;
        p.priority = 0;
    }

    std::vector<int> order(n);
    for(int i=0;i<n;i++) order[i]=i;
    std::sort(order.begin(), order.end(), [&](int a, int b){
        return res.processes[a].arrival < res.processes[b].arrival;
    });

    int nextIdx=0;
    int current = -1;

    for(int t=0;t<last_instant;t++){
        while(nextIdx<n && res.processes[order[nextIdx]].arrival <= t){
            ready.push_back({order[nextIdx], 0, 0});
            nextIdx++;
        }

        for(auto &ps : ready){
            if(ps.id == current){ ps.waiting = 0; ps.priority = 0; }
            else { ps.waiting++; ps.priority++; }
        }

        if(ready.empty()) continue;

        std::sort(ready.begin(), ready.end(), [&](auto &a, auto &b){
            return a.priority > b.priority;
        });

        current = ready[0].id;

        int quantum = baseQuantum;
        while(quantum-- > 0 && t < last_instant){
            res.timeline[t] = current;
            res.processes[current].remaining--;
            if(res.processes[current].remaining == 0){
                res.processes[current].finish_time = t+1;
                ready.erase(ready.begin());
                break;
            }
            t++;
        }
        t--;
    }

    return res;
}
