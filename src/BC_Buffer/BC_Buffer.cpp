/* File: BC_Buffer.cpp
   Author: Brett Crawford
   Date: 2014-
   Prof: Kwatny
   TAs: Liang and Casey
   Course: CIS 3207, Sec 2
   Description:
*/

using namespace std;

#include "BC_Buffer.hpp"
#include <semaphore.h>


BC_Buffer::BC_Buffer(int size, BC_Logger *logger)
{
	first = last = 0;
	this->size = size;
	this->logger = logger;
	buffer = (int*) calloc(this->size, sizeof(int));
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(&lock, &attr);
	sem_init(&available, 0, this->size);
	sem_init(&unavailable, 0, 0);
}

BC_Buffer::~BC_Buffer()
{
	free(buffer);
	pthread_mutex_destroy(&lock);
	sem_destroy(&available);
	sem_destroy(&unavailable);
}

/**
 * Checks if the buffer is full.
 * Returns 1 if the buffer is full, otherwise 0
 */
int BC_Buffer::isFull()
{
	int result = 0;
	pthread_mutex_lock(&lock);
	if((last - first) == size)
		result = 1;
	pthread_mutex_unlock(&lock);

	return result;
}

int BC_Buffer::isEmpty()
{
	int result = 0;
	pthread_mutex_lock(&lock);
	if((last - first == 0))
		result = 1;
	pthread_mutex_unlock(&lock);

	return result;
}

void BC_Buffer::insert(int item)
{
	
	sem_wait(&available);
	this->insert_internal(item);
	sem_post(&unavailable);
}

void BC_Buffer::insert_internal(int item)
{
	int num;
	char *event = (char*) calloc(50, sizeof(char));
	pthread_mutex_lock(&lock);
	buffer[last % size] = item;
	last++;
	num = last - first;
	pthread_mutex_unlock(&lock);
	sprintf(event, "Buffer: %d inserted, num: %d, first: %d, last: %d", item, num, first, last);
	logger->log_event(event);
	free(event);
}

int BC_Buffer::remove()
{
	sem_wait(&unavailable);
	int item = this->remove_internal();
	sem_post(&available);

	return item;
}

int BC_Buffer::remove_internal()
{
	int num;
	char *event = (char*) calloc(50, sizeof(char));
	pthread_mutex_lock(&lock);
	int item = buffer[first % size];
	first++;
	num = last - first;
	pthread_mutex_unlock(&lock);
	sprintf(event, "Buffer: %d removed, num: %d, first: %d, last: %d", item, num, first, last);
	logger->log_event(event);
	free(event);

	return item;
}

