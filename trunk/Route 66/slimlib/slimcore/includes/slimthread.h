/*
    SlimThread for Posix
*/

#ifndef _SLIMTHREAD_
#define _SLIMTHREAD_

#if defined (unix) || defined (__APPLE__)
	#include <pthread.h>

	#define DWORD  int
#endif

#ifdef unix
		#include <sys/time.h>
		#include <unistd.h>
#endif

#if defined (WIN32) || defined (_WIN32) || defined (_WIN32_WCE)
    #include <windows.h>
#endif


/**
* @brief SlimThread is a class to encapsulate a thread.
*/
class SlimThread
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

    public:

		/**
		 * Create a new thread, and according to <i>suspended</i>, start it or not.
		 * When the thread is starting, run() method is called.
		 * The thread terminate when run() terminate.
		 * @param suspended must be true if the thread must be created suspended. You will heve to call resume() later to start it.
		 * @see resume()
		 * @see run()
		 */
		SlimThread(bool suspended);
		virtual ~SlimThread() { };



		/**
		 * Get the thread state
		 * @return true if this thread is in a suspended state, false if it is running
		 */
		bool isSuspended(void);



        /** This function have to be implemented in your specialized class.
		 * This method has to contain the threaded code. Its termination make the thread terminate.
		 *  THIS METHOD MUST NOT BE CALLED MANUALY
		 */
        virtual void run(void);



	    /**
		 * Start a thread that was created suspended
		 * @see SlimThread()
		 * @see run()
		 */
        void resume(void);



		#if defined (unix) || defined (__APPLE__)
		/**
		 * Undocumented function. Internal use only, you do not have to use it
		 */
		pthread_mutex_t *get_m_start(void);
		#endif
};



/**
 * Make the calling thread sleeping for <i>millisecond</i> millisecond
 * @param millisecond containing the time to sleep
 */
void slimSleep(long millisecond);



/**
 * @brief Mutex is a class to manage mutual exclusion
 */
class Mutex
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
        ~Mutex(void);



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
class Waiter
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

    public:
        Waiter(void);
        ~Waiter(void);


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
