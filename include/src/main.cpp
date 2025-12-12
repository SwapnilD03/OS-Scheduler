#include "../include/model.h"
#include "../include/parser.h"
#include "scheduler.h"

// Algorithms
#include "algorithms/fcfs.h"
#include "algorithms/rr.h"
#include "algorithms/spn.h"
#include "algorithms/srt.h"
#include "algorithms/hrrn.h"
#include "algorithms/fb1.h"
#include "algorithms/fb2i.h"
#include "algorithms/aging.h"

#include <iostream>
#include <iomanip>
#include <string>

// -------------------- PRINT HELPERS --------------------

static void printTimeline(const SchedulerResult& r) {
    std::cout << "\nAlgorithm: " << r.algorithm_name << "\n";
    std::cout << "Timeline: ('.' = idle)\n";

    for (int t = 0; t < r.last_instant; ++t) {
        int pid = r.timeline[t];
        if(pid >= 0) std::cout << r.processes[pid].name;
        else std::cout << ".";
    }
    std::cout << "\n\n";
}

static void printStats(const SchedulerResult& r) {
    std::cout << "\nAlgorithm: " << r.algorithm_name << "\n";
    std::cout << "---------------------------------------------------------------\n";
    std::cout << "Process | Arr | Serv | Finish | Turnaround | NormTurn\n";
    std::cout << "---------------------------------------------------------------\n";

    double sumTurn = 0, sumNorm = 0;
    int n = r.processes.size();

    for (const auto &p : r.processes) {
        int finish = p.finish_time;
        int turn = (finish >= 0 ? finish - p.arrival : -1);
        double norm = (p.service > 0 && turn >= 0) ? (double)turn / p.service : 0.0;

        sumTurn += (turn >= 0 ? turn : 0);
        sumNorm += norm;

        std::cout << std::setw(7) << p.name << " | "
                  << std::setw(3) << p.arrival << " | "
                  << std::setw(4) << p.service << " | "
                  << std::setw(6) << finish << " | "
                  << std::setw(10) << turn << " | "
                  << std::setw(8) << std::fixed << std::setprecision(2) << norm
                  << "\n";
    }

    if (n > 0) {
        std::cout << "---------------------------------------------------------------\n";
        std::cout << "Avg Turnaround = " << (sumTurn / n)
                  << ",  Avg NormTurn = " << (sumNorm / n) << "\n\n";
    }
}

// ----------------------- MAIN ----------------------------

int main() {
    try {
        // Parse input
        Config cfg = parse_input_from_stdin();

        // Construct scheduler
        Scheduler sched(cfg.processes, cfg.last_instant);

        // Register all algorithms (IDs: 1–8)
        sched.registerAlgorithm("1", runFCFS,  "FCFS");
        sched.registerAlgorithm("2", runRR,    "RR");
        sched.registerAlgorithm("3", runSPN,   "SPN");
        sched.registerAlgorithm("4", runSRT,   "SRT");
        sched.registerAlgorithm("5", runHRRN,  "HRRN");
        sched.registerAlgorithm("6", runFB1,   "FB-1");
        sched.registerAlgorithm("7", runFB2i,  "FB-2i");
        sched.registerAlgorithm("8", runAging, "Aging");

        // Run each algorithm requested in the input
        for (auto &alg : cfg.algorithms) {
            char id = alg.first;       
            int quantum = alg.second;  
            std::string key(1, id);

            SchedulerResult res = sched.runAlgorithm(key, quantum);

            if (cfg.operation == "trace")
                printTimeline(res);
            else
                printStats(res);

            std::cout << "===============================================================\n";
        }
    }
    catch (const std::exception &ex) {
        std::cerr << "ERROR: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
