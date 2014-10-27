/**
 * @file BC_Logger.cpp 
 * @author Brett Crawford
 * @brief A shared, event logging class
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-10-18
 *  Description: A shared, event logging class. This class is written for 
 *  use with linux.
*/

using namespace std;

#include "BC_Logger.hpp"

/**
 * Constructs a shared, event logger object. 
 * 
 * @param[in] log_file The name of the log file in which to record events
*/
BC_Logger::BC_Logger(const char log_file[])
{
	lfp = fopen(log_file, "a");
	pthread_mutex_init(&lock, NULL);
}

/**
 * Destroys a shared, event logger object. 
*/
BC_Logger::~BC_Logger()
{
	fclose(lfp);
	pthread_mutex_destroy(&lock);
}

/**
 * Records an event within the log. The event can be at most 62 characters.
 *
 * @param[in] event The event to be logged
 * @param Returns 1 on success, otherwise 0
*/
int BC_Logger::log_event(const char event[])
{
	/* String to hold date and time (date/time string) */
	char *dts = (char*) calloc(21, sizeof(char));
	/* String to hold microseconds */
	char *ms = (char*) calloc(7, sizeof(char));
	/* String to hold date, time, microseconds, and event  (full string)*/
	int fs_size = 80;
	char *fs = (char*) calloc(fs_size, sizeof(char));

	/* Get date and time */
	time_t now = time(NULL);
	strftime(dts, 21, "%Y-%m-%d %H:%M:%S:", localtime(&now));

	/* Get microseconds */
	struct timeval tvp;
	gettimeofday(&tvp, NULL);
	snprintf(ms, 7, "%ld", tvp.tv_usec);

	/* Build the string to insert in log file */
	strcpy(fs, dts);
	strcat(fs, ms);
	strcat(fs, " ");
	strcat(fs, event);
	strcat(fs, "\n");

	/* Write event to log file */
	pthread_mutex_lock(&lock);
	/** CRITICAL SECTION ENTRY */
	fwrite(fs, sizeof(char), strlen(fs), lfp);
	/** CRITICAL SECTION EXIT */
	pthread_mutex_unlock(&lock);

	/* Free all dynamically allocated memory */
	free(dts);
	free(ms);
	free(fs);

	// TODO: handle any possible errors
	return 1;
}

