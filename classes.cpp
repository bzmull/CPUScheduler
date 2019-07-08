//
// Created by ben on 6/27/19.
//

#include "header.h"

class Process
{
public:
    int proc_num, AT, TC, CB, IO, static_prio;
    int state_ts = 0;
    int remaining_time;     //for SRTF
    Process(int proc_num, int at, int tc, int cb, int io, int st_prio) {
        this->proc_num = proc_num;
        this->AT = at;
        TC = tc;
        CB = cb;
        IO = io;
        static_prio = st_prio;
        remaining_time = TC;
        state_ts = AT;       //the start time at a specific state

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
public:
//    list <Process *> ready_queue;

//    Scheduler(list <Process *> ready_queue)
//    {
//        this->ready_queue = ready_queue;
//    }
    Process* get_next_process(list <Process *> &ready_queue)
    {
        Process *proc = ready_queue.front();
        ready_queue.pop_front();
        return proc;
    }
    void add_process(Process *proc, list <Process *> &ready_queue){}   //functions to add and get process from RUN_QUEUE
};


class FCFS : public Scheduler
{
public:
    void add_process(Process *proc, list <Process *> &ready_queue)
    {
        ready_queue.push_front(proc);
    }
};


class LCFS : Scheduler
{
public:
    void add_process()      //functions to add and get process from RUN_QUEUE
    {

    }
};


class SRTF : Scheduler
{
public:
    void add_process()      //functions to add and get process from RUN_QUEUE
    {

    }
};


class RoundRobin : Scheduler
{
public:
    void add_process()      //functions to add and get process from RUN_QUEUE
    {

    }
};


class Prio : Scheduler
{
public:
    void add_process()      //functions to add and get process from RUN_QUEUE
    {

    }
};


class PrePrio : Scheduler
{
public:
    void add_process()      //functions to add and get process from RUN_QUEUE
    {

    }
};