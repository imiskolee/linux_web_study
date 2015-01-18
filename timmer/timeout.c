#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<unistd.h>
#include<signal.h>


#include "timeout.h"


timmer_container *timmer_container_create(unsigned int slot_size,unsigned slot_time){

    if(slot_size > MAX_SLOT_SIZE) return NULL;

    timmer_container *container = (timmer_container *)malloc(sizeof(timmer_container));

    container->slot_size = slot_size;
    container->slot_time = slot_time;
    container->cursor_slot  = 0;

    container->tasks = (timmer_task *)malloc(sizeof(timmer_task) * container->slot_size);

    unsigned int i;
    for(i =0;i<container->slot_size;i++){
        container->tasks[i].id = -1;
        container->tasks[i].next = NULL;
        container->tasks[i].callback = NULL;
        container->tasks[i].run_time = 0;
        container->tasks[i].positive_time = 0;
    }

    return container;
};

timmer_task *timmer_task_create(unsigned int id,callback_func callback, unsigned int run_time,timmer_bool is_loop){

    timmer_task *task = (timmer_task *)malloc(sizeof(timmer_task));

    task->id = id;
    task->callback = callback;
    task->run_time = run_time;
    task->is_loop = is_loop;
    task->next = NULL;
    task->positive_time = 0;


    return task;
}

int timmer_task_add(timmer_container *container,callback_func callback, unsigned int delay,timmer_bool is_loop){
    unsigned int slot = (container->cursor_slot + ((int)(delay / container->slot_time))) % container->slot_size;

    time_t run_time = time(NULL) + delay;

    unsigned int id = pack_id (slot, get_task_id(slot));

    timmer_task *task = timmer_task_create (id, callback, run_time, is_loop);

    task->positive_time = delay;

    timmer_task *head = &(container->tasks[slot]);

    while(head->next != NULL){

        head = head->next;

    }

    head->next = task;
    return id;
}


int pack_id(unsigned int slot, unsigned int offset){

    return ((slot & 0xFF) << 16) + (offset & 0xFF);

};

int unpack_id(unsigned int id){

    return (id ^ 0xFF00) >>16;;

};


int get_task_id(unsigned int slot){

    static unsigned  task_ids[MAX_SLOT_SIZE];

    return task_ids[slot]++;
}



int setTimeout(timmer_container *container,callback_func callback, unsigned delay){

    return timmer_task_add (container, callback, delay, 0);
};


timmer_bool  clearSetTimeout(timmer_container *container,int id){

    unsigned int slot = unpack_id (id);

    timmer_task *head = &(container->tasks[slot]);

    while(head->next != NULL){

        timmer_task *next = head->next;

        if(next->id == id){

            head->next = next->next;

            free(next);

            return TIMMER_TRUE;

        }

    }

    return TIMMER_FALSE;

}



int setInterval(timmer_container *container,callback_func callback, unsigned delay){

    return timmer_task_add (container, callback, delay, 1);
};


timmer_bool clearSetInterval(timmer_container *container,unsigned int id){
    return clearSetTimeout (container, id);
}


void do_time_slot(timmer_container *container, unsigned int slot){

    time_t now = time(NULL);

    timmer_task *head = &(container->tasks[slot]);

    while(head->next != NULL){
        timmer_task *task = head->next;
        if(task->run_time <= now){
            task->callback();
            head->next = task->next;
            free(task);
            if(task->is_loop == TIMMER_TRUE){
                timmer_task_add (container, task->callback, task->positive_time, task->is_loop);
            }
        }

        if(head->next){
            head = head->next;
        }


    }
}


timmer_bool time_container_loop(timmer_container *container){
    printf("cursor%d\n",container->cursor_slot);
    do_time_slot (container, container->cursor_slot);
    container->cursor_slot++;

    if(container->cursor_slot >= container->slot_size) container->cursor_slot = 0;
};


