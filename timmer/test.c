#include "timeout.h"

#include <stdio.h>
#include <time.h>
#include<unistd.h>
#include<signal.h>

timmer_container *container = NULL;

void sig_alarm(void){

    printf("alarm\n");

    if(!container) return  ;

    time_container_loop (container);

    signal(SIGALRM, sig_alarm);

    alarm(container->slot_time);

    pause ();
}

void test_timeout(){

    printf("task run.\n");
}



int main(void){

    container = timmer_container_create (5, 1);

    //int id = setTimeout (container, test_timeout, 1);

   // clearSetTimeout (container, id);
    setInterval (container, test_timeout, 1);
    sig_alarm();

}