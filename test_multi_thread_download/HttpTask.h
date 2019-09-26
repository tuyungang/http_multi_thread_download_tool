/**
 * @file HttpTask.h
 * @brief http下载任务子类
 * @author tuyungang
 * @version 1.0
 * @date 2019-09-25
 */
#ifndef _HTTP_TASK_H
#define _HTTP_TASK_H

#include "Util.h"
#include "Task.h"
#include "locker.h"

//class CTask;
class CSeedHttpTask;


class CHttpTask : public CTask
{
    public:
        CHttpTask();
        CHttpTask(char *url, long shreshold_size);
        virtual ~CHttpTask();

    public:
        /**
         * 下载文件地址
         */
        string  download_url;
        /**
         * 保存下载文件名
         */
        string  download_file_path;
        /**
         * 分段数
         */
        long    seg_total;
        /**
         * 下载文件大小
         */
        long    download_file_size;
        /**
         * 分段最大阀值
         */
        long    MAX_SEG_SIZE;
        /**
         * 分段下载标志
         */
        bool    isseg;
        FILE    *fp;
        /**
         * 分段下载写文件锁
         */
        locker  seg_locker;

        /**
         * 保存分段子任务
         */
        map<int, CSeedHttpTask*> download_seg_map;

    public:
        /**
         * @brief Run 
         *
         * @returns  
         */
        bool Run();
        /**
         * @brief GetDownloadFileSize 获取下载文件大小并得到下载分片数 
         *
         * @param seg_num 得到下载分片数
         *
         * @returns  返回下载文件大小
         */
        long GetDownloadFileSize(unsigned int *seg_num);
        /**
         * @brief AssignSegTask 分配并推送任务至任务链表供工作线程抢占获取 
         *
         * @returns  
         */
        bool AssignSegTask();
};


class CSeedHttpTask : public CHttpTask 
{
    public:
        CSeedHttpTask();
        virtual ~CSeedHttpTask();

    public:
        /**
         * 父类指针
         */
        CHttpTask *parent;

        /**
         * 分段号
         */
        long seg_no;
        /**
         * 分段开始位置 
         */
        long start_pos;
        /**
         * 分段结束位置
         */
        long end_pos;
        /**
         * 分段下载完成标志
         */
        bool isfinished;

    public:
        /**
         * @brief Run 执行下载任务
         *
         * @returns  
         */
        bool Run();
        /**
         * @brief SetParent 设置父类指针 
         *
         * @param p
         */
        void SetParent(void *p) {parent = (CHttpTask *)p;}
        /**
         * @brief write_data curl执行下载时所需调用函数，写文件至本地
         *
         * @param ptr
         * @param size
         * @param nmemb
         * @param userdata
         *
         * @returns  
         */
        static size_t write_data(void* ptr, size_t size, size_t nmemb, void* userdata);
};

#endif
