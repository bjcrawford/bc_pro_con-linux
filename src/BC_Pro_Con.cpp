/* File: BC_Pro_Con.cpp
   Author: Brett Crawford
   Date: 2014-
   Prof: Kwatny
   TAs: Liang and Casey
   Course: CIS 3207, Sec 2
   Description:
*/
#include "BC_Logger/BC_Logger.h"
#include <iostream>

using namespace std;

void logger_test();

int main(int argc, char **argv)
{
	logger_test();
	return EXIT_SUCCESS;
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
