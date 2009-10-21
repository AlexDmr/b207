/*
    Thread for Posix
*/
/**
 * @file slimthread.h
 */
#ifndef __Class_Thread_
#define __Class_Thread_

#include <Slimlib/Core/Object.h>

#if defined (unix) || defined (__APPLE__)
	#include <pthread.h>
	#include <unistd.h>
	#define DWORD  int
#endif

#ifdef unix
		#include <sys/time.h>
#endif

#if defined (WIN32) || defined (_WIN32_WCE) || defined (_WIN32)
    #include <windows.h>
#endif

/**
 * Control value for Thread::terminated. Indicate that thread is not yet started
 */
#define THREAD_WAITINGFORSTART	-1

/**
 * Control value for Thread::terminated. Indicate that thread is running
 */
#define THREAD_RUNNING      	 0

/**
 * Control value for Thread::terminated. Indicate that thread is stopping
 */
#define THREAD_STOPPING      	 1

/**
 * Control value for Thread::terminated. Indicate that thread is stopped
 */
#define THREAD_TERMINATED   	 2

/**
 * @see Thread::Thread()
 */
#define THREAD_SUSPENDED			true

/**
 * @see Thread::Thread()
 */
#define THREAD_NOT_SUSPENDED		false



/**
* @brief Thread is a class to encapsulate a thread.
*/
class Thread: public Object
{
    private:
       bool suspended;
		
    protected:

		#if defined (unix) || defined (__APPLE__)
		pthread_mutex_t m_start;
        pthread_t handle;   
		#endif

		#if defined (WIN32) || defined (_WIN32_WCE)
        HANDLE handle;
        #endif
        
       	DWORD id;
       	/**
		 * Derivated classes have to used
		 */
       	int terminated;

    public:
		
		/**
		 * Create a new thread, and according to <i>suspended</i>, start it or not.
		 * When the thread is starting, run() method is called.<br><b>Warning: run() is called immediatelly, without waiting for the end of the constructor's execution !</b>
		 * - if derived class constructor does some initialization, it must call Thread constructor with SUSPENDED value, and resume() thread at the end of the initialization code.
		 * The thread terminate when run() terminate.
		 * @param suspended must be SUSPENDED if the thread must be created suspended. Call resume() to start the run() method. Must be NOT_SUSPENDED otherwise.
		 * @see resume()
		 * @see run()
		 */		
		Thread(bool suspended);
		virtual ~Thread() { };
		
		
		
		/**
		 * Get the thread state
		 * @return return true if this thread is in a suspended state, false if it is running
		 */
		bool isSuspended(void);
		
		/**
		 * Get the thread stopping state
		 * @return return true if this thread is sopping or stopped, false otherwise
		 */
		bool isStopping(void);
       
        
        
        /** This function have to be implemented in your specialized class. 
		 * This method has to contain the threaded code. Its termination make the thread terminate.
		 *  THIS METHOD MUST NOT BE CALLED MANUALY
		 */
        virtual void run(void);
       
       
       
	    /**
		 * Start a thread that was created suspended
		 * @see Thread()
		 * @see run()
		 */
        void resume(void);
		
		
		/**
		 * Make the caller waiting for thread termination
		 * @return 0 if successfull, an error code otherwise
		 */
        int join(void);
        
        
        /**
		 * Stop the thread by controlling the tread main loop.
		 * Basically, this method set Thread::terminated to THREAD_STOPPING and loop until the
		 * thread main loop set Thread::terminated to THREAD_TERMINATED.<br>
		 * As a consequence, this method work ONLY if derivated classes implements their main loop as follow<br>
		 * <code>while(terminated == THREAD_RUNNING)
		 * {
		 *  // thread main loop
		 * }
		 * terminated = THREAD_TERMINATED;<code><br>
		 * In some cases, for instance when the thread main loop of the derived class is waiting for a blocking call,
		 * that method MUST be overrride.
		 */
		virtual void stop(void);

		/**
		 * Kill the thread and give no chance to the running code to end properly.
		 * Prefer stop() except if this thread as a chance to
		 */
		void kill(void);
		
		
		#if defined (unix) || defined (__APPLE__)
		/**
		 * Undocumented function. Internal use only, you do not have to use it
		 */
		pthread_mutex_t *get_m_start(void);
		
		void terminate_suspended(void);
		#endif
		
		/**
		 * Make the calling thread sleeping for <i>millisecond</i> millisecond
		 * @param millisecond containing the time to sleep
		 */
		static void sleep(long millisecond);
};



/**
 * @brief Mutex is a class to manage mutual exclusion
 */
class Mutex: public Object
{
    private:
        #if defined (unix) || defined (__APPLE__)
		pthread_mutex_t m;
		#endif
		
		#if defined (WIN32) || defined (_WIN32_WCE)
		HANDLE hMutex;
		#endif
    
    public:
        Mutex(void);        
        virtual ~Mutex(void);



		/**
		 * Lock the mutex. If the mutex is already lock, it make the calling thread waiting until a lock's owner unlock the mutex
		 * @see unlock()
		 */
        void lock(void);
        
        
        
         /**
		 * Unlock the mutex
		 * @see lock()
		 */
        void unlock(void);
};



/**
 * @brief Waiter is a class to manage thread syncronization
 */
class Waiter: public Object
{
    private:
		#if defined (unix) || defined (__APPLE__)
        pthread_mutex_t m;
        pthread_cond_t c;
        struct timespec t;
		#endif
		
		#if defined (WIN32) || defined (_WIN32_WCE)
		HANDLE hEvent;
		#endif
		
		bool earlyNotify;
		long waiter;
        
    public:
        Waiter(void);
        virtual ~Waiter(void);
        
        
        /**
		 * Make the calling thread waiting. This thread will be awaked when an other thread call notify()
		 * @see notify()
		 */
        void wait(void);
        
        
        
        /**
		 * Make the calling thread waiting. This thread will be awaked when an other thread call notify() or 
		 * if <i>millisecond</i> time is up.
		 * @param millisecond contains a value in millisecond for the timeout
		 * @return 0 if a notify() occurs before <i>millisecond</i>, 1 else
		 */
        int timedwait(long millisecond);
        
        
        
        /**
		 * Make a waiting thread awake
		 */
        void notify(void);
};
#endif
