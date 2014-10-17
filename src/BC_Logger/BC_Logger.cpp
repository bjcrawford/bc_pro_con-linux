/* File: BC_Logger.cpp
   Author: Brett Crawford
   Date: 2014-
   Prof: Kwatny
   TAs: Liang and Casey
   Course: CIS 3207, Sec 2
   Description:
*/

using namespace std;

#include "BC_Logger.hpp"

BC_Logger::BC_Logger(const char log_file[])
{
	lfp = fopen(log_file, "a");
	pthread_mutex_init(&lock, NULL);
}

BC_Logger::~BC_Logger()
{
	fclose(lfp);
	pthread_mutex_destroy(&lock);
}

int BC_Logger::log_event(const char event[])
{
	/* String to hold date and time */
	char *ts = (char*) calloc(23, sizeof(char));
	/* String to hold microseconds */
	char *ms = (char*) calloc(7, sizeof(char));
	/* String to hold date, time, microseconds, and event */
	char *ls = (char*) calloc(30 + strlen(event) + 1, sizeof(char));

	/* Get date and time */
	time_t now = time(NULL);
	strftime(ts, 23, "%Y-%m-%d %H:%M:%S:", localtime(&now));

	/* Get microseconds */
	struct timeval tvp;
	gettimeofday(&tvp, NULL);
	sprintf(ms, "%ld", tvp.tv_usec);

	/* Build the string to insert in log file */
	strcpy(ls, ts);
	strcat(ls, ms);
	strcat(ls, " ");
	strcat(ls, event);
	strcat(ls, "\n");

	/* Write event to log file */
	pthread_mutex_lock(&lock);
	fwrite(ls, sizeof(char), strlen(ls), lfp);
	pthread_mutex_unlock(&lock);

	/* Free all dynamically allocated memory */
	free(ts);
	free(ms);
	free(ls);

	// TODO: handle any possible errors
	return 1;
}

