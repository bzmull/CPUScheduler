//
// Created by ben on 6/27/19.
//

#include "header.h"

class Process
{
private:
    int AT, TC, CB, IO;

public:
    Process(int at, int tc, int cb, int io) {
        AT = at;
        TC = tc;
        CB = cb;
        IO = io;
    }
    int get_AT(){return AT;}
    int get_TC(){return TC;}
    int get_CB(){return CB;}
    int get_IO(){return IO;}
};

class Event
{
public:
    int time_stamp;
    Process *process;
    trans_to state;

    Event(int ts, Process *proc, trans_to state1)
    {
        time_stamp = ts;
        process = proc;
        state = state1;
    }
//    int get_ts(){return time_stamp;}
//    Process get_proc(){ return *process;}
//    trans_to get_state(){ return state;}
};

class Scheduler
{
    queue <Process *> event_queue;

public:
    Scheduler(queue <Process *> event_q)
    {
        event_queue = event_q;
    }
    Process* getNextProcess()
    {
        Process *proc = event_queue.front();
        event_queue.pop();
        return proc;
    }
    void addProcess(Process *proc)
    {
        event_queue.push(proc);
    }
};

class FCFS : Scheduler
{

};

class LCFS : Scheduler
{

};

class SRTF : Scheduler
{

};

class RoundRobin : Scheduler
{

};

class Prio : Scheduler
{

};

class PrePrio : Scheduler
{

};