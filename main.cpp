//#include "header.h"
#include "classes.cpp"


list<Process*> read_file();
void put_event(int time_stamp, Process *process, trans_to state, list<Event *> &event_list);
Event *get_event(list<Event *> &event_list);
void simulation(list<Event *> &event_list, list<Process *> &ready_queue, int ofs, vector<int> rand_vals);
int get_next_event_time(list<Event *> event_list);
int get_random_number(int burst, int &ofs, vector<int> rand_vals);

//bool debug = true;
bool debug = false;

int main() {
    int ofs = 1;
    int rand;
    vector<int> rand_vals;
    string rand_ints_file_path = "./lab2_assign/rfile";
    FILE *ptr1 = fopen(rand_ints_file_path.c_str(), "r");

    while(fscanf(ptr1, "%d", &rand) > 0)
        rand_vals.push_back(rand);


    int max_prio = 4;   //by default for FCFS and co.
    int static_prio = 0;

    int AT, TC, CB, IO;
    int proc_num = 0;
    list<Process *> ready_queue;
    list<Event *> event_list;
    list<Event *> finished_processes;
    string file_path = "./lab2_assign/input2";
    FILE *ptr2 = fopen(file_path.c_str(), "r");

    while (fscanf(ptr2, "%d %d %d %d", &AT, &TC, &CB, &IO) > 0) {
        static_prio = get_random_number(max_prio, ofs, rand_vals);
        Process *process = new Process(proc_num, AT, TC, CB, IO, static_prio);
        put_event(AT, process, CREATED, event_list);
        proc_num++;
//        ready_queue.push(process);
    }
//    list<Event *>::iterator it = event_list.begin();
//    printf("QSize = <%zu>\n", event_list.size());
//    int i = 1;
//    while (it != event_list.end()) {
//        Event *evt = *it;
//        printf("proc %d = <%d, %d>\n", i + 1, evt->time_stamp, evt->process->get_CB());
//        i++;
//    }
//    for(list<Event*>::iterator iter = event_list.begin(); iter!=event_list.end(); ++iter)
//    {
//        Event *evt = *iter;
//        printf("proc = <%d, %d>\n", evt->time_stamp, evt->process->get_CB());
//    }
//
//    for(int i=0; i<10; i++)
//    {
//        int num = get_random_number(10, ofs, rand_vals);
//        printf("rand %d = <%d>\n", i+1, num);
//    }

    simulation(event_list, ready_queue, ofs, rand_vals);
}



//list<Process*> read_file()
//{
//    int AT, TC, CB, IO;
//    list<Process *> ready_queue;
//    list<Event *> event_list;
//    string file_path = "./lab2_assign/input4";
//    FILE *ptr = fopen(file_path.c_str(), "r");
//
//    while (fscanf(ptr, "%d %d %d %d", &AT, &TC, &CB, &IO) > 0) {
//        Process *process = new Process(AT, TC, CB, IO);
//        put_event(AT, process, TRANS_TO_READY, event_list);
//        ready_queue.insert(ready_queue.begin(),process);
//    }
//    list<Event *>::iterator it = event_list.begin();
//    int i = 1;
//    while(it != event_list.end())
//    {
//        Event *evt = *it;
//        printf("proc %d = <%d, %d>\n", i+1, evt->time_stamp, evt->process->get_CB());
//        i++;
//    }
//    return ready_queue;
//}


void print_evt_q(list<Event *> event_list)
{
    if (! debug)
        return;
    printf("Evt queue: ");
    for(list<Event*>::iterator iter = event_list.begin(); iter!=event_list.end(); ++iter)
    {
        printf("evt pid = <%d>, ts = <%d>, ", (*iter)->process->proc_num, (*iter)->time_stamp);
        if((*iter)->state==CREATED)
            printf("state = CREATED");
        else if((*iter)->state==TRANS_TO_READY)
            printf("state = TRANS_TO_READY");
        else if((*iter)->state==TRANS_TO_RUNNING)
            printf("state = TRANS_TO_RUNNING");
        else if((*iter)->state==TRANS_TO_BLOCKED)
            printf("state = TRANS_TO_BLOCKED");
        else if((*iter)->state==TRANS_TO_PREEMPT)
            printf("state = TRANS_TO_PREEMPT");

//        switch ((*iter)->state) {
//            case TRANS_TO_READY:
//                printf("state = TRANS_TO_READY");
//                break;
//            case TRANS_TO_BLOCKED:
//                printf("state = TRANS_TO_BLOCKED");
//                break;
//            case TRANS_TO_RUNNING:
//                printf("state = TRANS_TO_RUNNING");
//                break;
//            case TRANS_TO_PREEMPT:
//                printf("state = TRANS_TO_PREEMPT");
//                break;
//            default:
//                printf("Other");
//                break;

    }
    printf("\n");
}


void print_ready_q(list<Process *> ready_q)
{
    if(!debug)
        return;
    printf("Ready Queue: ");
    for(list<Process*>::iterator iter = ready_q.begin(); iter!=ready_q.end(); ++iter)
    {
        printf("pid = <%d>; ", (*iter)->proc_num);
    }
    printf("\n");
}


void put_event(int time_stamp, Process *process, trans_to state, list<Event *> &event_list) //change to LIST
{
    Event *new_evt = new Event(time_stamp, process, state);
    if(event_list.empty())     //insert event at beginning of Q if empty
    {
        event_list.insert(event_list.begin(), new_evt);
        return;
    }
    bool is_inserted = false;
    list<Event *>::iterator iter1 = event_list.begin();

//    for(int i=0; i<event_list.size(); i++)     //compares new entry with each event in Q
//    while(iter1 != event_list.end())
    for(iter1=event_list.begin(); iter1!=event_list.end(); ++iter1)
    {
//        int ts = event_list[i]->time_stamp;
        int ts = (*iter1)->time_stamp;
        if(ts <= new_evt->time_stamp)
        {
            continue;
        }
        else if(ts > new_evt->time_stamp )
        {
//            printf("its bigger\n");
            event_list.insert(iter1, new_evt);
            is_inserted = true;
            break;
        }
    }
    if(!is_inserted)
        event_list.insert(event_list.end(), new_evt);
}


Event *get_event(list<Event *> &event_list)
{
    if(event_list.empty())
        return nullptr;
    Event *evt = event_list.front();

//    ts = evt->time_stamp;
//    proc  = *evt->process;
//    state = evt->state;

    event_list.erase(event_list.begin());
    return evt;
}


void simulation(list<Event *> &event_list, list<Process *> &ready_queue, int ofs, vector<int> rand_vals)
{
    FCFS *sched = new FCFS();

    list<Event *> finished_processes;
    Process *CURRENT_RUNNING_PROCESS = nullptr;
    Event *evt;
    while((evt = get_event(event_list)))
    {
        Process *proc = evt->process;
        int current_time = evt->time_stamp;
        int time_in_state = current_time - proc->state_ts;

        if (debug) {
            printf("current time=%d, pid=%d, state ts=%d\n", current_time, proc->proc_num, proc->state_ts);
        }
        proc->state_ts = current_time;
        bool call_sched = false;
        int cb, io;
        bool was_blocked;


        if(evt->state == CREATED)
        {
            was_blocked = false;
            printf("%d %d %d: CREATED -> READY\n", evt->time_stamp, proc->proc_num, time_in_state);
            evt->state = TRANS_TO_READY;
        }

        switch(evt->state)
        {
            case TRANS_TO_READY:
                if(was_blocked)
                    printf("%d %d %d: BLOCK -> READY\n", evt->time_stamp, proc->proc_num, time_in_state);
                proc->ready_start_time = current_time;
                sched->add_process(proc, ready_queue); //add to ready_Q --> main functionality of whichever algorithm.
//                printf("ready proc AT = <%d>\n", ready_queue.front()->AT);
                print_ready_q(ready_queue);
                call_sched = true;
                break;
            case TRANS_TO_RUNNING:
                cb = get_random_number(proc->get_CB(), ofs, rand_vals); //create random CB
                if(cb > proc->TC)
                    cb = proc->TC;
//                time_in_state = 0;
                printf("%d %d %d: READY -> RUNNG cb=%d rem=%d prio=%d\n", evt->time_stamp, proc->proc_num,
                       time_in_state, cb, proc->TC, proc->static_prio-1);
                proc->TC = proc->TC - cb;
//                proc->state_ts = current_time;
                proc->CW = current_time - proc->ready_start_time;
                proc->total_cb += cb;
//                current_time = current_time + cb;
//                evt->state = TRANS_TO_BLOCKED; //and change to TRANS_TO_BLOCKED (or PREEMPT if using PREPRIO)
                put_event(current_time+cb, proc, TRANS_TO_BLOCKED, event_list);

//                if(proc->TC > 0)
//                    put_event(current_time+cb, proc, TRANS_TO_BLOCKED, event_list);
//                else {
//                    printf("%d %d %d: Done\n", current_time+cb, proc->proc_num, cb);
//                    proc->FT = current_time + cb;
//                    proc->TT = current_time + cb - proc->AT;
//                    put_event(proc->proc_num, proc, TRANS_TO_READY, finished_processes);
//                }
                break;
            case TRANS_TO_BLOCKED:
                CURRENT_RUNNING_PROCESS = nullptr;
                was_blocked = true;
                call_sched = true;
//                io = get_random_number(proc->get_IO(), ofs, rand_vals); //create random IO
//                printf("%d %d %d: RUNNG -> BLOCK  ib=%d rem=%d\n", evt->time_stamp, proc->proc_num,
//                       time_in_state, io, proc->TC);
//                proc->state_ts = current_time;
//                proc->IT = proc->IT + io;
//                current_time = current_time + io;
//                evt->state = TRANS_TO_READY; //and change to TRANS_TO_READY
//                put_event(current_time+io, proc, TRANS_TO_READY, event_list);

                if(proc->TC==0)
                {
                    printf("%d %d %d: Done\n", current_time, proc->proc_num, cb);
                    proc->FT = current_time;
                    proc->TT = current_time - proc->AT;
                }
                else {
                    io = get_random_number(proc->get_IO(), ofs, rand_vals); //create random IO
                    proc->IT = proc->IT + io;
                    printf("%d %d %d: RUNNG -> BLOCK  ib=%d rem=%d\n", evt->time_stamp, proc->proc_num,
                           time_in_state, io, proc->TC);
                    put_event(current_time+io, proc, TRANS_TO_READY, event_list);
                }

                //and update remaining time (for SRTF)
                break;
            case TRANS_TO_PREEMPT:
                call_sched = true;
                break;
        }
        delete(evt);
        evt = nullptr;

        if(call_sched)
//        printf("current time=<%d>\n", current_time);
        {
            if(get_next_event_time(event_list) == current_time)  //function return ts of event after the one we deleted
            {
                continue; //process next event from Event queue
            }
            call_sched = false; // reset global
            if (CURRENT_RUNNING_PROCESS == nullptr) {   //if nothings running, get next process from ready Q
//                printf("no current proc\n");
                CURRENT_RUNNING_PROCESS = sched->get_next_process(ready_queue); // gets the next proc from ready_Q
                if (CURRENT_RUNNING_PROCESS == nullptr)
                    continue;   // create event to make this process runnable for same time.
                put_event(current_time, CURRENT_RUNNING_PROCESS, TRANS_TO_RUNNING, event_list);
            }
        }

        print_evt_q(event_list);

    }
    //FINAL PRINT

    double total_cpu_util=0, total_io_util=0, total_tt=0, total_cw=0, thru_put=0.0;
    int fin_time=0, counter=0;
    list<Event *>::iterator iter = finished_processes.begin();
    string sched_algo;
    sched_algo = "FCFS";
    cout << sched_algo << endl;
//    printf("%s\n", sched_algo);
    for(iter=finished_processes.begin(); iter!=finished_processes.end(); ++iter)
    {
        Process *proc = (*iter)->process;
        printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
                counter++, proc->AT, proc->static_TC, proc->CB, proc->IO,
                proc->static_prio, proc->FT, proc->TT, proc->IT, proc->CW);
        fin_time = proc->FT;
        total_cpu_util += proc->total_cb;
        total_io_util += proc->IT;
        total_tt += proc->TT;
        total_cw += proc->CW;
    }
    int num_procs = finished_processes.size();
    double temp = (double)fin_time/(double)100;
    thru_put = (double)num_procs/temp;
    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
            fin_time, total_cpu_util/fin_time*100, total_io_util/fin_time*100,
            total_tt/num_procs, total_cw/num_procs, thru_put);
}


int get_next_event_time(list<Event *> event_list)
{
    if(event_list.front()==nullptr)
        return 0;
    int time = event_list.front()->time_stamp;
    return time;
}


int get_random_number(int burst, int &ofs, vector<int> rand_vals)
{
    int total_vals = rand_vals[0];
//    ofs++;
    if(ofs > total_vals)
        ofs = 1;
//    printf("rand index = <%d>\n", ofs);
    return 1 + (rand_vals[ofs++] % burst);
}