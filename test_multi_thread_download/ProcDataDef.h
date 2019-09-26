/**
 * @file ProcDataDef.h
 * @brief 变量声明
 * @author tuyungang
 * @version 1.0
 * @date 2019-09-25
 */
#ifndef _PROC_DATA_DEF_H
#define _PROC_DATA_DEF_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_TASK_SIZE        99999
#define SEG_SHRESHOLD_SIZE   20
#define THREAD_STACK_SIZE    0
#define THREAD_NUMBER        8

typedef enum
{
    JOB_WORK_TYPE_NONE = 0,
    JOB_WORK_TYPE_HTTP,
    JOB_WORK_TYPE_FTP,
    JOB_WORK_TYPE_OTHER
}EnumJobType;


typedef struct threadpool_conf_s
{
    unsigned int thread_num;
    unsigned int thread_stack_size;
    unsigned int max_tasknum;
}threadpool_conf_t, *pthreadpool_conf_t;


typedef struct task_queue_s
{
    void *head;
    void **tail;
    unsigned int max_tasknum;
    unsigned int cur_tasknum;
}task_queue_t;

#endif
