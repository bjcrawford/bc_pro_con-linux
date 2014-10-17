/* File: BC_Pro_Con.cpp
   Author: Brett Crawford
   Date: 2014-
   Prof: Kwatny
   TAs: Liang and Casey
   Course: CIS 3207, Sec 2
   Description:
*/

#include "BC_Buffer/BC_Buffer.h"
#include "BC_Consumer/BC_Consumer.h"
#include "BC_Logger/BC_Logger.h"
#include "BC_Producer/BC_Producer.h"

#include <iostream>
#include <pthread.h>
#include <stdio.h>

using namespace std;

#define BUFFER_SIZE 20

const char log_file[] = "log.txt";

void pro_con_solution();
void *produce(BC_Producer*);
void *consume(BC_Consumer*);
void logger_test();

int main(int argc, char **argv)
{
	pro_con_solution();
	return EXIT_SUCCESS;
}

void pro_con_solution()
{
	printf("Main thread Start\n");
	BC_Logger *logger;
	BC_Buffer *buffer;
	BC_Producer *producer;
	BC_Consumer *consumer;
	pthread_t threads[2];

	logger = new BC_Logger(log_file);
	buffer = new BC_Buffer(BUFFER_SIZE, logger);
	producer = new BC_Producer(0, buffer, logger);
	consumer = new BC_Consumer(0, buffer, logger);

	if(pthread_create(&(threads[0]), NULL, (void* (*)(void*)) &produce, producer))
	{
		perror("");
		exit(-1);
	}
	if(pthread_create(&(threads[1]), NULL, (void* (*)(void*)) &consume, consumer))
		{
			perror("");
			exit(-1);
		}
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	printf("Main thread finished\n");
}

void *produce(BC_Producer *producer)
{
	int i;
	for(i = 0; i < 25; i++)
		producer->produce();
	pthread_exit(NULL);
}

void *consume(BC_Consumer *consumer)
{
	int i;
	for(i = 0; i < 25; i++)
		consumer->consume();
	pthread_exit(NULL);
}

void logger_test()
{
	char *s = (char*) calloc(20, sizeof(char));
	BC_Logger *my_log = new BC_Logger("log.txt");
	int i;
	for(i = 0; i < 10; i++)
	{
		sprintf(s, "Event %d", i);
		my_log->log_event(s);
	}
	free(s);
	delete my_log;
}
