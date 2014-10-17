/* File: BC_Buffer.h
   Author: Brett Crawford
   Date: 2014-
   Prof: Kwatny
   TAs: Liang and Casey
   Course: CIS 3207, Sec 2
   Description:
*/

#ifndef BC_BUFFER_H_
#define BC_BUFFER_H_

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "../BC_Logger/BC_Logger.hpp"

class BC_Buffer
{
	private:
		int first;
		int last;
		int size;
		int *buffer;
		BC_Logger *logger;
		pthread_mutexattr_t attr;
		pthread_mutex_t lock;    /* Mutual exclusion lock */
		sem_t available;         /* Number of available slots in buffer */
		sem_t unavailable;       /* Number of unavailable slots in buffer */

	public:
		BC_Buffer(int, BC_Logger*);
		virtual ~BC_Buffer();

		int isFull();
		int isEmpty();
		void insert(int);
		void insert_internal(int);
		int remove();
		int remove_internal();
};

#endif /* BC_BUFFER_H_ */
