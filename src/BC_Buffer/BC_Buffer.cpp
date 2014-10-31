/**
 * @file BC_Buffer.cpp 
 * @author Brett Crawford
 * @brief A shared, bounded buffer class
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-10-18
 *  Description: A sharedbounded buffer class to be used with an implementation
 *  of the producer/consumer problem. The buffer class is to be used in 
 *  conjunction with a shared logger as well as with producers and consumers. 
 *  This class is written for use with linux.
*/

using namespace std;

#include "BC_Buffer.hpp"
#include <semaphore.h>

/**
 * Constructs a shared, bounded buffer object. The buffer can hold elements
 * of type void* meaning the elements should be allocated on the heap before 
 * insertion into the buffer.
 * 
 * @param[in] size The max number of elements in buffer
 * @param[in] logger A pointer to the shared logger
*/
BC_Buffer::BC_Buffer(size_t size, BC_Logger *logger)
{
	firstFilled = nextEmpty = 0;
	this->size = size;
	this->logger = logger;
	buffer = (void**) calloc(this->size, sizeof(void*));
	pthread_mutex_init(&lock, NULL);
	//pthread_mutex_init(&insert_lock, NULL);
	//pthread_mutex_init(&remove_lock, NULL);
	sem_init(&available, 0, this->size);
	sem_init(&unavailable, 0, 0);
}

/**
 * Destroys a buffer object
*/
BC_Buffer::~BC_Buffer()
{
	size_t i;
	for(i = firstFilled; i < nextEmpty; i++)
		free(buffer[i % size]);
	free(buffer);
	pthread_mutex_destroy(&lock);
	//pthread_mutex_destroy(&insert_lock);
	//pthread_mutex_destroy(&remove_lock);
	sem_destroy(&available);
	sem_destroy(&unavailable);
}

/**
 * Inserts an element in the buffer. If the buffer is full, this call will
 * block until the insert can be performed or if the shared insert data of 
 * the buffer is currently in use.
 *
 * @param[in] item A void pointer to the element to be inserted
*/
void BC_Buffer::insert(void *item)
{
	int l;
	int temp = *(int*) item;
	char *event = (char*) calloc(62, sizeof(char));

	sem_wait(&available);
	pthread_mutex_lock(&lock);
	//pthread_mutex_lock(&insert_lock);

	/** CRITICAL SECTION ENTRY */
	buffer[nextEmpty % size] = item;
	nextEmpty++;
	l = nextEmpty;
	snprintf(event, 62, "Buffer: %d inserted             nextEmpty: %d",
	         temp, l);
	logger->log_event(event);
	free(event);
	/** CRITICAL SECTION EXIT */

	pthread_mutex_unlock(&lock);
	//pthread_mutex_unlock(&insert_lock);
	sem_post(&unavailable);
}

/**
 * Removes an element from the buffer. If the buffer is empty, this call will
 * block until the remove can be performed or if the shared remove data of the 
 * buffer is currently in use.  
 *
 * @return A void pointer to the element removed from the buffer
*/
void *BC_Buffer::remove()
{
	int f;
	void *item;
	char *event = (char*) calloc(62, sizeof(char));

	sem_wait(&unavailable);
	pthread_mutex_lock(&lock);
	//pthread_mutex_lock(&remove_lock);

	/** CRITICAL SECTION ENTRY */
	item = buffer[firstFilled % size];
	buffer[firstFilled % size] = NULL;
	firstFilled++;
	f = firstFilled;
	snprintf(event, 62, "Buffer: %d removed              firstFilled: %d", 
		     *(int*)item, f);
	logger->log_event(event);
	free(event);
	/** CRITICAL SECTION EXIT */

	pthread_mutex_unlock(&lock);
	//pthread_mutex_unlock(&remove_lock);
	sem_post(&available);

	return item;
}