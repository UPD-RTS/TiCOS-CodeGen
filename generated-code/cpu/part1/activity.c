#include <arinc653/types.h>
#include <arinc653/time.h>
#include <arinc653/sampling.h>
#include <arinc653/queueing.h>
#include <arinc653/process.h>
#include <arinc653/blackboard.h>
#include "deployment.h"


extern BLACKBOARD_ID_TYPE pok_blackboards_ids[1];

extern PROCESS_ID_TYPE arinc_threads[POK_CONFIG_NB_THREADS];

int blackboards_values[8] = {0,1,2,3,4,5,6,7};

// Sampling ports ids, msg and msg size
extern SAMPLING_PORT_ID_TYPE SS_0_id;
MESSAGE_ADDR_TYPE SS_0_msg;
MESSAGE_SIZE_TYPE SS_0_msg_size;

// Queuing ports ids, msg and msg size
extern QUEUING_PORT_ID_TYPE QD_1_id;
MESSAGE_ADDR_TYPE QD_1_msg;
MESSAGE_SIZE_TYPE QD_1_msg_size;


void* thr1_1_job ()
{
	RETURN_CODE_TYPE ret;
	unsigned char val1 = 1;
	SS_0_msg = &val1;
	SS_0_msg_size = sizeof(unsigned char);


	MESSAGE_ADDR_TYPE msg_to_display;
	MESSAGE_SIZE_TYPE msg_to_display_size;
	msg_to_display_size = sizeof (int);
	int j = 0;
	while (1)
	{
		printf("Partition n. 1 - Thread n.1\n");

		/***************************************************************/
		/* Message processing code should be placed here               */
		/***************************************************************/

		WRITE_SAMPLING_MESSAGE (SS_0_id, SS_0_msg, SS_0_msg_size, &(ret));
		printf("  WRITTEN message 0x%x  to port 1, size: %d, ret: %d\n", ((unsigned char *)SS_0_msg)[0], SS_0_msg_size, ret);

		printf("1 :: Display\n");
		msg_to_display = &(blackboards_values[j]);
		DISPLAY_BLACKBOARD (pok_blackboards_ids[0], msg_to_display, msg_to_display_size, &(ret));
		printf("1: --> Displayed: %i in BLACKBOARD %i, size: %i ret: [%i]\n", *((int*)msg_to_display), pok_blackboards_ids[0], msg_to_display_size, ret);
		j = (j+1) % 8;
		printf("1: --> Resume sporadic\n\n");
		RESUME (arinc_threads[2], &(ret));
		PERIODIC_WAIT (&(ret));
	}
}

void* thr1_1bis_job()
{
	int flag = 0;
	RETURN_CODE_TYPE ret;
	SYSTEM_TIME_TYPE time = APERIODIC_PROCESS;
	printf("\t2 :: Display sporadic\n");
	while (1)
	{
		if (flag)
		{
			printf("\t2: --> Display Th self-suspending.........zzzz\n\n");
		} else {
			flag=1;
		}
		SUSPEND_SELF (time, &ret);
	}
}

void* thr1_3_job ()
{
	RETURN_CODE_TYPE ret;
	while (1)
	{
		printf("Partition n. 1 - Thread n.3\n");

		/***************************************************************/
		/* Message processing code should be placed here               */
		/***************************************************************/

		RECEIVE_QUEUING_MESSAGE (QD_1_id, 0, QD_1_msg, &QD_1_msg_size, &(ret));
		if (ret == 0){
			printf("  RECEIVED message   0x%x  from port 2, size %d, ret: %i\n",((unsigned char *)QD_1_msg)[0], QD_1_msg_size, ret);
		}else{
			printf("	Port empty or RECEIVE error: %d\n\n", ret);
		}

		printf("3: -- do something\n\t\t3 :: Resuming Reader\n\n");
		RESUME (arinc_threads[4], &(ret));
		PERIODIC_WAIT (&(ret));
	}
}

void* thr1_3bis_job()
{
	int flag = 0;
	RETURN_CODE_TYPE ret;
	SYSTEM_TIME_TYPE time = APERIODIC_PROCESS;
	MESSAGE_ADDR_TYPE msg_to_read;
	MESSAGE_SIZE_TYPE msg_to_read_size;
	int output_value;
	msg_to_read = &output_value;
	printf("	Partition n. 1 - Thread n.4\n");
	printf ("\t4: Read sporadic\n");
	while (1)
	{
		if (flag)
		{
			printf ("\t4: Read Th self-suspending.........zzzz\n");
		} else {
			flag=1;
		}
		SUSPEND_SELF (time, &ret);
		printf ("\t4: --> Resume - Read sporadic \n");
		READ_BLACKBOARD (pok_blackboards_ids[0], 2000, msg_to_read, &msg_to_read_size, &(ret));
		printf ("\t4: --> Sporadic - Read: %i in BLACKBOARD %i, size: %i, ret: [%i]\n", *((int*)msg_to_read), pok_blackboards_ids[0], msg_to_read_size, ret);
	}
}

void* thr1_5_job ()
{
	RETURN_CODE_TYPE ret;
	while (1)
	{
		printf("Partition n. 1 - Thread n.5\n");

		/***************************************************************/
		/* Message processing code should be placed here               */
		/***************************************************************/

		PERIODIC_WAIT (&(ret));
	}
}

void* thr1_6_job ()
{
	RETURN_CODE_TYPE ret;
	while (1)
	{
		printf("Partition n. 1 - Thread n.6\n");

		/***************************************************************/
		/* Message processing code should be placed here               */
		/***************************************************************/

		PERIODIC_WAIT (&(ret));
	}
}


