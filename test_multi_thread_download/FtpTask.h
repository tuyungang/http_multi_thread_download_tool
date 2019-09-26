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
        bool Run(){}
};


class CSeedFtpTask : public CFtpTask 
{
    public:
        CSeedHttpTask(){}
        virtual ~CSeedHttpTask(){}

    public:
        bool Run(){}
};


#endif
