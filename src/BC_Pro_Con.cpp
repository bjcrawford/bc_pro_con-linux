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

/** Includes */
#include "BC_Buffer/BC_Buffer.hpp"
#include "BC_Consumer/BC_Consumer.hpp"
#include "BC_Logger/BC_Logger.hpp"
#include "BC_Producer/BC_Producer.hpp"

#include <iostream>
#include <pthread.h>
#include <stdio.h>

/** Namespace */
using namespace std;

/** Constants */
#define BUFFER_SIZE 20      /**< Size of the buffer */
#define NUM_PRODUCERS 4     /**< Number of producers */
#define NUM_CONSUMERS 3     /**< Number of consumers */
#define NUM_PRODUCTIONS 20  /**< Number of productions each producer makes */
#define NUM_CONSUMPTIONS 25 /**< Number of consumptions each consumer makes */

/** The name of the log file to be used */
const char log_file[] = "log.txt";

/** A struct to hold the arguments to be passed to the produce function */
typedef struct 
{
	size_t num;             /**< The number of productions to make */
	BC_Producer *producer;  /**< A pointer to the producer object */
} produce_args;

/** A struct to hold the arguments to be passed to the consume function */
typedef struct 
{
	size_t num;             /**< The number of consumptions to make */
	BC_Consumer *consumer;  /**< A pointer to the consumer object */
} consume_args;

void *produce(void*);
void *consume(void*);

/**
 * The main function of the solution.
*/
int main(int argc, char **argv)
{
	printf("Main thread start\n");

	/** Declare variables */
	int i;
	BC_Logger *logger;
	BC_Buffer *buffer;
	BC_Producer *producer[NUM_PRODUCERS];
	BC_Consumer *consumer[NUM_CONSUMERS];
	pthread_t producer_threads[NUM_PRODUCERS];
	pthread_t consumer_threads[NUM_CONSUMERS];

	/** Instantiate logger and buffer objects */
	logger = new BC_Logger(log_file);
	buffer = new BC_Buffer(BUFFER_SIZE, logger);

	/** Instantiate producers */
	for(i = 0; i < NUM_PRODUCERS; i++)
		producer[i] = new BC_Producer(i, buffer, logger);

	/** Instantiate consumers */
	for(i = 0; i < NUM_CONSUMERS; i++)
		consumer[i] = new BC_Consumer(i, buffer, logger);

	/** Create producer threads */
	for(i = 0; i < NUM_PRODUCERS; i++)
	{
		produce_args *p_args = (produce_args*) calloc(1, sizeof(produce_args));
		p_args->num = NUM_PRODUCTIONS;
		p_args->producer = producer[i];
		pthread_create(&(producer_threads[i]), NULL, 
			           (void* (*)(void*)) &produce, p_args);
		printf("Producer %d thread created\n", i);
	}

	/** Create consumer threads */
	for(i = 0; i < NUM_CONSUMERS; i++)
	{
		consume_args *c_args = (consume_args*) calloc(1, sizeof(consume_args));
		c_args->num = NUM_CONSUMPTIONS;
		c_args->consumer = consumer[i];
		pthread_create(&(consumer_threads[i]), NULL, 
			           (void* (*)(void*)) &consume, c_args);
		printf("Consumer %d thread created\n", i);
	}

	/** Ensure producer threads finished */
	for(i = 0; i < NUM_PRODUCERS; i++)
	{
		pthread_join(producer_threads[i], NULL);
		printf("Producer %d thread joined\n", i);
	}

	/** Ensure consumer threads finished */
	for(i = 0; i < NUM_CONSUMERS; i++)
	{
		pthread_join(consumer_threads[i], NULL);
		printf("Consumer %d thread joined\n", i);
	}

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
	produce_args *p_args = (produce_args*) args;
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
	consume_args *c_args = (consume_args*) args;
	for(i = 0; i < c_args->num; i++)
		c_args->consumer->consume();
	pthread_exit(NULL);
}
