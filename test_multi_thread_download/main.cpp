/* *******************************************************
 * Call Center On Demand Product Series
 * Copyright (C) 2019 .
 * All right reserved
 * 
 * TODO: 多线程分片下载工具，可实现HTTP下载(实现)，ftp下载(可扩展)等下载方式，
 *       可设置下载阀值(线程数根据该阀值自动配置生成)，程序会自动判断是否
 *       需要多线程分段下载，程序具有一定扩展性，采用模板多态方式，无需改
 *       动主体多线程下载逻辑，只需添加相应任务之类文件即可。
 * 
 * *******************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#if _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>

#endif

#include "Util.h"
#include "ThreadPool.h"

static void show_usage(void) 
{
    printf("description:\n"
        "-H http方式下载.\n"
        "-F ftp方式下载(可扩展，未实现).\n"
        "-O 其他方式下载(可扩展，未实现).\n"
        "-s 分段下载阀值size(单位m,不设置时默认值为20),当下载文件大于阀值启用分段下载，否则不开启.\n"
        "-h help.\n"
        "\n"
        "e.g:\n"
        "test -H 'download url' -s 20\n");
 }

int main(int argc, char **argv) 
{
    int ch, type = -1;
    char download_url[200] = { 0 };
    long seg_shreshold_size = SEG_SHRESHOLD_SIZE;

    while ((ch = getopt(argc, argv, "H:F:O:s:h")) != -1)
    {
        switch (ch) 
        {
            case 'H':
                strcpy(download_url, optarg);
                type = 1;
                break;
            case 'F':
#if FTP_RUN
                //strcpy(download_url, optarg);
#endif
                show_usage();
                type = 2;
                //goto PROC_END;
                break;
            case 'O':
                show_usage();
                type = 3;
                //goto PROC_END;
                break;
            case 's':
                seg_shreshold_size = atol(optarg);
                break;
            case '?':
                show_usage();
                type = 4;
                //goto PROC_END;
                break;
            case ':':
                std::cout << "no option" << std::endl;
                show_usage();
                type = 5;
                //goto PROC_END;
                break;
            case 'h':
                show_usage();
                type = 6;
                //goto PROC_END;
                break;
            default:
                std::cout << "no option or error option" << std::endl;
                show_usage();
                type = 7;
                //goto PROC_END;
                break;
        }
    }

    CThreadPool<CHttpTask> *http_pool = NULL;
    CHttpTask *h_task = NULL;

#if FTP_RUN
    //CThreadPool<CFtpTask> *ftp_pool = NULL;
    //CFtpTask *f_task = NULL;
#endif

    std::map<int, CSeedHttpTask*>::iterator it;
    if (type == 1) {
        h_task = new CHttpTask(download_url, seg_shreshold_size);
        unsigned int seg_num = 0;
        long file_size = h_task->GetDownloadFileSize(&seg_num);
        if (file_size <= 0) {
            std::cout << "[error] get file size failed! please check url validity " << std::endl;
            goto PROC_EXIT;
        }
        h_task->SetWorkType(JOB_WORK_TYPE_HTTP);

        threadpool_conf_t conf = {seg_num, 0, 10000};
        http_pool = NULL;
        try {
           http_pool = new CThreadPool<CHttpTask>((void*)&conf);

        } catch (...) {
            std::cout << "[error] thread pool init failed!" << std::endl;
            goto PROC_EXIT;
        }

        if (!h_task->AssignSegTask()) {
            std::cout << "[warn] assign task failed!" << std::endl;
            http_pool->SetThreadExited();
            goto PROC_EXIT;
        }

        if (h_task->download_seg_map.empty()) {
            std::cout << "[warn] assign task failed!" << std::endl;
            http_pool->SetThreadExited();
            goto PROC_EXIT;
        }

        std::cout << "[notice] downloading..." << std::endl;
        it = h_task->download_seg_map.begin();
        while (it != h_task->download_seg_map.end()) {
            http_pool->append((*it).second);
            it++;
        }

    } else if (type == 2) {

#if FTP_RUN
        f_task = new CFtpTask(download_url, seg_shreshold_size);
        unsigned int seg_num = 0;
        long file_size = f_task->GetDownloadFileSize(&seg_num);
        if (file_size <= 0) {
            std::cout << "[error] get file size failed! please check url validity " << endl;
            goto PROC_EXIT;
        }
        f_task->SetWorkType(JOB_WORK_TYPE_FTP);

        threadpool_conf_t conf = {seg_num, 0, 10000};
        ftp_pool = NULL;
        try {
           ftp_pool = new CThreadPool<CFtpTask>((void*)&conf);

        } catch (...) {
            std::cout << "[error] thread pool init failed!" << endl;
            goto PROC_EXIT;
        }

        if (!f_task->AssignSegTask()) {
            std::cout << "[warn] assign task failed!" << endl;
            ftp_pool->SetThreadExited();
            goto PROC_EXIT;
        }

        if (f_task->download_seg_map.empty()) {
            std::cout << "[warn] assign task failed!" << endl;
            ftp_pool->SetThreadExited();
            goto PROC_EXIT;
        }

        std::cout << "[notice] downloading..." << endl;
        it = f_task->download_seg_map.begin();
        while (it != f_task->download_seg_map.end()) {
            ftp_pool->append((*it).second);
            it++;
        }

#endif

    } else {
        goto PROC_END;
    }
    
PROC_EXIT:

    if (http_pool) {
        http_pool->WaitThreadsExit();
    }

    if (!h_task->download_seg_map.empty()) {
        it = h_task->download_seg_map.begin();
        while (it != h_task->download_seg_map.end()) {
            if (!((*it).second->isfinished)) {
                break;
            }
            it++;
        }
        if (it == h_task->download_seg_map.end()) {
            fclose(h_task->fp);
            printf("[notice] generate %s in the current direction\n", basename((char*)h_task->download_file_path.c_str()));
            printf("[notice] download sucessfully!\n");
        } else {
            fclose(h_task->fp);
            remove(basename((char*)h_task->download_file_path.c_str()));
            printf("[notice] download failed!\n");
        }
    }

    if (http_pool != NULL) {
        delete http_pool;
        http_pool = NULL;
    }

    if (h_task != NULL) {
        delete h_task;
        h_task = NULL;
    }
    
#if FTP_RUN
    if (ftp_pool != NULL) {
        delete ftp_pool;
        ftp = NULL;
    }

    if (f_task != NULL) {
        delete f_task;
        f_task = NULL;
    }
#endif

PROC_END:

    printf("bye!\n");
    return 0;
}

