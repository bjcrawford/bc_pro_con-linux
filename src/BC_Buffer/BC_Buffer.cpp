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
 * Constructs a shared, bounded buffer object. 
 * 
 * @param[in] size The max number of elements in buffer
 * @param[in] logger A pointer to the shared logger
*/
BC_Buffer::BC_Buffer(size_t size, BC_Logger *logger)
{
	first = last = 0;
	this->size = size;
	this->logger = logger;
	buffer = (void**) calloc(this->size, sizeof(void*));
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(&insert_lock, &attr);
	pthread_mutex_init(&remove_lock, &attr);
	sem_init(&available, 0, this->size);
	sem_init(&unavailable, 0, 0);
}

/**
 * Destroys a buffer object
*/
BC_Buffer::~BC_Buffer()
{
	size_t i;
	for(i = first; i < last; i++)
		free(buffer[i % size]);
	free(buffer);
	pthread_mutex_destroy(&insert_lock);
	pthread_mutex_destroy(&remove_lock);
	sem_destroy(&available);
	sem_destroy(&unavailable);
}

/**
 * Inserts an element in the buffer. If the buffer is full, this call will
 * block until the insert can be performed. 
 *
 * @param[in] item A void pointer to the element to be inserted
*/
void BC_Buffer::insert(void *item)
{
	
	sem_wait(&available);
	this->insert_internal(item);
	sem_post(&unavailable);
}

/**
 * For internal class use only. Inserts an element in the buffer. If this 
 * method has been called, the buffer is guaranteed to be in a not full 
 * state. This call will block if the shared data of the buffer is currently
 * in use.  
 *
 * @param[in] item A void pointer to the element to be inserted
*/
void BC_Buffer::insert_internal(void *item)
{
	int l;
	char *event = (char*) calloc(62, sizeof(char));
	pthread_mutex_lock(&insert_lock);
	buffer[last % size] = item;
	last++;
	l = last;
	pthread_mutex_unlock(&insert_lock);
	snprintf(event, 62, "Buffer: %d inserted, last: %d",*(int*)item, l);
	logger->log_event(event);
	free(event);
}

/**
 * Removes an element from the buffer. If the buffer is empty, this call will
 * block until the remove can be performed. 
 *
 * @return A void pointer to the element removed from the buffer
*/
void *BC_Buffer::remove()
{
	sem_wait(&unavailable);
	void *item = this->remove_internal();
	sem_post(&available);

	return item;
}

/**
 * For internal class use only. Removes an element from the buffer. If this 
 * method has been called, the buffer is guaranteed to be in a not empty 
 * state. This call will block if the shared data of the buffer is currently
 * in use.  
 *
 * @return A void pointer to the element removed from the buffer
*/
void *BC_Buffer::remove_internal()
{
	int f;
	void *item;
	char *event = (char*) calloc(62, sizeof(char));
	pthread_mutex_lock(&remove_lock);
	item = buffer[first % size];
	buffer[first % size] = NULL;
	first++;
	f = first;
	pthread_mutex_unlock(&remove_lock);
	snprintf(event, 62, "Buffer: %d removed, first: %d", *(int*)item, f);
	logger->log_event(event);
	free(event);

	return item;
}