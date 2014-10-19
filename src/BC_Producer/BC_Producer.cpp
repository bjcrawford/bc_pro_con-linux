/* File: BC_Producer.cpp
   Author: Brett Crawford
   Date: 2014-10-18
   Prof: Kwatny
   TAs: Liang and Casey
   Course: CIS 3207, Sec 2
   Description: A producer class to be used with an implementation of the 
   producer/consumer problem. The producer class is to be used in conjunction
   with a shared buffer and a shared logger as well as with other producers
   and consumers.
*/

using namespace std;

#include "BC_Producer.hpp"

/**
 * Constructs a producer object. 
 * 
 * @param[in] id An identifer for the producer
 * @param[in] buffer A pointer to the shared buffer
 * @param[in] logger A pointer to the shared logger
*/
BC_Producer::BC_Producer(int id, BC_Buffer *buffer, BC_Logger *logger)
{
	this->id = id;
	this->buffer = buffer;
	this->logger = logger;
}

/**
 * Destroys a producer object
*/
BC_Producer::~BC_Producer()
{
	this->buffer = NULL;
	this->logger = NULL;
}

/**
 * Produces a pseudo-randomly generated number between 0 and 9999,
 * stores its value in dynamically allocated memory, inserts a 
 * pointer to the value into the buffer, and logs the event with
 * logger.
*/
void BC_Producer::produce()
{
	char *event = (char*) calloc(62, sizeof(char));
	int *item = (int*) calloc(1, sizeof(int));
	srand(clock() * time(NULL));
	*item = rand() % 10000;
	buffer->insert((void*) item);
	snprintf(event, 
			 62, 
			 "Producer %d: %d inserted into buffer", 
			 this->id, *item);
	logger->log_event(event);
	free(event);
}

