//#include "header.h"
#include "classes.cpp"


queue<Process*> read_file();
void put_event(int time_stamp, Process *process, trans_to state, vector<Event *> &event_queue);
Event *get_event(vector<Event *> &event_queue);
void simulation(vector<Event *> &event_queue);



int main()
{
    int AT, TC, CB, IO;
    queue<Process *> run_queue;
    vector<Event *> event_queue;
    string file_path = "./lab2_assign/input4";
    FILE *ptr = fopen(file_path.c_str(), "r");

    while (fscanf(ptr, "%d %d %d %d", &AT, &TC, &CB, &IO) > 0) {
        Process *process = new Process(AT, TC, CB, IO);
        put_event(AT, process, TRANS_TO_RUNNING, event_queue);
        run_queue.push(process);
    }

    for(int i=0; i<event_queue.size(); i++)
        printf("proc %d = <%d, %d>\n", i+1, event_queue[i]->time_stamp, event_queue[i]->process->get_CB());
//    enum trans_to {CREATED, READY, RUNNING, BLOCKED};
//    queue<Process *> q = read_file();
    int ts;
    Process *proc;
    trans_to state;
    Event *evt = get_event(event_queue);
    printf("ts = <%d>\n",evt->time_stamp);
    Event *evt1 = get_event(event_queue);
    printf("ts = <%d>\n",evt1->time_stamp);
}


queue<Process*> read_file()
{
    int AT, TC, CB, IO;
    queue<Process *> run_queue;
    vector<Event *> event_queue;
    string file_path = "./lab2_assign/input4";
    FILE *ptr = fopen(file_path.c_str(), "r");

    while (fscanf(ptr, "%d %d %d %d", &AT, &TC, &CB, &IO) > 0) {
        Process *process = new Process(AT, TC, CB, IO);
        put_event(AT, process, TRANS_TO_RUNNING, event_queue);
        run_queue.push(process);
    }

    for(int i=0; i<event_queue.size(); i++)
        printf("proc %d = <%d, %d>\n", i+1, event_queue[i]->time_stamp, event_queue[i]->process->get_CB());
    return run_queue;
}


void put_event(int time_stamp, Process *process, trans_to state, vector<Event *> &event_queue)
{
    Event *ev = new Event(time_stamp, process, state);
    if(event_queue.empty())     //insert event at beginning of Q if empty
    {
        event_queue.insert(event_queue.begin(), ev);
        return;
    }
    bool is_inserted = false;
    for(int i=0; i<event_queue.size(); i++)     //compares new entry with each event in Q
    {
        int ts = event_queue[i]->time_stamp;
        if(ts < ev->time_stamp)
        {
            continue;
        }
        else if(ts > ev->time_stamp )
        {
            printf("its bigger\n");
            event_queue.insert(event_queue.begin()+i, ev);
            is_inserted = true;
            break;
        }
        else if(ts==ev->time_stamp)
        {
            bool is_equal = true;
            int j = i;
            int new_ts = ev->time_stamp;
            while(is_equal)
            {
                if(event_queue[j]==NULL)
                {
                    event_queue.insert(event_queue.end(), ev);
                    return;
                }
                int ts1 = event_queue[j]->time_stamp;
                if(ts1!=new_ts)
                    is_equal = false;
                j++;
            }
            event_queue.insert(event_queue.begin()+i+1, ev);
            is_inserted = true;
            break;
        }
    }
    if(!is_inserted)
        event_queue.insert(event_queue.end(), ev);
}


Event *get_event(vector<Event *> &event_queue)
{
    if(event_queue.empty())
        return nullptr;
    Event *evt = event_queue.front();

//    ts = evt->time_stamp;
//    proc  = *evt->process;
//    state = evt->state;

    event_queue.erase(event_queue.begin());
    return evt;
}


void simulation(vector<Event *> &event_queue)
{
    Event *evt;
    while(evt = get_event(event_queue))
    {
        Process *proc = evt->process;
        int current_time = evt->time_stamp;
        int time_in_state = current_time - proc->get_AT();
    }
}