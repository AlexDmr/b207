#include "slimthread.h"

#if defined (WIN32) || defined (_WIN32_WCE)
DWORD WINAPI ThreadFunc( LPVOID lpParam ) 
{  
    SlimThread *ct = (SlimThread*) lpParam;
    ct->run();
    return 0; 
}
#endif

#if defined (unix) || defined (__APPLE__)
void *ThreadFunc(void *param)
{
    SlimThread *t = (SlimThread*)(param);
    
    pthread_mutex_lock(t->get_m_start());
    t->run();
    pthread_mutex_unlock(t->get_m_start());

    return 0;
}
#endif


bool SlimThread::isSuspended(void)
{
    return suspended;
}


SlimThread::SlimThread(bool suspended)
{
    #if defined (WIN32) || defined (_WIN32_WCE)
    DWORD flags = 0;
    
    if(suspended) flags = CREATE_SUSPENDED;
    
    
    
    handle = CreateThread( 
        NULL,               // default security attributes 
        0,                  // use default stack size  
        ThreadFunc,         // thread function 
        this,               // argument to thread function 
        flags,              // use default creation flags or CREATE_SUSPENDED
        &id);                       // returns the thread identifier 
        
    #endif

    #if defined (unix) || defined (__APPLE__)
    int rc;
    
    this->suspended = suspended;
    
    pthread_mutex_init(&m_start, NULL);
    if(suspended) pthread_mutex_lock(&m_start);
    id = pthread_create(&handle, NULL, ThreadFunc, this);  
    #endif
    
}


void SlimThread::resume(void)
{
    #if defined (WIN32) || defined (_WIN32_WCE)
    ResumeThread(handle);
    #endif

    #if defined (unix) || defined (__APPLE__)
    pthread_mutex_unlock(&m_start);
    #endif
}


#if defined (unix) || defined (__APPLE__)
pthread_mutex_t *SlimThread::get_m_start(void)
{
    return &m_start;
}
#endif

void SlimThread::run(void)
{

}


void slimSleep(long millisecond)
{
    #if defined (WIN32) || defined (_WIN32_WCE)
    Sleep(millisecond);
    #endif

    #if defined (unix) || defined (__APPLE__)
    usleep(millisecond*1000);
    #endif
}





Mutex::Mutex(void)
{            
    #if defined (WIN32) || defined (_WIN32_WCE)
    hMutex = CreateMutex(
    NULL,           // no security attributes
    FALSE,          // initially not owned
    NULL);          // no name

    if (hMutex == NULL) 
    {
        // Check for error.
    }
    #endif

    #if defined (unix) || defined (__APPLE__)
    pthread_mutex_init(&m, NULL);
    #endif
}
        
Mutex::~Mutex(void)
{
    #if defined (WIN32) || defined (_WIN32_WCE)
    CloseHandle(hMutex);
    #endif

    #if defined (unix) || defined (__APPLE__)
    pthread_mutex_destroy(&m);
    #endif
}
        
void Mutex::lock(void)
{
    #if defined (WIN32) || defined (_WIN32_WCE)
    WaitForSingleObject(hMutex, INFINITE);
    #endif
    
    #if defined (unix) || defined (__APPLE__)
    pthread_mutex_lock(&m);
    #endif
}
        
void Mutex::unlock(void)
{
    #if defined (WIN32) || defined (_WIN32_WCE)
    if (! ReleaseMutex(hMutex)) 
    { 
        // Deal with error.
    } 
    #endif
    
    #if defined (unix) || defined (__APPLE__)
    pthread_mutex_unlock(&m);
    #endif
}



Waiter::Waiter(void)
{
    #if defined (WIN32) || defined (_WIN32_WCE)
    hEvent = CreateEvent(
    NULL,        // no security attributes
    FALSE,       // auto-reset
    FALSE,       // initially not signaled
    NULL);       // no name 
    #endif
    
    #if defined (unix) || defined (__APPLE__)
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&c, NULL);
    #endif
}

Waiter::~Waiter(void)
{
    #if defined (WIN32) || defined (_WIN32_WCE)
    CloseHandle(hEvent);
    #endif
    
    #if defined (unix) || defined (__APPLE__)
    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&c);
    #endif
}

void Waiter::wait(void)
{
    #if defined (WIN32) || defined (_WIN32_WCE)
    WaitForSingleObject(hEvent, INFINITE);
    #endif
    
    #if defined (unix) || defined (__APPLE__)
    pthread_mutex_lock(&m);
    pthread_cond_wait(&c, &m);
    pthread_mutex_unlock(&m);
    #endif
}

int Waiter::timedwait(long millisecond)
{
    #if defined (WIN32) || defined (_WIN32_WCE)
    int dwWaitResult;
    
    dwWaitResult = WaitForSingleObject(hEvent, (DWORD)(millisecond));
    
    switch(dwWaitResult)
    {
        case WAIT_OBJECT_0:
                return 0;
        case WAIT_TIMEOUT:
                return 1;
    }    
    return 2;
    #endif
    
    #if defined (unix) || defined (__APPLE__)
    struct timeval tv;
    struct timezone tz;
    int rc;
    
    pthread_mutex_lock(&m);
    
    t.tv_sec = millisecond / 1000;
    t.tv_nsec = (millisecond - (t.tv_sec * 1000)) * 1000000;
    
    //printf("sec: %d, nsec: %d\n", t.tv_sec, t.tv_nsec);
    
    gettimeofday(&tv, &tz);
    
    t.tv_nsec += tv.tv_usec * 1000;
    t.tv_sec += tv.tv_sec;
    
    //printf("sec: %d, nsec: %d\n", t.tv_sec, t.tv_nsec);
    rc = pthread_cond_timedwait(&c, &m, &t);
    
    pthread_mutex_unlock(&m);
    
    return rc;
    #endif
}

void Waiter::notify(void)
{
    #if defined (WIN32) || defined (_WIN32_WCE)
    if(!SetEvent(hEvent))
    {
        // Error exit.
    }
    #endif
    
    #if defined (unix) || defined (__APPLE__)
    pthread_mutex_lock(&m);
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
    #endif
}

