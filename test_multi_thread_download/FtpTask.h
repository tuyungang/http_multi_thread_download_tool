/**
 * @file FtpTask.h
 * @brief ftp下载任务子类(扩展类，未实现)
 * @author tuyungang
 * @version 1.0
 * @date 2019-09-25
 */
#ifndef _FTP_TASK_H
#define _FTP_TASK_H

#include "Util.h"
#include "Task.h"
#include "locker.h"

class CSeedFtpTask;


class CFtpTask : public CTask
{
    public:
        CFtpTask(){}
        CFtpTask(char *url, long shreshold_size){}
        virtual ~CFtpTask(){}

    public:
        map<int, CSeedFtpTask*> download_seg_map;

    public:
        bool Run(){}
        long GetDownloadFileSize(unsigned int *seg_num){}
        bool AssignSegTask(){}
};


class CSeedFtpTask : public CFtpTask 
{
    public:
        CSeedHttpTask(){}
        virtual ~CSeedHttpTask(){}

    public:
        CFtpTask *parent;

    public:
        bool Run(){}
        void SetParent(void *p) {parent = (CFtpTask *)p;}
};


#endif
