#pragma once
#include "model.h"
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <iostream>

struct Config {
    std::string operation; // "trace" or "stats"
    int last_instant = 0;
    std::vector<std::pair<char,int>> algorithms; 
    std::vector<Process> processes;
};

// Parse same input format you had: operation algorithms last_instant process_count
// Then process_count tokens
inline Config parse_input_from_stdin() {
    Config cfg;
    std::string algorithm_chunk;
    int process_count;

    if(!(std::cin >> cfg.operation >> algorithm_chunk >> cfg.last_instant >> process_count)) {
        throw std::runtime_error("Invalid input format");
    }

    // parse algorithms chunk
    std::stringstream stream(algorithm_chunk);
    while (stream.good()) {
        std::string temp;
        std::getline(stream, temp, ',');
        if(temp.empty()) continue;
        std::stringstream ss(temp);
        std::string idpart;
        std::getline(ss, idpart, '-');
        char id = idpart[0];
        int quantum = -1;
        std::string qpart;
        if(std::getline(ss, qpart, '-')) {
            if(!qpart.empty()) quantum = std::stoi(qpart);
        }
        cfg.algorithms.push_back({id, quantum});
    }

    // parse processes
    for (int i = 0; i < process_count; ++i) {
        std::string token;
        std::cin >> token; 
        std::stringstream stream2(token);
        std::string name, a_str, s_str;
        std::getline(stream2, name, ',');
        std::getline(stream2, a_str, ',');
        std::getline(stream2, s_str, ',');
        Process p;
        p.name = name;
        p.arrival = std::stoi(a_str);
        p.service = std::stoi(s_str);
        p.remaining = p.service;
        cfg.processes.push_back(p);
    }
    return cfg;
}
