#include "fb1.h"
#include <queue>
#include <vector>
#include <algorithm>

SchedulerResult runFB1(const std::vector<Process>& proc, int last_instant, int) {
    SchedulerResult res;
    res.last_instant = last_instant;
    res.processes = proc;
    res.timeline.assign(last_instant, -1);

    int n = res.processes.size();
    for(auto &p : res.processes) p.remaining = p.service;

    std::vector<std::queue<int>> q(10);

    std::vector<int> arrivalOrder(n);
    for(int i=0;i<n;i++) arrivalOrder[i] = i;
    std::sort(arrivalOrder.begin(), arrivalOrder.end(), [&](int a, int b){
        return res.processes[a].arrival < res.processes[b].arrival;
    });

    int nextIdx = 0;

    for(int t=0;t<last_instant;t++){
        while(nextIdx<n && res.processes[arrivalOrder[nextIdx]].arrival<=t){
            q[0].push(arrivalOrder[nextIdx]);
            nextIdx++;
        }

        int level = -1;
        for(int i=0;i<10;i++){
            if(!q[i].empty()){ level = i; break; }
        }
        if(level==-1) continue;

        int id = q[level].front(); q[level].pop();

        res.timeline[t] = id;
        res.processes[id].remaining--;

        if(res.processes[id].remaining == 0)
            res.processes[id].finish_time = t+1;
        else
            q[std::min(level+1,9)].push(id);
    }

    return res;
}
