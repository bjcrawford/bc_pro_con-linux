/* File: BC_Producer.h
   Author: Brett Crawford
   Date: 2014-10-18
   Prof: Kwatny
   TAs: Liang and Casey
   Course: CIS 3207, Sec 2
   Description: The header file for the producer class.
*/

#ifndef BC_PRODUCER_H_
#define BC_PRODUCER_H_

#include <stdlib.h>
#include <time.h>

#include "../BC_Buffer/BC_Buffer.hpp"
#include "../BC_Logger/BC_Logger.hpp"

class BC_Producer
{
	private:
		int id;              /**< An identifier for the producer */
		BC_Buffer *buffer;   /**< A pointer to the shared buffer */
		BC_Logger *logger;   /**< A pointer to the shared logger */

	public:
		BC_Producer(int id, BC_Buffer*, BC_Logger*);
		virtual ~BC_Producer();

		void produce();
};

#endif /* BC_PRODUCER_H_ */
