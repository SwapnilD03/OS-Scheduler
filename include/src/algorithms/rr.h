#pragma once
#include "../../include/model.h"

SchedulerResult runRR(const std::vector<Process>& proc, int last_instant, int quantum);
