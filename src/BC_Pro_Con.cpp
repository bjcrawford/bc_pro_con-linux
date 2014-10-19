/**
 * @file BC_Pro_Con.cpp 
 * @author Brett Crawford
 * @brief A producer/consumer problem solution
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-10-18
 *  Description: 
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

struct produce_args
{
	size_t num;
	BC_Producer *producer;
};

struct consume_args
{
	size_t num;
	BC_Consumer *consumer;
};

void *produce(void*);
void *consume(void*);

/**
 * The main function of the solution.
*/
int main(int argc, char **argv)
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
	{
		struct produce_args *p_args = (struct produce_args*) calloc(1, sizeof(produce_args));
		p_args->num = 20;
		p_args->producer = producer[i];
		pthread_create(&(producer_threads[i]), NULL, (void* (*)(void*)) &produce, p_args);
	}

	for(i = 0; i < 3; i++)
	{
		struct consume_args *c_args = (struct consume_args*) calloc(1, sizeof(consume_args));
		c_args->num = 25;
		c_args->consumer = consumer[i];
		pthread_create(&(consumer_threads[i]), NULL, (void* (*)(void*)) &consume, c_args);
	}

	for(i = 0; i < 4; i++)
		pthread_join(producer_threads[i], NULL);

	for(i = 0; i < 3; i++)
		pthread_join(consumer_threads[i], NULL);

	printf("Main thread finished\n");

	return EXIT_SUCCESS;
}

/**
 * Uses a producer object to perform num productions.
 *
 * @param[in] num The number of productions to perform
 * @param[in] producer A pointer to the producer object
*/
void *produce(void *args)
{
	size_t i;
	struct produce_args *p_args = (struct produce_args*) args;
	for(i = 0; i < p_args->num; i++)
		p_args->producer->produce();
	pthread_exit(NULL);
}

/**
 * Uses a consumer object to perform num consumptions.
 *
 * @param[in] num The number of consumptions to perform
 * @param[in] producer A pointer to the consumer object
*/
void *consume(void *args)
{
	size_t i;
	struct consume_args *c_args = (struct consume_args*) args;
	for(i = 0; i < c_args->num; i++)
		c_args->consumer->consume();
	pthread_exit(NULL);
}
