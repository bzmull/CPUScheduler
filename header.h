//
// Created by ben on 6/27/19.
//

#ifndef CPUSCHEDULER_HEADER_H
#define CPUSCHEDULER_HEADER_H

#endif //CPUSCHEDULER_HEADER_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <queue>
#include <list>
#include <getopt.h>

using namespace std;


enum trans_to {CREATED, TRANS_TO_READY, TRANS_TO_RUNNING, TRANS_TO_BLOCKED, TRANS_TO_PREEMPT};
enum sched_type {F, L, S, R, P, E};


