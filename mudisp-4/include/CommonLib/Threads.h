/*
* Copyright 2008 Free Software Foundation, Inc.
*
* This software is distributed under the terms of the GNU Affero Public License.
* See the COPYING file in the main directory for details.
*
* This use of this software may be subject to additional restrictions.
* See the LEGAL file in the main directory for details.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>
#include <iostream>
#include <assert.h>
#include <semaphore.h>

class Mutex;


/**@name Multithreaded access for standard streams. */
//@{

/**@name Functions for gStreamLock. */
//@{
extern Mutex gStreamLock;	///< global lock for cout and cerr
void lockCerr();		///< call prior to writing cerr
void unlockCerr();		///< call after writing cerr
void lockCout();		///< call prior to writing cout
void unlockCout();		///< call after writing cout
//@}

/**@name Macros for standard messages. */
//@{
#define COUT(text) { lockCout(); std::cout << text; unlockCout(); }
#define CERR(text) { lockCerr(); std::cerr << __FILE__ << ":" << __LINE__ << ": " << text; unlockCerr(); }
#ifdef NDEBUG
#define DCOUT(text) {}
#define OBJDCOUT(text) {}
#else
#define DCOUT(text) { COUT(__FILE__ << ":" << __LINE__ << " " << text); }
#define OBJDCOUT(text) { DCOUT(this << " " << text); } 
#endif
//@}
//@}



/**@defgroup C++ wrappers for pthread mechanisms. */
//@{

/** @class Mutex Threads.h <CommonLibs/Threads.h> 
 * @brief A class for recursive mutexes based on pthread_mutex. 
 * */
class Mutex {

	private:
	
	pthread_mutex_t mMutex;
	pthread_mutexattr_t mAttribs;

	public:

	Mutex();

	~Mutex();
	
	/**
		Put threads resource in lock state 
	 */
	void lock() { pthread_mutex_lock(&mMutex); }
	
	/** 
		Put threads resource in unlock state 
	 */
	void unlock() { pthread_mutex_unlock(&mMutex); }

	friend class Signal;

};


/** @class Signal Threads.h <CommonLibs/Threads.h> 
 * @brief A C++ interthread signal based on pthread condition variables.
 * */

class Signal {

	private:

	mutable pthread_cond_t mSignal;

	public:

	Signal() { int s = pthread_cond_init(&mSignal,NULL); assert(s == 0); }

	~Signal() { pthread_cond_destroy(&mSignal); }

	/**
		Block for the signal up to the cancellation timeout.
		Under Linux, spurious returns are possible.
	*/
	void wait(Mutex& wMutex, unsigned timeout) const;

	/**
		Block for the signal.
		Under Linux, spurious returns are possible.
	*/
	void wait(Mutex& wMutex) const
		{ pthread_cond_wait(&mSignal,&wMutex.mMutex); }

	/** 
		Send unicast unlock signal 
	*/
	void signal() { pthread_cond_signal(&mSignal); }

	/** 
		Send broadcast unlock signal 
	*/
	void broadcast() { pthread_cond_broadcast(&mSignal); }

};

/** @class ThreadSemaphore Threads.h <CommonLibs/Threads.h> 
 * @brief  Semaphore 
 * */
class ThreadSemaphore {

	private:

	/** @var sem_t mSem
	 * @brief Define mutex semaphore 
	 * */
	sem_t mSem;

	public:

/** @enum Result */
 /*!  @brief Define result state for semaphore */
	enum Result {
		TSEM_OK,       /*!< Success. */
		TSEM_TIMEOUT,  /*!< wait() or trywait() timed out. */
		TSEM_OVERFLOW, /*!< post() overflows a semaphore */
		TSEM_ERROR     /*!< Generic error. */
	};

	/** Create and initialize semaphore.
	* @param[in] value - initial semaphore value.
	*/
	ThreadSemaphore(unsigned value = 0)
	{
		int s = sem_init(&mSem,0,value);
		assert(s == 0);
	}

	~ThreadSemaphore() { sem_destroy(&mSem); }

	/** Wait for semaphore to be signaled with timeout.
	* @param[in] timeoutMs - timeout in milliseconds
	*
	* @retval TSEM_OK on success.
	* @retval TSEM_TIMEOUT on timeout.
	* @retval TSEM_ERROR on error.
	*/
	Result wait(unsigned timeoutMs);

	/** Wait for semaphore to be signaled infinitely.
	* @retval TSEM_OK on success.
	* @retval TSEM_ERROR on error.
	*/
	Result wait();

	/** Check if semaphore has been signaled and disarm it.
	* @retval TSEM_OK is semaphore is signaled.
	* @retval TSEM_TIMEOUT if semaphore is not signaled.
	* @retval TSEM_ERROR on error.
	*/
	Result trywait();

	/** Signal semaphore.
	* @retval TSEM_OK on success.
	* @retval TSEM_ERROR on error.
	*/
	Result post();

};
/** @def START_THREAD(thread,function,argument) thread.start((void *(*)(void*))function, (void*)argument)
 * @brief Define association from thread an runtime function */
#define START_THREAD(thread,function,argument) \
	thread.start((void *(*)(void*))function, (void*)argument);


/** @class Thread Threads.h <CommonLibs/Threads.h> 
 * @brief  A C++ wrapper for pthread threads.  */
class Thread {

	private:
	/** @var pthread_t mThread
	 * @brief Define  posix mutex threads
	 * */
	pthread_t mThread;
	
   /** @var pthread_attr_t mAttrib
	 * @brief Define posix mutex threads attrinutes variable
	 * */
	pthread_attr_t mAttrib;
	
	/** @var size_t mStackSize
	 * @brief define stack size for threads class */
	// FIXME -- Can this be reduced now?
	size_t mStackSize;
	

	public:

	/** @fn  Thread(size_t wStackSize = (65536*4)):mThread((pthread_t)0)
	 * 	@brief Constructor. Create a thread in a non-running state. 
	 * @param wStackSize Define stack size for inizialization 
	 * */
	Thread(size_t wStackSize = (65536*4)):mThread((pthread_t)0) { mStackSize=wStackSize;}

	/**
		Destroy the Thread.
		It should be stopped and joined.
	*/
	~Thread() { int s = pthread_attr_destroy(&mAttrib); assert(s==0); }


	/** Start the thread on a task. */
	void start(void *(*task)(void*), void *arg);

	/** Join a thread that will stop on its own. */
 
	void join() { int s = pthread_join(mThread,NULL); assert(s==0); }

};




#endif
// vim: ts=4 sw=4
