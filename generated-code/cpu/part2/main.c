#include "activity.h"
#include <arinc653/types.h>
#include <arinc653/process.h>
#include <arinc653/partition.h>
#include "deployment.h"
#include <arinc653/sampling.h>
#include <arinc653/queueing.h>
#include <arinc653/event.h>
#include <arinc653/buffer.h>


PROCESS_ID_TYPE arinc_threads[POK_CONFIG_NB_THREADS];

extern EVENT_ID_TYPE pok_arinc653_events_ids[1];
extern char* pok_arinc653_events_names[POK_CONFIG_ARINC653_NB_EVENTS];
extern BUFFER_ID_TYPE pok_buffers_ids[1];
extern char* pok_buffers_names[POK_CONFIG_NB_BUFFERS];
// Sampling ports ids
SAMPLING_PORT_ID_TYPE SD_3_id;
// Queuing ports ids
QUEUING_PORT_ID_TYPE QS_2_id;

int main ()
{

	PROCESS_ATTRIBUTE_TYPE tattr;
	RETURN_CODE_TYPE ret;
	uint8_t buffers_max_message_size = 256;

	printf("	part2 - Main thread\n");
	// Sampling ports creation 
	CREATE_SAMPLING_PORT ("port_04", POK_CONFIG_MAX_MESSAGE_SIZE, DESTINATION, 2000, &(SD_3_id), &(ret));
	printf("	Sampling port created! id: 4, name: port_04, core id:%d, ret:%d\n", SD_3_id, ret);

	// Queuing ports creation 
	CREATE_QUEUING_PORT ("port_03", POK_CONFIG_MAX_MESSAGE_SIZE,  POK_CONFIG_PART_MAX_QUEUING_MESSAGES, SOURCE, 0, &(QS_2_id), &(ret));
	printf("	Queueing port created! id: 3, name: port_03, core id:%d, ret:%d\n", QS_2_id, ret);


	CREATE_EVENT (pok_arinc653_events_names[0], &(pok_arinc653_events_ids[0]), &(ret));
	printf("	Event %s created! EVENT ID: %u, ret: %d \n", pok_arinc653_events_names[0], pok_arinc653_events_ids[0], ret);


	CREATE_BUFFER (pok_buffers_names[0], buffers_max_message_size, POK_CONFIG_MAX_BUFFERS_MESSAGES, FIFO,  &(pok_buffers_ids[0]), &(ret));
	printf("	Buffer %s created! BUFFER ID: %u, ret: %d \n", pok_buffers_names[0], pok_buffers_ids[0], ret);

	tattr.PERIOD = 200;
	tattr.BASE_PRIORITY = 9;
	tattr.ENTRY_POINT = thr2_1_job;
	CREATE_PROCESS (&(tattr), &(arinc_threads[1]), &(ret));

	tattr.PERIOD = APERIODIC_PROCESS;
	tattr.BASE_PRIORITY = 8;
	tattr.ENTRY_POINT = thr2_1bis_job;
	CREATE_PROCESS (&(tattr), &(arinc_threads[2]), &(ret));

	tattr.PERIOD = 200;
	tattr.BASE_PRIORITY = 7;
	tattr.ENTRY_POINT = thr2_3_job;
	CREATE_PROCESS (&(tattr), &(arinc_threads[3]), &(ret));

	tattr.PERIOD = APERIODIC_PROCESS;
	tattr.BASE_PRIORITY = 6;
	tattr.ENTRY_POINT = thr2_3bis_job;
	CREATE_PROCESS (&(tattr), &(arinc_threads[4]), &(ret));

	tattr.PERIOD = 200;
	tattr.BASE_PRIORITY = 17;
	tattr.ENTRY_POINT = thr2_5_job;
	CREATE_PROCESS (&(tattr), &(arinc_threads[5]), &(ret));

	tattr.PERIOD = APERIODIC_PROCESS;
	tattr.BASE_PRIORITY = 16;
	tattr.ENTRY_POINT = thr2_5bis_job;
	CREATE_PROCESS (&(tattr), &(arinc_threads[6]), &(ret));

	tattr.PERIOD = 200;
	tattr.BASE_PRIORITY = 15;
	tattr.ENTRY_POINT = thr2_7_job;
	CREATE_PROCESS (&(tattr), &(arinc_threads[7]), &(ret));

	tattr.PERIOD = APERIODIC_PROCESS;
	tattr.BASE_PRIORITY = 14;
	tattr.ENTRY_POINT = thr2_7bis_job;
	CREATE_PROCESS (&(tattr), &(arinc_threads[8]), &(ret));


	START (arinc_threads[1],&(ret));
	START (arinc_threads[2],&(ret));
	START (arinc_threads[3],&(ret));
	START (arinc_threads[4],&(ret));
	START (arinc_threads[5],&(ret));
	START (arinc_threads[6],&(ret));
	START (arinc_threads[7],&(ret));
	START (arinc_threads[8],&(ret));

	SET_PARTITION_MODE (NORMAL, &(ret));
	return 0;
}
