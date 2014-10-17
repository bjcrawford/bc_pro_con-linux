/* File: BC_Pro_Con.cpp
   Author: Brett Crawford
   Date: 2014-
   Prof: Kwatny
   TAs: Liang and Casey
   Course: CIS 3207, Sec 2
   Description:
*/

#include "BC_Buffer/BC_Buffer.hpp"
#include "BC_Consumer/BC_Consumer.hpp"
#include "BC_Logger/BC_Logger.hpp"
#include "BC_Producer/BC_Producer.hpp"

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
	int i;
	BC_Logger *logger;
	BC_Buffer *buffer;
	BC_Producer *producer[4];
	BC_Consumer *consumer[3];
	pthread_t producer_threads[4];
	pthread_t consumer_threads[3];

	logger = new BC_Logger(log_file);
	buffer = new BC_Buffer(BUFFER_SIZE, logger);

	for(i = 0; i < 4; i++)
		producer[i] = new BC_Producer(i, buffer, logger);

	for(i = 0; i < 3; i++)
		consumer[i] = new BC_Consumer(i, buffer, logger);

	for(i = 0; i < 4; i++)
		pthread_create(&(producer_threads[i]), NULL, (void* (*)(void*)) &produce, producer[i]);

	for(i = 0; i < 3; i++)
		pthread_create(&(consumer_threads[i]), NULL, (void* (*)(void*)) &consume, consumer[i]);

	for(i = 0; i < 4; i++)
		pthread_join(producer_threads[i], NULL);

	for(i = 0; i < 3; i++)
		pthread_join(consumer_threads[i], NULL);

	printf("Main thread finished\n");
}

void *produce(BC_Producer *producer)
{
	int i;
	for(i = 0; i < 20; i++)
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
