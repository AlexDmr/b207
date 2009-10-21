#include <Slimlib/Core/Thread.h>
#include <string.h>

#if defined (WIN32) || defined (_WIN32_WCE)
DWORD WINAPI ThreadFunc( LPVOID lpParam ) 
{  
    Thread *ct = (Thread*) lpParam;
    ct->run();
    return 0; 
}
#endif

#if defined (unix) || defined (__APPLE__)
void *ThreadFunc(void *param)
{
    Thread *t = (Thread*)(param);
    
    pthread_mutex_lock(t->get_m_start());
    if(!t->isStopping()) t->run();
    pthread_mutex_unlock(t->get_m_start());
	if(t->isStopping()) t->terminate_suspended();
	
    return 0;
}
#endif


bool Thread::isSuspended(void)
{
    return suspended;
}


bool Thread::isStopping(void)
{
    return terminated != THREAD_RUNNING;
}

Thread::Thread(bool suspended)
{
	if(suspended)
	{
		terminated = THREAD_WAITINGFORSTART;
	}
	else
	{
		terminated = THREAD_RUNNING;
	}
	
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
    
    this->suspended = suspended;
    
    pthread_mutex_init(&m_start, NULL);
    if(suspended) pthread_mutex_lock(&m_start);
    id = pthread_create(&handle, NULL, ThreadFunc, this);  
    #endif
    
}


void Thread::resume(void)
{
	if((terminated == THREAD_RUNNING) || (terminated == THREAD_STOPPING))
	{
		return;
	}
	
	terminated = THREAD_RUNNING;
	
	#if defined (WIN32) || defined (_WIN32_WCE)
    ResumeThread(handle);
    #endif

    #if defined (unix) || defined (__APPLE__)
    pthread_mutex_unlock(&m_start);
    #endif
}


int Thread::join(void)
{
    #if defined (WIN32) || defined (_WIN32_WCE)
    return (int) WaitForSingleObject(handle, INFINITE);
    #endif

    #if defined (unix) || defined (__APPLE__)
    return pthread_join(handle, NULL);
    #endif
}




void Thread::stop(void)
{
	#if defined (unix) || defined (__APPLE__)
    if(terminated == THREAD_WAITINGFORSTART)
	{
		terminated = THREAD_STOPPING;
		pthread_mutex_unlock(&m_start);
		while(terminated != THREAD_TERMINATED) Thread::sleep(10);
	}
	else 
	#endif
	if(terminated == THREAD_RUNNING)
    {
		terminated = THREAD_STOPPING;
		while(terminated != THREAD_TERMINATED) Thread::sleep(10);
	}
}


void Thread::kill(void)
{ 
	#if defined (WIN32) || defined (_WIN32_WCE)
    TerminateThread(handle, 0);
    #endif

    #if defined (unix) || defined (__APPLE__)
    pthread_kill(handle, 9); // 9 == KILL_SIGNAL_NUMBER
    #endif
}



#if defined (unix) || defined (__APPLE__)
pthread_mutex_t *Thread::get_m_start(void)
{
    return &m_start;
}

void Thread::terminate_suspended(void)
{
	terminated = THREAD_TERMINATED;
}
#endif

void Thread::run(void)
{

}


void Thread::sleep(long millisecond)
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
		earlyNotify = false;
		waiter = 0;
		
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
	if(!earlyNotify)
	{
		#if defined (WIN32) || defined (_WIN32_WCE)
		waiter++;
		WaitForSingleObject(hEvent, INFINITE);
		#endif
		
		#if defined (unix) || defined (__APPLE__)
		pthread_mutex_lock(&m);
		waiter++;
		pthread_cond_wait(&c, &m);
		pthread_mutex_unlock(&m);
		#endif
		return;
	}
	
	earlyNotify = false;
}

int Waiter::timedwait(long millisecond)
{
	if(!earlyNotify)
	{
    #if defined (WIN32) || defined (_WIN32_WCE)
    int dwWaitResult;
    waiter++;
		
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
    waiter++;
		
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
	
	earlyNotify = false;
	return 0;
}

void Waiter::notify(void)
{
    #if defined (WIN32) || defined (_WIN32_WCE)
    if(!SetEvent(hEvent))
    {
        // Error exit.
			return;
    }
	
		if(waiter == 0) 
		{
			earlyNotify = true;
		}
		else
		{
			waiter--;
		}
    #endif
    
    #if defined (unix) || defined (__APPLE__)
    pthread_mutex_lock(&m);
    pthread_cond_signal(&c);
		if(waiter == 0) 
		{
			earlyNotify = true;
		}
		else
		{
			waiter--;
		}

    pthread_mutex_unlock(&m);
    #endif
}

