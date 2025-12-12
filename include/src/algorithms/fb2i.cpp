#include "fb2i.h"
#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>

SchedulerResult runFB2i(const std::vector<Process>& proc, int last_instant, int) {
    SchedulerResult res;
    res.last_instant = last_instant;
    res.processes = proc;
    res.timeline.assign(last_instant, -1);

    int n = res.processes.size();
    for(auto &p : res.processes) p.remaining = p.service;

    std::vector<std::queue<int>> q(10);

    std::vector<int> order(n);
    for(int i=0;i<n;i++) order[i]=i;
    std::sort(order.begin(), order.end(), [&](int a, int b){
        return res.processes[a].arrival < res.processes[b].arrival;
    });

    int nextIdx=0;

    for(int t=0;t<last_instant;t++){
        while(nextIdx<n && res.processes[order[nextIdx]].arrival<=t){
            q[0].push(order[nextIdx]);
            nextIdx++;
        }

        int level=-1;
        for(int i=0;i<10;i++) if(!q[i].empty()){ level=i;break; }
        if(level==-1) continue;

        int id = q[level].front(); q[level].pop();
        int quantum = std::pow(2, level);

        int step=0;
        while(step < quantum && res.processes[id].remaining>0 && t < last_instant){
            res.timeline[t] = id;
            res.processes[id].remaining--;
            step++;
            t++;

            while(nextIdx<n && res.processes[order[nextIdx]].arrival<=t){
                q[0].push(order[nextIdx]);
                nextIdx++;
            }
        }
        t--;

        if(res.processes[id].remaining == 0)
            res.processes[id].finish_time = t+1;
        else
            q[std::min(level+1,9)].push(id);
    }

    return res;
}
