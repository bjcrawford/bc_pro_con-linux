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
 #include <string>

/** Namespace */
using namespace std;

/** Constants */
//#define BUFFER_SIZE 20      /**< Size of the buffer */
//#define NUM_PRODUCERS 4     /**< Number of producers */
//#define NUM_CONSUMERS 3     /**< Number of consumers */
//#define NUM_PRODUCTIONS 20  /**< Number of productions each producer makes */
//#define NUM_CONSUMPTIONS 25 /**< Number of consumptions each consumer makes */

/** The name of the log file to be used */
//const char log_file[] = "log.txt";

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

/** Function declarations */
void *produce(void*);
void *consume(void*);

/**
 * The main function of the solution.
*/
int main(int argc, char **argv)
{
	/** Declare variables */
	size_t i;                /**< Iteration variable */
	size_t buffer_size;      /**< Size of the buffer */
	size_t num_producers;    /**< Number of producers */
	size_t num_consumers;    /**< Number of consumers */
	size_t num_productions;  /**< Number of productions each producer makes */
	size_t num_consumptions; /**< Number of consumptions each consumer makes */
	string log_file;
	BC_Logger *logger;
	BC_Buffer *buffer;
	BC_Producer **producer;
	BC_Consumer **consumer;
	pthread_t *producer_threads;
	pthread_t *consumer_threads;

	cout << "Main thread start\n\n";
	cout << "Welcome to the Producer-Consumer solution by Brett Crawford\n\n";
	cout << "Please enter a size for the buffer: ";
	cin >> buffer_size;
	cout << "\nPlease enter the number of producers: ";
	cin >> num_producers;
	cout << "\nPlease enter the number of consumers: ";
	cin >> num_consumers;
	cout << "\nPlease enter the number of productions for each producer: ";
	cin >> num_productions;
	cout << "\nPlease enter the number of consumptions for each consumer: ";
	cin >> num_consumptions;
	cout << "\nPlease enter a name for the log file: ";
	cin >> log_file;

	/** Instantiate logger and buffer objects */
	logger = new BC_Logger(log_file.c_str());
	buffer = new BC_Buffer(buffer_size, logger);

	//producer = (**BC_Producer) calloc(num_producers, sizeof(*BC_Producer));
	producer = new BC_Producer*[num_producers];
	//consumer = (**BC_Consumer) calloc(num_consumers, sizeof(*BC_Consumer));
	consumer = new BC_Consumer*[num_consumers];
	producer_threads = (pthread_t*) calloc(num_producers, sizeof(pthread_t));
	consumer_threads = (pthread_t*) calloc(num_consumers, sizeof(pthread_t));

	/** Instantiate producers */
	for(i = 0; i < num_producers; i++)
		producer[i] = new BC_Producer(i, buffer, logger);

	/** Instantiate consumers */
	for(i = 0; i < num_consumers; i++)
		consumer[i] = new BC_Consumer(i, buffer, logger);

	/** Create producer threads */
	for(i = 0; i < num_producers; i++)
	{
		produce_args *p_args = (produce_args*) calloc(1, sizeof(produce_args));
		p_args->num = num_productions;
		p_args->producer = producer[i];
		pthread_create(&(producer_threads[i]), NULL, 
			           (void* (*)(void*)) &produce, p_args);
		cout << "Producer " << (int) i << " thread created\n";
	}

	/** Create consumer threads */
	for(i = 0; i < num_consumers; i++)
	{
		consume_args *c_args = (consume_args*) calloc(1, sizeof(consume_args));
		c_args->num = num_consumptions;
		c_args->consumer = consumer[i];
		pthread_create(&(consumer_threads[i]), NULL, 
			           (void* (*)(void*)) &consume, c_args);
		cout << "Consumer " << (int) i << " thread created\n";
	}

	/** Ensure producer threads finished */
	for(i = 0; i < num_producers; i++)
	{
		pthread_join(producer_threads[i], NULL);
		cout << "Producer " << (int) i << " thread joined\n";
	}

	/** Ensure consumer threads finished */
	for(i = 0; i < num_consumers; i++)
	{
		pthread_join(consumer_threads[i], NULL);
		cout << "Consumer " << (int) i << " thread joined\n";
	}

	/** Clean up */
	delete(logger);
	delete(buffer);
	for(i = 0; i < num_producers; i++)
		delete(producer[i]);
	for(i = 0; i < num_consumers; i++)
		delete(consumer[i]);

	cout << "Log results saved in file " << log_file << "\n";

	cout << "Main thread finished\n";

	return EXIT_SUCCESS;
}

/**
 * Uses a producer object to perform num productions.
 *
 * @param[in] args A void pointer to a produce_args struct containing 
 *                 the necessary arguments.
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
 * @param[in] args A void pointer to a consume_args struct containing 
 *                 the necessary arguments.
*/
void *consume(void *args)
{
	size_t i;
	consume_args *c_args = (consume_args*) args;
	for(i = 0; i < c_args->num; i++)
		c_args->consumer->consume();
	pthread_exit(NULL);
}
