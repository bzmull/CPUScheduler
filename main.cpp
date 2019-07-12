//#include "header.h"
#include "classes.cpp"
#include <cstring>

list<Process*> read_file();
void put_event(int time_stamp, Process *process, trans_to state, list<Event *> &event_list);
Event *get_event(list<Event *> &event_list);
void simulation(list<Event *> &event_list, list<Process *> &ready_queue, int ofs,
        vector<int> rand_vals, int time_quantum, sched_type schedType);
int get_next_event_time(list<Event *> event_list);
int get_random_number(int burst, int &ofs, vector<int> rand_vals);
void e_sched_evt_queue(list<Event *> &evt_queue, Process *current_proc, Event *new_evt, int current_time);

//bool debug = true;
bool debug = false;
bool print_verbose = false;

sched_type schedType;

//int main() {
//    sched_type schedType = F;
//    int time_quantum = 10000;
//    int max_prio = 4;   //by default for FCFS and co.
//    string file_path = "./lab2_assign/input1";

int main(int argc, char **argv) {

    int arg_count = 1;
    int time_quantum = 0;
    int max_prio = 0;
    char type;
    Scheduler *sched;
//    enum sched_type {F, L, S, R, P, E};
//    sched_type schedType;
    int option;
    string rand_ints_file_path = "./lab2_assign/rfile";
    string file_path = "./lab2_assign/input3";


    if (argc > 1) {
//        cout << argc << endl;
        while ((option = getopt(argc, argv, "vtes:")) != -1) {
            arg_count++;
            switch (option) {
                case 'v':
                    print_verbose = true;
                    break;

                case 't':
                    debug = true; //temporarily using debug for trace
                    break;

                case 'e':
                    debug = true; //temporarily using debug for whatever e is
                    break;

                case 's': {
                    //                sscanf(optarg, "%s%d:%d", &time_quantum, &max_prio);
                    //                Scheduler *sched = new FCFS();
                    time_quantum = 10000;
                    max_prio = 4;

                    int len = strlen(optarg);
//                        cout << len << optarg[0] << endl;
                    if (len == 1)
                        switch (optarg[0]) {
                            case 'F':
                                schedType = F;
                                break;
                            case 'L':
                                schedType = L;
                                break;
                            case 'S':
                                schedType = S;
                                break;
                        }
                    else if (len > 1) {
                        string str_optarg = optarg;
                        if (str_optarg.find(":") == -1) {
                            sscanf(optarg, "%c%d", &type, &time_quantum);
//                            cout << type <<  " no colon " << time_quantum << endl;
                        }
                        else {
                            sscanf(optarg, "%c%d:%d", &type, &time_quantum, &max_prio);
//                            cout << "colon" << endl;
                        }
                        switch (type) {
                            case 'R':
//                                cout << "its R" << endl;
                                schedType = R;
                                break;
                            case 'P':
                                schedType = P;
                                break;
                            case 'E':
                                schedType = E;
                                break;
                        }
                    }
                    break;
                }
            }
        }
        rand_ints_file_path = argv[arg_count+1];
        file_path = argv[arg_count];
    }
    else {
        print_verbose = true;
        schedType = E;
        time_quantum = 4;
        max_prio = 4;

    }
//    cout << schedType << " " << time_quantum << endl;
//    cout << "in path = " <<  file_path << endl;
//    int time_quantum = strtol(argv[2], nullptr, 10);

//    printf("quantum = <%d>\n", time_quantum);

    int ofs = 1;
    int rand;

    vector<int> rand_vals;
//    string rand_ints_file_path = argv[arg_count+1];
    FILE *ptr1 = fopen(rand_ints_file_path.c_str(), "r");

    while(fscanf(ptr1, "%d", &rand) > 0)
        rand_vals.push_back(rand);


    int static_prio = 0;

    int AT, TC, CB, IO;
    int proc_num = 0;
    list<Process *> ready_queue;
    list<Event *> event_list;
    list<Process *> proc_table;
    list<Event *> finished_processes;

//    string file_path = argv[2];
//    string file_path = argv[arg_count];
    FILE *ptr2 = fopen(file_path.c_str(), "r");

    while (fscanf(ptr2, "%d %d %d %d", &AT, &TC, &CB, &IO) > 0) {
        static_prio = get_random_number(max_prio, ofs, rand_vals);
        Process *process = new Process(proc_num, AT, TC, CB, IO, static_prio);
        put_event(AT, process, CREATED, event_list);
        proc_table.push_back(process);
        proc_num++;
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

//    Scheduler *sched1 = new FCFS();
    simulation(event_list, ready_queue, ofs, rand_vals, time_quantum, schedType);
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
        printf("pid = <%d>; prio = <%d> ;", (*iter)->proc_num, (*iter)->dynamic_prio);
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


void e_sched_evt_queue(list<Event *> &evt_queue, Process *current_proc, Event *new_evt, int current_time) {
    list<Event *>::iterator iter;
    bool is_preempted = true;

    for(iter=evt_queue.begin(); iter!=evt_queue.end();) {
        if((*iter)->time_stamp==current_time && current_proc->proc_num == (*iter)->process->proc_num) {
            iter = evt_queue.erase(iter);
//            evt_queue.remove(*iter);
            iter ++;
            evt_queue.push_front((*iter));
            is_preempted = false;
        }
        else if((*iter)->time_stamp!=current_time && current_proc->proc_num == (*iter)->process->proc_num) {
            iter = evt_queue.erase(iter);
//            evt_queue.remove(*iter);
            iter ++;
        }
        else {
            ++iter;
        }
    }
    if(is_preempted) {
        Event *temp_evt = new Event(current_time, current_proc, TRANS_TO_READY);
        evt_queue.push_front(temp_evt);
    }
}



void simulation(list<Event *> &event_list, list<Process *> &ready_queue, int ofs,
        vector<int> rand_vals, int time_quantum, sched_type schedType)
{
    string sched_algo;
    Scheduler *sched;
    switch (schedType) {
        case F: {
            sched = new FCFS();
            sched_algo = "FCFS";
            break;
        }
        case L: {
            sched = new LCFS();
            sched_algo = "LCFS";
            break;
        }
        case S: {
            sched = new SRTF();
            sched_algo = "SRTF";
            break;
        }
        case R: {
            sched = new RoundRobin();
            sched_algo = "RR";
            break;
        }
        case P: {
            sched = new Prio();
            sched_algo = "PRIO";
            break;
        }
        case E: {
            sched = new PrePrio();
            sched_algo = "PREPRIO";
            break;
        }
    }

    list<Event *> finished_processes;
    Process *CURRENT_RUNNING_PROCESS = nullptr;
    Event *evt;
    int block_counter = 0;
    int block_time = 0;
    int current_time = 0;
    int delta_time = 0;
    int cb_counter = 0;
    bool reuse_cb = false;

    while((evt = get_event(event_list)))
    {
        cout << evt->process << endl;
        Process *proc = evt->process;
        delta_time = evt->time_stamp - current_time;
        current_time = evt->time_stamp;
        int time_in_state = current_time - proc->state_ts;

        if (block_counter > 0) {
            block_time += delta_time;
        }

        if (debug) {
            printf("current time=%d, pid=%d, state ts=%d\n", current_time, proc->proc_num, proc->state_ts);
        }
        proc->state_ts = current_time;
        bool call_sched = false;
        int cb, io;
        bool was_blocked = true;


        if(evt->state == CREATED)
        {
            was_blocked = false;
            if(print_verbose)
                printf("%d %d %d: CREATED -> READY\n", evt->time_stamp, proc->proc_num, time_in_state);
            evt->state = TRANS_TO_READY;
        }


        switch(evt->state)
        {
            case TRANS_TO_READY:
                if(schedType==E && CURRENT_RUNNING_PROCESS != nullptr) {
                    if(CURRENT_RUNNING_PROCESS->dynamic_prio < proc->dynamic_prio) {
                        evt->state = TRANS_TO_PREEMPT;
                        Event *evt1 = new Event(evt->time_stamp, evt->process, evt->state);
                        event_list.push_front(evt1);
                        e_sched_evt_queue(event_list, CURRENT_RUNNING_PROCESS, evt, current_time);
                        break;
                    }
                }

                if(proc->prev_state==TRANS_TO_RUNNING) {
                    CURRENT_RUNNING_PROCESS = nullptr;
                    proc->current_cb -= time_quantum;
                    if(print_verbose)
                        printf("%d %d %d: RUNNG -> READY  cb=%d rem=%d prio=%d\n", evt->time_stamp, proc->proc_num,
                                time_in_state, proc->current_cb, proc->TC, proc->dynamic_prio);
                    if (schedType == P || schedType == E) {
                        proc->dynamic_prio--;
                    }
                }
                if(proc->prev_state==TRANS_TO_BLOCKED) {
                    proc->current_cb = 0;
                    if(print_verbose)
                        printf("%d %d %d: BLOCK -> READY\n", evt->time_stamp, proc->proc_num, time_in_state);
                    block_counter --;
                    proc->dynamic_prio = proc->static_prio-1;
                }
                proc->ready_start_time = current_time;

                sched->add_process(proc, ready_queue); //add to ready_Q --> main functionality of whichever algorithm.
//                printf("added process <%d>\n", proc->proc_num);
//                printf("ready proc AT = <%d>\n", ready_queue.front()->AT);
                print_ready_q(ready_queue);

                call_sched = true;
                break;
            case TRANS_TO_RUNNING:
//                if(!reuse_cb) {
                if(proc->current_cb==0)
                    proc->current_cb = get_random_number(proc->get_CB(), ofs, rand_vals); //create random CB
                if(proc->current_cb > proc->TC)
                    proc->current_cb = proc->TC;
                if(proc->current_cb > time_quantum) {

                    put_event(current_time+time_quantum, proc, TRANS_TO_READY, event_list);
                    if(print_verbose)
                        printf("%d %d %d: READY -> RUNNG cb=%d rem=%d prio=%d\n", evt->time_stamp, proc->proc_num,
                               time_in_state, proc->current_cb, proc->TC, proc->dynamic_prio);
                    proc->TC = proc->TC - time_quantum;
                    proc->CW += time_in_state;
                    proc->total_cb += time_quantum;
                    proc->prev_state = TRANS_TO_RUNNING;
                    break;
                }

                if(print_verbose)
                    printf("%d %d %d: READY -> RUNNG cb=%d rem=%d prio=%d\n", evt->time_stamp, proc->proc_num,
                            time_in_state, proc->current_cb, proc->TC, proc->dynamic_prio);

                proc->TC = proc->TC - proc->current_cb;
                proc->CW += time_in_state;
                proc->total_cb += proc->current_cb;
                proc->prev_state = TRANS_TO_RUNNING;
                put_event(current_time + proc->current_cb, proc, TRANS_TO_BLOCKED, event_list);


                break;
            case TRANS_TO_BLOCKED:
                CURRENT_RUNNING_PROCESS = nullptr;
                was_blocked = true;
                call_sched = true;
                proc->prev_state = TRANS_TO_BLOCKED;

                if(proc->TC==0)
                {
                    if(print_verbose)
                        printf("%d %d %d: Done\n", current_time, proc->proc_num, proc->current_cb);
                    proc->FT = current_time;
                    proc->TT = current_time - proc->AT;
                    put_event(proc->proc_num, proc, TRANS_TO_READY, finished_processes);
                }
                else {
                    proc->prev_state = TRANS_TO_BLOCKED;
                    block_counter++;
                    io = get_random_number(proc->get_IO(), ofs, rand_vals); //create random IO
                    proc->IT = proc->IT + io;
                    if(print_verbose)
                        printf("%d %d %d: RUNNG -> BLOCK  ib=%d rem=%d\n", evt->time_stamp, proc->proc_num,
                                time_in_state, io, proc->TC);
                    put_event(current_time+io, proc, TRANS_TO_READY, event_list);
                }

                //and update remaining time (for SRTF)
                break;
            case TRANS_TO_PREEMPT:
                CURRENT_RUNNING_PROCESS = nullptr;
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
                CURRENT_RUNNING_PROCESS->prev_state = TRANS_TO_READY;
                put_event(current_time, CURRENT_RUNNING_PROCESS, TRANS_TO_RUNNING, event_list);
            }
        }

        print_evt_q(event_list);

    }
    //FINAL PRINT

    double total_cpu_util=0, total_io_util=0, total_tt=0, total_cw=0, thru_put=0.0;
    int fin_time=0, counter=0;
    list<Event *>::iterator iter = finished_processes.begin();
    if(time_quantum == 10000)
        cout << sched_algo << endl;
    else
        cout << sched_algo << " " << time_quantum << endl;
//    cout << sched_algo << " " << time_quantum << endl;
//    printf("%s\n", sched_algo);
    for(iter=finished_processes.begin(); iter!=finished_processes.end(); ++iter)
    {
        Process *proc = (*iter)->process;
        printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
                counter++, proc->AT, proc->static_TC, proc->CB, proc->IO,
                proc->static_prio, proc->FT, proc->TT, proc->IT, proc->CW);
        if(proc->FT > fin_time)
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
            fin_time, total_cpu_util/fin_time*100, 1.*block_time/fin_time*100,
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




// END OF TRANS_TO_RUNNING
//                proc->state_ts = current_time;
//                proc->CW = current_time - proc->ready_start_time;

//                current_time = current_time + cb;
//                evt->state = TRANS_TO_BLOCKED; //and change to TRANS_TO_BLOCKED (or PREEMPT if using PREPRIO)


//                if(proc->TC > 0)
//                    put_event(current_time+cb, proc, TRANS_TO_BLOCKED, event_list);
//                else {
//                    printf("%d %d %d: Done\n", current_time+cb, proc->proc_num, cb);
//                    proc->FT = current_time + cb;
//                    proc->TT = current_time + cb - proc->AT;
//                    put_event(proc->proc_num, proc, TRANS_TO_READY, finished_processes);
//                }

//END OF TRANS_TO_BLOCKED
//                io = get_random_number(proc->get_IO(), ofs, rand_vals); //create random IO
//                printf("%d %d %d: RUNNG -> BLOCK  ib=%d rem=%d\n", evt->time_stamp, proc->proc_num,
//                       time_in_state, io, proc->TC);
//                proc->state_ts = current_time;
//                proc->IT = proc->IT + io;
//                current_time = current_time + io;
//                evt->state = TRANS_TO_READY; //and change to TRANS_TO_READY
//                put_event(current_time+io, proc, TRANS_TO_READY, event_list);