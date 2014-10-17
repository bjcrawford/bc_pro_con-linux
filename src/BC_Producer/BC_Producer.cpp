/* File: BC_Producer.cpp
   Author: Brett Crawford
   Date: 2014-
   Prof: Kwatny
   TAs: Liang and Casey
   Course: CIS 3207, Sec 2
   Description:
*/

using namespace std;

#include "BC_Producer.hpp"

BC_Producer::BC_Producer(int id, BC_Buffer *buffer, BC_Logger *logger)
{
	this->id = id;
	this->buffer = buffer;
	this->logger = logger;
	srand((time_t) time(NULL));
}

BC_Producer::~BC_Producer()
{

}

void BC_Producer::produce()
{
	char *event = (char*) calloc(50, sizeof(char));
	int item = rand() % 10000;
	buffer->insert(item);
	sprintf(event, "Producer %d: %d inserted into buffer", this->id, item);
	logger->log_event(event);
	free(event);
}

