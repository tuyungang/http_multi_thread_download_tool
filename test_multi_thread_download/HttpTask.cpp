#include "HttpTask.h"


CHttpTask::CHttpTask()
{

}

CHttpTask::CHttpTask(char *url, long shreshold_size)
{
    download_url = url;
    MAX_SEG_SIZE = shreshold_size * 1024 * 1024;
    download_file_size = -1;
    isseg = false;
    fp = NULL;
    //isfinished = false;
}

CHttpTask::~CHttpTask()
{
    if (!download_seg_map.empty()) {
        std::map<int, CSeedHttpTask*>::iterator it = download_seg_map.begin();
        while (it != download_seg_map.end()) {
            delete (*it).second;
            it++;
        }
    }
    download_seg_map.clear();
}

bool CHttpTask::Run()
{
#if 0 
    CURL* curl;
    CURLcode res;

    char range[64] = { 0 };
    if(parent->isseg) {
        snprintf(range, sizeof(range), "%ld-%ld", start_pos, end_pos);
    }

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, parent->download_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)this);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 5L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.143 Safari/537.36"); 
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1L);
    //curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

    if(parent->isseg)
        curl_easy_setopt(curl, CURLOPT_RANGE, range);

    res = curl_easy_perform(curl);
    if(CURLE_OK != res){
        printf("[error] curl error:%d download %ld segment url:%s\n", res, seg_no, parent->download_url.c_str());
    } else {
        isfinished = true;
    }

    curl_easy_cleanup(curl);

#endif
    return true;
}

long CHttpTask::GetDownloadFileSize(unsigned int *seg_num)
{
    double file_len = 0;
    CURL *handle = curl_easy_init ();
    curl_easy_setopt (handle, CURLOPT_URL, download_url.c_str());
    curl_easy_setopt (handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt (handle, CURLOPT_MAXREDIRS, 3L);
    curl_easy_setopt (handle, CURLOPT_AUTOREFERER, 1L);
    curl_easy_setopt (handle, CURLOPT_HEADER, 0L);  
    curl_easy_setopt (handle, CURLOPT_NOBODY, 1L);   
    curl_easy_setopt (handle, CURLOPT_FORBID_REUSE, 1);
    //curl_easy_setopt (handle, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.143 Safari/537.36"); //user-agent
    
    if (curl_easy_perform (handle) == CURLE_OK) {
        curl_easy_getinfo (handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &file_len);
    }
    else {
        file_len = -1;
    }

    if (file_len <= 0) {
        curl_easy_cleanup(handle);
        seg_total = -1;
        return seg_total;
    }

    download_file_size = file_len;
    int additional = (download_file_size % MAX_SEG_SIZE == 0) ? 0 : 1;
    seg_total = (download_file_size < MAX_SEG_SIZE) ? 1 : (download_file_size / MAX_SEG_SIZE + additional);

    if (seg_total > 1) {
        isseg = true;
    }

    curl_easy_cleanup(handle);
    *seg_num = seg_total;
    return file_len;
    //return seg_total;
}

bool CHttpTask::AssignSegTask()
{
    CSeedHttpTask *t = NULL;

    int additional = (download_file_size % MAX_SEG_SIZE == 0) ? 0 : 1;
    seg_total = (download_file_size < MAX_SEG_SIZE) ? 1 : (download_file_size / MAX_SEG_SIZE + additional);

    const char* file_name = basename((char*)download_url.c_str());
    download_file_path  = file_name;
    download_file_path = "./" + download_file_path; 
    fp = fopen(download_file_path.c_str(), "wb"); 
    if(NULL == fp) {
        printf("[error] create %s file failed!\n", download_file_path.c_str());
        return false;
    }

    if(!isseg){
        t = new CSeedHttpTask();
        t->SetParent((void*)this);
        t->start_pos = 0;
        t->end_pos = download_file_size - 1;
        t->seg_no = 1;
        download_seg_map.insert(std::map<int,CSeedHttpTask*>::value_type(1, t));
    }
    else{
        long seg_num = download_file_size / MAX_SEG_SIZE;
        for(int i = 0; i <= seg_num; i++) {
            t = new CSeedHttpTask();
            t->SetParent((void*)this);
            if(i < seg_num){
                t->start_pos = i * MAX_SEG_SIZE;
                t->end_pos = (i + 1) * MAX_SEG_SIZE - 1;
            }
            else{
                if(download_file_size % MAX_SEG_SIZE != 0){
                    t->start_pos = i * MAX_SEG_SIZE;
                    t->end_pos = download_file_size - 1;
                }else
                    break;
            }
            t->seg_no = i + 1;
            download_seg_map.insert(std::map<int,CSeedHttpTask*>::value_type(i + 1, t));
        }
    }

    return true;
}

CSeedHttpTask::CSeedHttpTask()
{
    parent = NULL;
    seg_no = -1;
    start_pos = -1;
    end_pos = -1;
    isfinished = false;
}

CSeedHttpTask::~CSeedHttpTask()
{

}


size_t CSeedHttpTask::write_data(void* ptr, size_t size, size_t nmemb, void* userdata)
{
    CSeedHttpTask* t = (CSeedHttpTask*) userdata;
    size_t written;

    if(t->parent->isseg) {
        t->parent->seg_locker.lock();
        if(t->start_pos + size * nmemb <= t->end_pos) {
            fseek(t->parent->fp, t->start_pos, SEEK_SET);
            written = fwrite(ptr, size, nmemb, t->parent->fp);
            t->start_pos += size * nmemb;
        }
        else {
            fseek(t->parent->fp, t->start_pos, SEEK_SET);
            written = fwrite(ptr, 1, t->end_pos - t->start_pos + 1, t->parent->fp);
            t->start_pos = t->end_pos;
        }
        t->parent->seg_locker.unlock();
    }
    else {
        written = fwrite(ptr, size, nmemb, t->parent->fp);
    }

    return written;
}

bool CSeedHttpTask::Run()
{
    CURL* curl;
    CURLcode res;
    bool bRet;

    char range[64] = { 0 };
    if(parent->isseg) {
        snprintf(range, sizeof(range), "%ld-%ld", start_pos, end_pos);
    }

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, parent->download_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)this);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 5L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.143 Safari/537.36"); 
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1L);
    //curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

    if(parent->isseg)
        curl_easy_setopt(curl, CURLOPT_RANGE, range);

    res = curl_easy_perform(curl);
    if(CURLE_OK != res){
        printf("[error] curl error:%d location:%ld segment url:%s\n", res, seg_no, parent->download_url.c_str());
        bRet = false;
    } else {
        isfinished = true;
        bRet = true;
    }

    curl_easy_cleanup(curl);

    return true;
}

