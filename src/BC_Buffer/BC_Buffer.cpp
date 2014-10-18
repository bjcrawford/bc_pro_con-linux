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


BC_Buffer::BC_Buffer(size_t size, BC_Logger *logger)
{
	first = last = 0;
	this->size = size;
	this->logger = logger;
	buffer = (void**) calloc(this->size, sizeof(void*));
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(&lock, &attr);
	sem_init(&available, 0, this->size);
	sem_init(&unavailable, 0, 0);
}

BC_Buffer::~BC_Buffer()
{
	int i;
	for(i = first; i < last; i++)
		free(buffer[i % size]);
	free(buffer);
	pthread_mutex_destroy(&lock);
	sem_destroy(&available);
	sem_destroy(&unavailable);
}

void BC_Buffer::insert(void *item)
{
	
	sem_wait(&available);
	this->insert_internal(item);
	sem_post(&unavailable);
}

void BC_Buffer::insert_internal(void *item)
{
	int f, l;
	char *event = (char*) calloc(60, sizeof(char));
	pthread_mutex_lock(&lock);
	buffer[last % size] = item;
	last++;
	f = first;
	l - last;
	pthread_mutex_unlock(&lock);
	sprintf_s(event, 59, "Buffer: %d inserted, l - f: %d, f: %d, l: %d", *(int*)item, l - f, f, l);
	logger->log_event(event);
	free(event);
}

void *BC_Buffer::remove()
{
	sem_wait(&unavailable);
	void *item = this->remove_internal();
	sem_post(&available);

	return item;
}

void *BC_Buffer::remove_internal()
{
	int f, l;
	void *item;
	char *event = (char*) calloc(60, sizeof(char));
	pthread_mutex_lock(&lock);
	item = buffer[first % size];
	buffer[first % size] = NULL;
	first++;
	f = first;
	l = last;
	pthread_mutex_unlock(&lock);
	sprintf(event, 59, "Buffer: %d removed, l - f: %d, f: %d, l: %d", *(int*)item, l - f, f, l);
	logger->log_event(event);
	free(event);

	return item;
}

