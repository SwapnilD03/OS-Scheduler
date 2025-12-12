#pragma once
#include "../../include/model.h"

SchedulerResult runSPN(const std::vector<Process>& proc, int last_instant, int quantum_unused);
