#include "spn.h"
#include <vector>
#include <algorithm>

SchedulerResult runSPN(const std::vector<Process>& proc, int last_instant, int) {
    SchedulerResult res;
    res.last_instant = last_instant;
    res.processes = proc;
    res.timeline.assign(last_instant, -1);

    int n = res.processes.size();
    for(auto &p: res.processes) p.remaining = p.service;

    std::vector<int> arrivalOrder(n);
    for(int i=0;i<n;++i){
        arrivalOrder[i] = i;
    } 
    std::sort(arrivalOrder.begin(), arrivalOrder.end(), [&](int a, int b){
        return res.processes[a].arrival < res.processes[b].arrival;
    });

    int t = 0,nextIdx = 0;

    while(true){
        std::vector<int> ready;
        while(nextIdx < n && res.processes[arrivalOrder[nextIdx]].arrival <= t) {
            ready.push_back(arrivalOrder[nextIdx]);
            nextIdx++;
        }

        if(ready.empty()) {
            if(nextIdx >= n) break;
            t = res.processes[arrivalOrder[nextIdx]].arrival;
            continue;
        }

        int best = ready[0];
        for(int id : ready) {
            if(res.processes[id].service < res.processes[best].service)
                best = id;
        }

        Process &p = res.processes[best];
        for(int k=0;k<p.service && t<last_instant;k++,t++){
            res.timeline[t] = best;
        }

        p.finish_time = t;
        p.remaining = 0;
    }

    return res;
}
