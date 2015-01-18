#ifndef __timeout_H_
#define __timeout_H_

/***
* 任务最大值为  MAX_SLOT_SIZE * MAX_SLOT_TASK_SIZE
*/
#define MAX_SLOT_SIZE 0xFF    //支持最大的slot_size
#define MAX_SLOT_TASK_SIZE 0xFF  //单个slot支持最大的任务数

#define TIMMER_TRUE  1
#define TIMMER_FALSE 0

typedef  unsigned short timmer_bool;

typedef  void(callback_func)(void);  //回调函数


typedef struct _timmer_task
{
    unsigned int id;         //任务id
    callback_func *callback; //回调函数
    timmer_bool is_loop;     //是否循环执行
    struct _timmer_task *next; //下一个任务
    unsigned int run_time;    //执行的时间
    unsigned int positive_time;
} timmer_task;


typedef struct _timmer_container
{

    timmer_task *tasks;         //任务hash表
    unsigned int slot_size;     //slot大小
    unsigned int slot_time;
    unsigned int cursor_slot;   //当前slot
} timmer_container;


timmer_container *timmer_container_create(unsigned int slot_size,unsigned slot_time);
timmer_task *timmer_task_create(unsigned int id,callback_func callback, unsigned int run_time,timmer_bool is_loop);


int pack_id(unsigned int slot, unsigned int offset);
int unpack_id(unsigned int id);


int setTimeout(timmer_container *container,callback_func callback, unsigned delay);

timmer_bool  clearSetTimeout(timmer_container *container,int id);

int setInterval(timmer_container *container,callback_func callback,unsigned int interval);

timmer_bool clearSetInterval(timmer_container *container,unsigned int id);


timmer_bool time_container_loop(timmer_container *container);







#endif //__HOUSITE_YAR_timeout.h_H_
