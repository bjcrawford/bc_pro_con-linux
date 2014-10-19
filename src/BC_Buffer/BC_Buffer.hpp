/**
 * @file BC_Buffer.hpp
 * @author Brett Crawford
 * @brief A shared, bounded buffer class header
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-10-18
 *  Description: The header for the buffer class.
*/

#ifndef BC_BUFFER_H_
#define BC_BUFFER_H_

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "../BC_Logger/BC_Logger.hpp"

/**
 * A shared, bounded buffer class to be used with an implementation of the 
 * producer/consumer problem. The buffer class is to be used in conjunction 
 * with a shared logger as well as with producers and consumers.
*/ 
class BC_Buffer
{
	private:
		size_t first;           /**< The index of the first filled slot */
		size_t last;            /**< The index of the first open slot */
		size_t size;            /**< The max number of elements */
		void **buffer;          /**< An array of void pointers */
		BC_Logger *logger;      /**< A pointer to the shared logger */
		pthread_mutex_t lock;   /**< Mutual exclusion lock */
		sem_t available;        /**< Number of available slots in buffer */
		sem_t unavailable;      /**< Number of unavailable slots in buffer */

		void insert_internal(void*);
		void *remove_internal();

	public:
		BC_Buffer(size_t, BC_Logger*);
		virtual ~BC_Buffer();

		void insert(void*);
		void *remove();
};

#endif /* BC_BUFFER_H_ */
