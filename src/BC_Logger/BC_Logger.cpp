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

/**
 * Event can be at most 62 char
 *
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
	sprintf_s(ms, 7, "%ld", tvp.tv_usec);

	/* Build the string to insert in log file */
	strcpy_s(fs, fs_size, dts);
	strcat_s(fs, fs_size, ms);
	strcat_s(fs, fs_size, " ");
	strcat_s(fs, fs_size, event);
	strcat_s(fs, fs_size, "\n");

	/* Write event to log file */
	pthread_mutex_lock(&lock);
	fwrite(fs, sizeof(char), strlen(fs), lfp);
	pthread_mutex_unlock(&lock);

	/* Free all dynamically allocated memory */
	free(dts);
	free(ms);
	free(fs);

	// TODO: handle any possible errors
	return 1;
}

