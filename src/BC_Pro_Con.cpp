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

int main(int argc, char **argv)
{
	logger_test();

	return EXIT_SUCCESS;
}

void logger_test()
{
	BC_Logger *my_log = new BC_Logger("log.txt");
	my_log->log_event("Log this");
	delete my_log;
}

