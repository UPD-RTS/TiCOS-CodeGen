#include <arinc653/types.h>
#include <arinc653/time.h>
#include <arinc653/sampling.h>
#include <arinc653/queueing.h>
#include <arinc653/process.h>
#include <arinc653/event.h>
#include <arinc653/buffer.h>
#include "deployment.h"


extern EVENT_ID_TYPE pok_arinc653_events_ids[1];
extern BUFFER_ID_TYPE pok_buffers_ids[1];

extern PROCESS_ID_TYPE arinc_threads[POK_CONFIG_NB_THREADS];

int buffers_values[8] = {10,11,12,13,14,15,16,17};

// Sampling ports ids, msg and msg size
extern SAMPLING_PORT_ID_TYPE SD_3_id;
MESSAGE_ADDR_TYPE SD_3_msg;
MESSAGE_SIZE_TYPE SD_3_msg_size;

// Queuing ports ids, msg and msg size
extern QUEUING_PORT_ID_TYPE QS_2_id;
MESSAGE_ADDR_TYPE QS_2_msg;
MESSAGE_SIZE_TYPE QS_2_msg_size;


void* thr2_1_job ()
{
	RETURN_CODE_TYPE ret;
	while (1)
	{
		printf("Partition n. 2 - Thread n.1\n");

		/***************************************************************/
		/* Message processing code should be placed here               */
		/***************************************************************/

		printf("1: -- do something\n\t\t1 :: Waiting on an event\n\n");
		RESUME (arinc_threads[2], &(ret));
		PERIODIC_WAIT (&(ret));
	}
}

void* thr2_1bis_job()
{
	int flag = 0;
	RETURN_CODE_TYPE ret;
	SYSTEM_TIME_TYPE time = APERIODIC_PROCESS;
	printf("	Partition n. 2 - Thread n.2\n");
			printf ("\t2: Sporadic (wait evt)\n");
	while (1)
	{
		if (flag){
			printf ("\t2: Th self-suspending.........zzzz\n");
		} else {
			flag=1;
		}
		SUSPEND_SELF (time, &ret);
		printf ("\t2: Resume - Going to WAIT_EVENT \n");
		WAIT_EVENT (pok_arinc653_events_ids[0], 0, &(ret));
		printf ("\t2: Event %i is UP\n", pok_arinc653_events_ids[0]);
	}
}

void* thr2_3_job ()
{
	RETURN_CODE_TYPE ret;
	while (1)
	{
		printf("Partition n. 2 - Thread n.3\n");

		/***************************************************************/
		/* Message processing code should be placed here               */
		/***************************************************************/

		printf(" 3: -- do something\n");
		printf(" 3: Setting an event UP\n\n");
		SET_EVENT (pok_arinc653_events_ids[0], &(ret));
		printf(" 3: Event SET -- Resume sporadic\n\n");
		RESUME (arinc_threads[4], &(ret));
		PERIODIC_WAIT (&(ret));
	}
}

void* thr2_3bis_job()
{
	int flag = 0;
	RETURN_CODE_TYPE ret;
	SYSTEM_TIME_TYPE time = APERIODIC_PROCESS;
	printf("\t4: Sporadic (after set evt)\n");
	while (1)
	{
		if (flag){
			printf("\t4: Resume - Continuation after event is set\n\n");
			RESET_EVENT (pok_arinc653_events_ids[0], &(ret));
			printf("\t4: Event %i is DOWN [%i]\n", pok_arinc653_events_ids[0], ret);
			printf("\t4: Th self-suspending.........zzzz\n");
		} else {
			flag=1;
		}
		SUSPEND_SELF (time, &ret);
	}
}

void* thr2_5_job ()
{
	RETURN_CODE_TYPE ret;
	VALIDITY_TYPE valid;
	while (1)
	{
		printf("Partition n. 2 - Thread n.5\n");

		/***************************************************************/
		/* Message processing code should be placed here               */
		/***************************************************************/

		READ_SAMPLING_MESSAGE (SD_3_id, SD_3_msg, &SD_3_msg_size, &(valid), &(ret));
		if (ret == 0){
			printf(" READ message: 0x%x from port 4, message size: %d, ret:%i\n",((unsigned char *)SD_3_msg)[0], SD_3_msg_size, ret);
		}else{
			printf("	Port empty or READ error:%d\n\n", ret);
		}

		printf("5: -- do something\n\t\t5 :: Resuming Sender\n\n");
		RESUME (arinc_threads[6], &(ret));
		PERIODIC_WAIT (&(ret));
	}
}

void* thr2_5bis_job()
{
	int flag = 0;
	RETURN_CODE_TYPE ret;
	SYSTEM_TIME_TYPE time = APERIODIC_PROCESS;

	MESSAGE_ADDR_TYPE msg_to_send;
	MESSAGE_SIZE_TYPE msg_to_send_size;
	int j = 0;
	printf ("\t6: Sender sporadic\n");
	while (1)
	{
		if (flag)
		{
			printf ("\t6: Sender Th self-suspending.........zzzz\n");
		} else {
			flag=1;
		}
		SUSPEND_SELF (time, &ret);
		printf ("\t6: --> Resume - Sender sporadic \n");
		msg_to_send_size = sizeof(buffers_values[j]);
		msg_to_send = &(buffers_values[j]);
		SEND_BUFFER(pok_buffers_ids[0],msg_to_send, msg_to_send_size,10000,&(ret));
		printf ("\t6: --> Sent: %i to BUFFER %i, return code: [%i]\n", buffers_values[j], pok_buffers_ids[0], ret);
		j = (j+1) % 8;
	}
}

void* thr2_7_job ()
{
	RETURN_CODE_TYPE ret;
	unsigned char val3 = 3;
	QS_2_msg = &val3;
	QS_2_msg_size = sizeof(unsigned char);


	while (1)
	{
		printf("Partition n. 2 - Thread n.7\n");

		/***************************************************************/
		/* Message processing code should be placed here               */
		/***************************************************************/

		SEND_QUEUING_MESSAGE (QS_2_id, QS_2_msg, QS_2_msg_size, 0, &(ret));
		printf("  SENT message 0x%x to port 3, size: %d, ret: %d\n", ((unsigned char *)QS_2_msg)[0], QS_2_msg_size, ret);

		printf("7: -- do something\n\t\t7 :: Resuming Receiver\n\n");
		RESUME (arinc_threads[8], &(ret));
		PERIODIC_WAIT (&(ret));
	}
}

void* thr2_7bis_job()
{
	int flag = 0;
	RETURN_CODE_TYPE ret;
	SYSTEM_TIME_TYPE time = APERIODIC_PROCESS;
	int tmp;
	MESSAGE_ADDR_TYPE msg_to_receive = &tmp;
	MESSAGE_SIZE_TYPE msg_to_receive_size;
	printf ("\t8 --> Receiver sporadic sporadic\n");
	while (1)
	{
		if (flag)
		{
			printf("\t8: --> Receiver self-suspending.........zzzz\n\n");
		} else {
			flag=1;
		}
		SUSPEND_SELF (time, &ret);
		printf("\t8: --> resume - Receiver sporadic\n");
		RECEIVE_BUFFER (pok_buffers_ids[0], 10000, msg_to_receive, &msg_to_receive_size, &(ret));
		printf ("\t8: --> Received: %i from BUFFER %i, (size: %i) return code: [%i]\n", *((int*)msg_to_receive), pok_buffers_ids[0], msg_to_receive_size, ret);
	}
}


