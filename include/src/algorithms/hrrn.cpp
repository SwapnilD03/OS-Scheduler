#include "hrrn.h"
#include <vector>
#include <algorithm>

SchedulerResult runHRRN(const std::vector<Process>& proc, int last_instant, int) {
    SchedulerResult res;
    res.last_instant = last_instant;
    res.processes = proc;
    res.timeline.assign(last_instant, -1);

    int n = res.processes.size();
    for(auto &p : res.processes) p.remaining = p.service;

    std::vector<int> order(n);
    for(int i=0;i<n;i++) {
        order[i] = i;
    }
    
    std::sort(order.begin(), order.end(), [&](int a, int b){
        return res.processes[a].arrival < res.processes[b].arrival;
    });

    int t = 0, nextIdx = 0;

    while(true){
        std::vector<int> ready;
        while(nextIdx < n && res.processes[order[nextIdx]].arrival <= t) {
            ready.push_back(order[nextIdx]);
            nextIdx++;
        }

        if(ready.empty()) {
            if(nextIdx >= n) break;
            t = res.processes[order[nextIdx]].arrival;
            continue;
        }

        double bestRatio = -1;
        int best = ready[0];

        for(int id : ready){
            Process &p = res.processes[id];
            int waiting = t - p.arrival;
            double rr = (waiting + p.service) / (double) p.service;
            if(rr > bestRatio){
                bestRatio = rr;
                best = id;
            }
        }

        Process &p = res.processes[best];
        for(int k=0;k<p.service && t<last_instant;k++,t++){
            res.timeline[t] = best;
        }

        p.remaining = 0;
        p.finish_time = t;
    }

    return res;
}
