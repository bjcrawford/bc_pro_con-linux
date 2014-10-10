/* File: BC_Logger.cpp
   Author: Brett Crawford
   Date: 2014-
   Prof: Kwatny
   TAs: Liang and Casey
   Course: CIS 3207, Sec 2
   Description:
*/
#include <iostream>
using namespace std;

#include "BC_Logger.h"

BC_Logger::BC_Logger(const char log_file[])
{
	if(lfp == NULL)
	{
		lfp = fopen(log_file, "a");
	}
}

BC_Logger::~BC_Logger()
{
	if(lfp != NULL)
	{
		fclose(lfp);
	}
}

int BC_Logger::log_event(const char event[])
{
	char *ts = (char*) calloc(25, sizeof(char));
	char *ls = (char*) calloc(25 + strlen(event) + 1, sizeof(char));
	time_t now = time(NULL);

	strftime(ts, 25, "%Y-%m-%d %I:%M:%S %p: ", localtime(&now));

	strcpy(ls, ts);
	strcat(ls, event);
	strcat(ls, "\n");

	fwrite(ls, sizeof(char), strlen(ls), lfp);

	free(ts);
	free(ls);

	return 1;
}

