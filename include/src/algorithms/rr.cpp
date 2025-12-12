#include "rr.h"
#include <queue>
#include <vector>

SchedulerResult runRR(const std::vector<Process>& proc, int last_instant, int quantum) {
    SchedulerResult res;
    res.last_instant = last_instant;
    res.processes = proc;
    res.timeline.assign(last_instant, -1);

    if (quantum <= 0) quantum = 1; // fallback

    int n = (int)res.processes.size();
    for (int i = 0; i < n; ++i) res.processes[i].remaining = res.processes[i].service;

    std::queue<int> q;
    int next_arrival_index = 0;

    // prepare arrival order
    std::vector<int> order(n);
    for(int i=0;i<n;i++){
        order[i]=i;
    } 
    
    std::sort(order.begin(), order.end(), [&](int a, int b){
        if(res.processes[a].arrival != res.processes[b].arrival) return res.processes[a].arrival < res.processes[b].arrival;
        return a < b;
    });

    // push any processes arriving at t=0
    while(next_arrival_index < n && res.processes[order[next_arrival_index]].arrival == 0) {
        q.push(order[next_arrival_index++]);
    }

    for(int t=0;t<last_instant;t++) {
        // push arrivals at time t
        while(next_arrival_index < n && res.processes[order[next_arrival_index]].arrival <= t) {
            q.push(order[next_arrival_index++]);
        }
        if(q.empty()) continue;
        int pid = q.front(); q.pop();
        int slice = std::min(quantum, res.processes[pid].remaining);
        // execute slice (but stop at last_instant)
        for(int step=0; step<slice && t <last_instant; step++,t++) {
            res.timeline[t] = pid;
            res.processes[pid].remaining--;
            // push any new arrivals that happen at this time (t)
            while(next_arrival_index < n && res.processes[order[next_arrival_index]].arrival<= t) {
                q.push(order[next_arrival_index++]);
            }
        }
        // we advanced t in inner loop; outer loop will increment t again, so decrement here
        --t;
        if(res.processes[pid].remaining > 0) q.push(pid);
        else res.processes[pid].finish_time = std::min(last_instant,t+1);
    }

    return res;
}
