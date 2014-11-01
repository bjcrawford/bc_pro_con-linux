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
#include <string>
#include <unistd.h>

/** A struct to hold the arguments to be passed to the produce function */
typedef struct 
{
	size_t num;             /**< The number of productions to make */
	size_t delay;           /**< The delay (microsecs) between productions */
	BC_Producer *producer;  /**< A pointer to the producer object */
} produce_args;

/** A struct to hold the arguments to be passed to the consume function */
typedef struct 
{
	size_t num;             /**< The number of consumptions to make */
	size_t delay;           /**< The delay (microsecs) between consumptions */
	BC_Consumer *consumer;  /**< A pointer to the consumer object */
} consume_args;

/** Function declarations */
produce_args* p_args_factory(size_t, size_t, BC_Producer*);
consume_args* c_args_factory(size_t, size_t, BC_Consumer*);
void *produce(void*);
void *consume(void*);

/**
 * The main function of the solution.
*/
int main(int argc, char **argv)
{
	size_t i;                /**< Iteration variable */
	size_t buffer_size;      /**< Size of the buffer */
	size_t visual;           /**< A flag to enable buffer visualization */
	size_t num_producers;    /**< Number of producers */
	size_t num_consumers;    /**< Number of consumers */
	size_t num_productions;  /**< Number of productions each producer makes */
	size_t num_consumptions; /**< Number of consumptions each consumer makes */
	size_t pro_delay;        /**< Delay (microsecs) between productions */
	size_t con_delay;        /**< Delay (microsecs) between consumptions */
	std::string log_file;    /**< Name of log file to be used */
	BC_Logger *logger;       /**< Pointer to the shared logger object */
	BC_Buffer *buffer;       /**< Pointer to the shared buffer object */
	BC_Producer **producer;  /**< Array of pointers to producer objects */
	BC_Consumer **consumer;  /**< Array of pointers to consumer objects */
	pthread_t *pro_threads;  /**< Array of producer threads */
	pthread_t *con_threads;  /**< Array of consumer threads */

	std::cout << "Main thread start\n\n";
	std::cout << "Welcome to the Producer-Consumer ";
	std::cout << "solution by Brett Crawford\n\n";
	std::cout << "Notes: Buffer sizes of 10 or fewer ";
	std::cout << "will be visualized in the log\n\n";
	std::cout << "Please enter a size for the buffer: ";
	std::cin >> buffer_size;
	std::cout << "\nPlease enter the number of producers: ";
	std::cin >> num_producers;
	std::cout << "\nPlease enter the number of consumers: ";
	std::cin >> num_consumers;
	std::cout << "\nPlease enter the number of productions ";
	std::cout << "for each producer: ";
	std::cin >> num_productions;
	std::cout << "\nPlease enter the number of consumptions ";
	std::cout << "for each consumer: ";
	std::cin >> num_consumptions;
	std::cout << "\nPlease enter the delay in microseconds ";
	std::cout << "between productions: ";
	std::cin >> pro_delay;
	std::cout << "\nPlease enter the delay in microseconds ";
	std::cout << "between consumptions: ";
	std::cin >> con_delay;
	std::cout << "\nPlease enter a name for the log file: ";
	std::cin >> log_file;

	visual = (buffer_size <= 10) ? 1 : 0;

	/** Instantiate logger and buffer objects */
	logger = new BC_Logger(log_file.c_str());
	buffer = new BC_Buffer(buffer_size, logger, visual);

	/** Instantiate arrays of producers and consumers */
	producer = new BC_Producer*[num_producers];
	consumer = new BC_Consumer*[num_consumers];

	/** Allocate space for producer and consumer threads arrays */
	pro_threads = (pthread_t*) calloc(num_producers, sizeof(pthread_t));
	con_threads = (pthread_t*) calloc(num_consumers, sizeof(pthread_t));

	/** Instantiate individual producers */
	for(i = 0; i < num_producers; i++)
		producer[i] = new BC_Producer(i, buffer, logger);

	/** Instantiate individual consumers */
	for(i = 0; i < num_consumers; i++)
		consumer[i] = new BC_Consumer(i, buffer, logger);

	/** Create producer args and producer threads */
	for(i = 0; i < num_producers; i++)
	{
		// Create array of pointers to producer args, memory leak
		produce_args *p_args = p_args_factory(num_productions, pro_delay, producer[i]);
		pthread_create(&(pro_threads[i]), 
			           NULL, 
			           (void* (*)(void*)) &produce, 
			           p_args);
		std::cout << "Producer " << (int) i << " thread created\n";
	}

	/** Create consumer args and consumer threads */
	for(i = 0; i < num_consumers; i++)
	{
		// Create array of pointers to consumer args, memory leak
		consume_args *c_args = c_args_factory(num_consumptions, con_delay, consumer[i]);
		pthread_create(&(con_threads[i]), 
			           NULL, 
			           (void* (*)(void*)) &consume, 
			           c_args);
		std::cout << "Consumer " << (int) i << " thread created\n";
	}

	/** Ensure producer threads finished */
	for(i = 0; i < num_producers; i++)
	{
		pthread_join(pro_threads[i], NULL);
		std::cout << "Producer " << (int) i << " thread joined\n";
	}

	/** Ensure consumer threads finished */
	for(i = 0; i < num_consumers; i++)
	{
		pthread_join(con_threads[i], NULL);
		std::cout << "Consumer " << (int) i << " thread joined\n";
	}

	std::cout << "All thread finished\n";

	/** Clean up */
	delete(logger);
	delete(buffer);
	for(i = 0; i < num_producers; i++)
		delete(producer[i]);
	for(i = 0; i < num_consumers; i++)
		delete(consumer[i]);
	delete(producer);
	delete(consumer);
	free(pro_threads);
	free(con_threads);

	std::cout << "Log results saved in file " << log_file << "\n";

	std::cout << "Main thread finished\n";

	return EXIT_SUCCESS;
}

/**
 * Creates and returns a produce_args struct filled with the given values.
 * The caller is responsible for freeing the allocated structure.
 *
 * @return A pointer to a dynamically allocated produce_args struct
*/
produce_args* p_args_factory(size_t num, size_t delay, BC_Producer* producer)
{
	produce_args *p_args = (produce_args*) calloc(1, sizeof(produce_args));
	p_args->num = num;
	p_args->delay = delay;
	p_args->producer = producer;

	return p_args;
}

/**
 * Creates and returns a consume_args struct filled with the given values.
 * The caller is responsible for freeing the allocated structure.
 *
 * @return A pointer to a dynamically allocated consume_args struct
*/
consume_args* c_args_factory(size_t num, size_t delay, BC_Consumer* consumer)
{
	consume_args *c_args = (consume_args*) calloc(1, sizeof(consume_args));
	c_args->num = num;
	c_args->delay = delay;
	c_args->consumer = consumer;

	return c_args;
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
	{
		usleep(p_args->delay);
		p_args->producer->produce();
	}
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
	{
		usleep(c_args->delay);
		c_args->consumer->consume();
	}
	pthread_exit(NULL);
}
