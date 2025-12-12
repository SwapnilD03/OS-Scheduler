#include "srt.h"
#include <queue>
#include <algorithm>

SchedulerResult runSRT(const std::vector<Process>& proc, int last_instant, int) {
    SchedulerResult res;
    res.last_instant = last_instant;
    res.processes = proc;
    res.timeline.assign(last_instant, -1);
    int n = res.processes.size();

    for(int i=0;i<n;i++) res.processes[i].remaining = res.processes[i].service;

    using PII = std::pair<int,int>;
    auto cmp = [&](const PII&a, const PII&b){
        return a.first > b.first;
    };
    
    std::priority_queue<PII, std::vector<PII>, decltype(cmp)> pq(cmp);

    std::vector<int> arrivalOrder(n);
    for(int i=0;i<n;i++) arrivalOrder[i] = i;
    std::sort(arrivalOrder.begin(), arrivalOrder.end(), [&](int a, int b){
        return res.processes[a].arrival < res.processes[b].arrival;
    });

    int nextIdx = 0;

    for(int t=0;t<last_instant;t++){
        while(nextIdx < n && res.processes[arrivalOrder[nextIdx]].arrival <= t){
            int id = arrivalOrder[nextIdx++];
            pq.push({res.processes[id].remaining, id});
        }

        if(pq.empty()) continue;

        auto [rem, id] = pq.top(); pq.pop();
        res.timeline[t] = id;
        res.processes[id].remaining--;

        if(res.processes[id].remaining == 0)
            res.processes[id].finish_time = t+1;
        else
            pq.push({res.processes[id].remaining, id});
    }

    return res;
}
