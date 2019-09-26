/**
 * @file Task.h
 * @brief 任务基类
 * @author tuyungang
 * @version 1.0
 * @date 2019-09-25
 */
#ifndef _TASK_H
#define _TASK_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <cassert>
#include "ProcDataDef.h"


class CTask 
{ 
    private: 
        int      m_JobNo;         
        char*    m_JobName;       
        EnumJobType m_WorkType;

    public: 
        //CTask(); 
        //virtual ~CTask(); 
        
    public:
        void SetWorkType(EnumJobType type) { m_WorkType = type; }
        EnumJobType GetWorkType() { return m_WorkType; }
        int GetJobNo(void) const { return m_JobNo; } 
        void SetJobNo(int jobno){ m_JobNo = jobno;} 
        char* GetJobName(void) const { return m_JobName; } 
        void SetJobName(char* jobname); 

        virtual bool Run() = 0; 
};


#endif
