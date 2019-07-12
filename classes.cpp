//
// Created by ben on 6/27/19.
//

#include "header.h"

class Process
{
public:
    int proc_num, AT, TC, CB, IO, static_prio, FT=0, TT=0, IT=0, CW=0, ready_start_time=0, total_cb=0, state_ts = 0;
    int static_TC, current_cb, dynamic_prio;
    bool expired;
    trans_to prev_state = CREATED;
    int remaining_time;     //for SRTF (used TC instead -> it's updated throughout the program)
    Process(int proc_num, int at, int tc, int cb, int io, int st_prio) {
        this->proc_num = proc_num;
        this->AT = at;
        TC = tc;
        CB = cb;
        IO = io;
        static_prio = st_prio;
        dynamic_prio = static_prio-1;
        static_TC = TC;
        remaining_time = TC;
        state_ts = AT;       //the start time at a specific state
        current_cb = 0;
        expired = false;
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
    trans_to state, prev_state;


    Event(int ts, Process *proc, trans_to state1)
    {
        time_stamp = ts;
        process = proc;
        state = state1;
        this->prev_state = prev_state;
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
    virtual Process* get_next_process(list <Process *> &ready_queue) //gets from ready_q
    {
        if(ready_queue.empty())
            return nullptr;
        Process *proc = ready_queue.front();
        ready_queue.pop_front();
        return proc;
    }
    virtual void add_process(Process *proc, list <Process *> &ready_queue) = 0; //adds to ready_q (different for each algo)
};


class FCFS : public Scheduler
{
public:
    void add_process(Process *proc, list <Process *> &ready_queue)
    {
        ready_queue.push_back(proc);
    }
};


class LCFS : public Scheduler
{
public:
    void add_process(Process *proc, list <Process *> &ready_queue)
    {
        ready_queue.push_front(proc);
    }
};


class SRTF : public Scheduler
{
public:
    void add_process(Process *proc, list <Process *> &ready_queue)
    {
        bool is_inserted = false;
        list<Process *>::iterator iter;
        for(iter=ready_queue.begin(); iter!=ready_queue.end(); ++iter)
        {
            if(proc->TC >= (*iter)->TC)
                continue;
            else {
                ready_queue.insert(iter, proc);
                is_inserted = true;
                break;
            }
        }
        if(!is_inserted)
            ready_queue.insert(ready_queue.end(),proc);
    }
};


class RoundRobin : public Scheduler
{
public:
    void add_process(Process *process, list<Process *> &ready_queue)
    {
        ready_queue.push_back(process);
    }
};


class Prio : public Scheduler
{
public:

//    list<Process *> active_ready;
    list<Process *> expired_ready;

    void add_process(Process *process, list<Process *> &active_ready)
    {
//        printf("prev state = <%d>\n", process->prev_state);
//        ready_queue.push_back(process);
        // SORT BY PRIO

        if(process->dynamic_prio == -1) {
            process->dynamic_prio = process->static_prio-1;
            list<Process *>::iterator iter;
            for(iter = expired_ready.begin(); iter!=expired_ready.end(); ++iter) {
                if(process->dynamic_prio > (*iter)->dynamic_prio) {
                    expired_ready.insert(iter, process);
                    return;
                }
            }
            expired_ready.insert(iter, process);
        }
        else {
            list<Process *>::iterator iter;
            for(iter = active_ready.begin(); iter!=active_ready.end(); ++iter) {
                if(process->dynamic_prio > (*iter)->dynamic_prio) {
                    active_ready.insert(iter, process);
                    return;
                }
            }
            active_ready.insert(iter, process);
        }
    }

    Process* get_next_process(list <Process *> &active_ready) {
        if(active_ready.empty()) {
            active_ready.swap(expired_ready);
        }
        if (active_ready.empty()) {
            return nullptr;
        }
        Process *proc = active_ready.front();
        active_ready.pop_front();
        return proc;
    }
};


class PrePrio : public Scheduler
{
    list<Process *> expired_ready;

    void add_process(Process *process, list<Process *> &active_ready)
    {
//        printf("prev state = <%d>\n", process->prev_state);
//        ready_queue.push_back(process);
        // SORT BY PRIO

        if(process->dynamic_prio == -1) {
            process->dynamic_prio = process->static_prio-1;
            list<Process *>::iterator iter;
            for(iter = expired_ready.begin(); iter!=expired_ready.end(); ++iter) {
                if(process->dynamic_prio > (*iter)->dynamic_prio) {
                    expired_ready.insert(iter, process);
                    return;
                }
            }
            expired_ready.insert(iter, process);
        }
        else {
            list<Process *>::iterator iter;
            for(iter = active_ready.begin(); iter!=active_ready.end(); ++iter) {
                if(process->dynamic_prio > (*iter)->dynamic_prio) {
                    active_ready.insert(iter, process);
                    return;
                }
            }
            active_ready.insert(iter, process);
        }
    }

    Process* get_next_process(list <Process *> &active_ready) {
        if(active_ready.empty()) {
            active_ready.swap(expired_ready);
        }
        if (active_ready.empty()) {
            return nullptr;
        }
        Process *proc = active_ready.front();
        active_ready.pop_front();
        return proc;
    }
};