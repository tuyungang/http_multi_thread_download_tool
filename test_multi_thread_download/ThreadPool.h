/**
 * @file ThreadPool.h
 * @brief 线程池
 * @author tuyungang
 * @version 1.0
 * @date 2019-09-25
 */
#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include <sys/epoll.h>
#include <cassert>
#include <fcntl.h>
#include <vector>
#include <map>
#include "ProcDataDef.h"
#include "locker.h"
#include "HttpTask.h"

template< typename T >
class CThreadPool
{
public:
    CThreadPool(void *arg);
    ~CThreadPool();
    /**
     * @brief append 
     *
     * @param request
     *
     * @returns  
     */
    bool append(T* request);
    /**
     * @brief get_threads 
     *
     * @returns  
     */
    pthread_t* get_threads(){ return m_Threads; }


private:
    /**
     * @brief run 
     */
    void run();
    /**
     * @brief z_conf_check 
     *
     * @param conf
     *
     * @returns  
     */
    int z_conf_check(threadpool_conf_t *conf);
    //void z_task_queue_init(task_queue_t *task_queue);

public:
    /**
     * @brief WorkerThreadProc  下载文件工作线程
     *
     * @param arg
     *
     * @returns  
     */
    static void* WorkerThreadProc( void* arg );
    /**
     * @brief WorkerWriteLogFileThread 写错误日志线程
     *
     * @param arg
     *
     * @returns  
     */
    static void* WorkerWriteLogFileThread( void* arg );

    /**
     * @brief Log 打印日志信息并推送至日志链表
     *
     * @param format
     * @param ...
     */
    void Log(const char* format, ... );  
    /**
     * @brief RunWriteLogFileThread 写错误日志文件 
     */
    void RunWriteLogFileThread();  
    /**
     * @brief SetThreadExited 设置并通知所有工作线程退出
     */
    void SetThreadExited(); 
    /**
     * @brief GetThreadNum 获取线程池线程数
     *
     * @returns  
     */
    unsigned int GetThreadNum() { return m_nThreadNum; }
    /**
     * @brief WaitThreadsExit 等待所有工作线程退出
     */
    void WaitThreadsExit();

private:
    //thread_conf_t m_ThreadPoolCfg;
    /**
     * 线程池中线程数
     */
    unsigned int m_nThreadNum;
    /**
     * 线程池处理最大任务数 
     */
    unsigned int m_nMaxTaskNum;
    /**
     * 线程池单个线程堆栈大小
     */
    unsigned int m_nThreadStackSize;
    /**
     * 当前任务数
     */
    unsigned int m_nCurTaskNum;
    pthread_t* m_Threads;
    pthread_t m_LogThr;
    /**
     * 任务存放链表 
     */
    std::list< T* > m_TaskQueue;
    /**
     * 日志存放链表
     */
    std::list< char* > m_LogQueue;
    /**
     * 任务链表线程锁
     */
    locker m_TaskQueuelocker;
    /**
     * 日志链表线程锁
     */
    locker m_Loglocker;
    /**
     * 通知工作线程获取任务信号量
     */
    sem m_TaskQueueSem;
    /**
     * 通知日志线程获取日志信息信号量
     */
    sem m_LogSem;
    /**
     * 工作线程退出标志
     */
    bool m_stop;
};

#include "ThreadPool.template"

#endif
