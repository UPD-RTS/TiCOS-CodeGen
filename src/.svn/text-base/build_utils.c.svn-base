/*
 *  Copyright (c) 2012  University of Padua, Department of Mathematics
 *
 *  This file is free software: you may copy, redistribute and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This file is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "arinc653-xml-conf.h"

extern int nb_events_blackb_buffers;
extern int major_frame;
extern int nb_blackboards;
extern int nb_events;
extern int nb_asynch_events;

unsigned long time_capacity = DEFAULT_TIME_CAPACITY_INIT_VALUE;


void init_kernel_macros (){
	
	char *one = "1";
	macro_enabled[POK_NEEDS_ARINC653] = TRUE;
	strcpy(macro_values[POK_NEEDS_ARINC653], one);
	
	macro_enabled[POK_NEEDS_THREADS] = TRUE;
	strcpy(macro_values[POK_NEEDS_THREADS], one);
	
	macro_enabled[POK_NEEDS_PARTITIONS] = TRUE;
	strcpy(macro_values[POK_NEEDS_PARTITIONS], one);
	
	macro_enabled[POK_SKIP_LOADER] = DEFAULT_POK_SKIP_LOADER;
	if (macro_enabled[POK_SKIP_LOADER]){
		strcpy(macro_values[POK_SKIP_LOADER], one);
	}
	
//	macro_enabled[POK_CONFIG_PARTITIONS_LOADADDR] = TRUE;
//	strcpy(macro_values[POK_CONFIG_PARTITIONS_LOADADDR], DEFAULT_POK_CONFIG_PARTITIONS_LOADADDR);
	
	macro_enabled[POK_DISABLE_LOADER_DATA_INIT] = TRUE;
	strcpy(macro_values[POK_DISABLE_LOADER_DATA_INIT], one);
	
	macro_enabled[POK_PREFILL_PTE] = TRUE;
	strcpy(macro_values[POK_PREFILL_PTE], one);
	
	macro_enabled[POK_CONFIG_DEFINE_BASE_VADDR] = TRUE;
	strcpy(macro_values[POK_CONFIG_DEFINE_BASE_VADDR], one);
	
	macro_enabled[POK_NEEDS_CONSOLE] = TRUE;
	strcpy(macro_values[POK_NEEDS_CONSOLE], one);
	
	macro_enabled[POK_NEEDS_SCHED] = TRUE;
	strcpy(macro_values[POK_NEEDS_SCHED], one);
	
	if (macro_enabled[POK_NEEDS_SCHED_O1]){
		macro_enabled[POK_CONFIG_NB_ASYNCH_EVENTS] = TRUE;	
		strcpy(macro_values[POK_CONFIG_NB_ASYNCH_EVENTS], number_to_string[nb_asynch_events]);
	}

	if (nb_events_blackb_buffers>0){
		macro_enabled[POK_NEEDS_LOCKOBJECTS] = TRUE;
		strcpy(macro_values[POK_NEEDS_LOCKOBJECTS], one);
		macro_enabled[POK_CONFIG_NB_LOCKOBJECTS] = TRUE;
		strcpy(macro_values[POK_CONFIG_NB_LOCKOBJECTS], number_to_string[nb_events_blackb_buffers]);
//		macro_enabled[POK_NEEDS_THREAD_SUSPEND] = TRUE;
//		strcpy(macro_values[POK_NEEDS_THREAD_SUSPEND], one);
	}

	if (macro_enabled[POK_NEEDS_SCHED_O1_SPLIT]){

		macro_enabled[POK_CONFIG_NB_SPORADIC_THREADS] 		= TRUE;	
		strcpy(macro_values[POK_CONFIG_NB_SPORADIC_THREADS], number_to_string[nb_of_sporadic_threads]);

	}


}

void build_main_c_pre_wait_thread (int partition_id, int thread_nb) {

	char tmp_period[5];
	//char thread.entry_point[40];
	thread_t *thread = &(partitions[partition_id].threads[thread_nb-1]);
 
//	printf("thread.priority: %d\n",thread.priority);
//	printf("priority: %d\n",priority);

	debug_printf("In build_main_c_pre_wait_thread id %d, thread_nb %d\n",partition_id, thread->thread_nb);
	sprintf(tmp_period,"%d",(partitions[partition_id].period *10));
	strcat(partitions[partition_id].main_c,"\ttattr.PERIOD = ");
	strcat(partitions[partition_id].main_c,tmp_period);
	strcat(partitions[partition_id].main_c,";\n");

	strcat(partitions[partition_id].main_c, "\ttattr.BASE_PRIORITY = ");
	strcat(partitions[partition_id].main_c, number_to_string[thread->priority]);
	strcat(partitions[partition_id].main_c, ";\n");
	
	strcat(partitions[partition_id].main_c, "\ttattr.TIME_CAPACITY = ");
	char tmp_time_capacity[10];
	sprintf(tmp_time_capacity,"%lu",time_capacity);
	strcat(partitions[partition_id].main_c, tmp_time_capacity);
	strcat(partitions[partition_id].main_c, ";\n");
	
	strcpy(thread->entry_point,"thr");
	strcat(thread->entry_point,number_to_string[partition_id+1]);
	strcat(thread->entry_point,"_");
	strcat(thread->entry_point,number_to_string[thread->thread_nb]);
	strcat(thread->entry_point,"_pre_wait");
	strcat(thread->entry_point,number_to_string[thread->event_nb+1]);
	strcat(thread->entry_point,"_job");
	strcat(partitions[partition_id].main_c,"\ttattr.ENTRY_POINT = ");
	strcat(partitions[partition_id].main_c,thread->entry_point);
	strcat(partitions[partition_id].main_c,";\n");

	strcat(partitions[partition_id].main_c,"\tCREATE_PROCESS (&(tattr), &(arinc_threads[");
	strcat(partitions[partition_id].main_c,number_to_string[thread->thread_nb]);
	strcat(partitions[partition_id].main_c,"]), &(ret));\n\n");
}

void build_main_c_succ_wait_thread (int partition_id, int thread_nb) {

	char tmp_period[5];
	//char thread.entry_point[40];
	thread_t *thread = &(partitions[partition_id].threads[thread_nb-1]);

	debug_printf("In build_main_c_succ_wait_thread id %d, thread_nb %d\n",partition_id, thread->thread_nb);
	sprintf(tmp_period,"%d",(partitions[partition_id].period *10));
	strcat(partitions[partition_id].main_c,"\ttattr.PERIOD = APERIODIC_PROCESS;\n");
	//strcat(partitions[partition_id].main_c,tmp_period);
	//strcat(partitions[partition_id].main_c,";\n");
	
	
	strcat(partitions[partition_id].main_c, "\ttattr.BASE_PRIORITY = ");
	strcat(partitions[partition_id].main_c, number_to_string[thread->priority]);
	strcat(partitions[partition_id].main_c, ";\n");
	
	strcat(partitions[partition_id].main_c, "\ttattr.TIME_CAPACITY = ");
	strcat(partitions[partition_id].main_c, number_to_string[DEFAULT_TIME_CAPACITY_SUCC_THREAD]);
	strcat(partitions[partition_id].main_c, ";\n");
	
	strcpy(thread->entry_point,"thr");
	strcat(thread->entry_point,number_to_string[partition_id+1]);
	strcat(thread->entry_point,"_");
	strcat(thread->entry_point,number_to_string[thread->thread_nb]);
	strcat(thread->entry_point,"_succ_wait");
	strcat(thread->entry_point,number_to_string[thread->event_nb+1]);
	strcat(thread->entry_point,"_job");
	strcat(partitions[partition_id].main_c,"\ttattr.ENTRY_POINT = ");
	strcat(partitions[partition_id].main_c,thread->entry_point);
	strcat(partitions[partition_id].main_c,";\n");

	strcat(partitions[partition_id].main_c,"\tCREATE_PROCESS (&(tattr), &(arinc_threads[");
	strcat(partitions[partition_id].main_c,number_to_string[thread->thread_nb]);
	strcat(partitions[partition_id].main_c,"]), &(ret));\n\n");
}


void build_main_c_set_thread (int partition_id, int thread_nb) {

	char tmp_period[5];

	thread_t *thread = &(partitions[partition_id].threads[thread_nb-1]);

	debug_printf("In build_main_c_set_thread id %d, thread_nb %d\n",partition_id, thread->thread_nb);
	sprintf(tmp_period,"%d",(partitions[partition_id].period *10));
	strcat(partitions[partition_id].main_c,"\ttattr.PERIOD = ");
	strcat(partitions[partition_id].main_c,tmp_period);
	strcat(partitions[partition_id].main_c,";\n");

	strcat(partitions[partition_id].main_c, "\ttattr.BASE_PRIORITY = ");
	strcat(partitions[partition_id].main_c, number_to_string[thread->priority]);
	strcat(partitions[partition_id].main_c, ";\n");
	
	strcat(partitions[partition_id].main_c, "\ttattr.TIME_CAPACITY = ");
	char tmp_time_capacity[10];
	sprintf(tmp_time_capacity,"%lu",time_capacity);
	strcat(partitions[partition_id].main_c, tmp_time_capacity);
	time_capacity += 1000;
	strcat(partitions[partition_id].main_c, ";\n");
	
	strcpy(thread->entry_point,"thr");
	strcat(thread->entry_point,number_to_string[partition_id+1]);
	strcat(thread->entry_point,"_");
	strcat(thread->entry_point,number_to_string[thread->thread_nb]);
	strcat(thread->entry_point,"_set");
	strcat(thread->entry_point,number_to_string[thread->event_nb+1]);
	strcat(thread->entry_point,"_job");
	strcat(partitions[partition_id].main_c,"\ttattr.ENTRY_POINT = ");
	strcat(partitions[partition_id].main_c,thread->entry_point);
	strcat(partitions[partition_id].main_c,";\n");

	strcat(partitions[partition_id].main_c,"\tCREATE_PROCESS (&(tattr), &(arinc_threads[");
	strcat(partitions[partition_id].main_c,number_to_string[thread->thread_nb]);
	strcat(partitions[partition_id].main_c,"]), &(ret));\n\n");
}



void build_main_c_normal_thread (int partition_id, int thread_number) {

	char tmp_period[5];

	thread_t *thread = &(partitions[partition_id].threads[thread_number-1]);

	debug_printf("In build_main_c_normal_thread partition id %d, thread_nb %d\n",partition_id, thread->thread_nb);
	sprintf(tmp_period,"%d",(partitions[partition_id].period *10));
	strcat(partitions[partition_id].main_c,"\ttattr.PERIOD = ");
	strcat(partitions[partition_id].main_c,tmp_period);
	strcat(partitions[partition_id].main_c,";\n");

	strcat(partitions[partition_id].main_c,"\ttattr.BASE_PRIORITY = ");
	strcat(partitions[partition_id].main_c,number_to_string[thread->priority]);
	strcat(partitions[partition_id].main_c,";\n");
	
	
	strcpy(thread->entry_point,"thr");
	strcat(thread->entry_point,number_to_string[partition_id+1]);
	strcat(thread->entry_point,"_");
	strcat(thread->entry_point,number_to_string[thread->thread_nb]);
	strcat(thread->entry_point,"_job");
	strcat(partitions[partition_id].main_c,"\ttattr.ENTRY_POINT = ");
	strcat(partitions[partition_id].main_c,thread->entry_point);
	strcat(partitions[partition_id].main_c,";\n");

	strcat(partitions[partition_id].main_c,"\tCREATE_PROCESS (&(tattr), &(arinc_threads[");
	strcat(partitions[partition_id].main_c,number_to_string[thread->thread_nb]);
	strcat(partitions[partition_id].main_c,"]), &(ret));\n\n");
}

void build_main_c_aperiodic_thread (int partition_id, int thread_number) {

	thread_t *thread = &(partitions[partition_id].threads[thread_number-1]);

	char tmp_period[5];
	strcat(partitions[partition_id].main_c,"\ttattr.PERIOD = APERIODIC_PROCESS;\n");
	strcat(partitions[partition_id].main_c,"\ttattr.BASE_PRIORITY = ");
	strcat(partitions[partition_id].main_c,number_to_string[thread->priority]);
	strcat(partitions[partition_id].main_c,";\n");

	strcpy(thread->entry_point,"thr");
	strcat(thread->entry_point,number_to_string[partition_id+1]);
	strcat(thread->entry_point,"_");
	strcat(thread->entry_point,number_to_string[thread->thread_nb-1]);
	strcat(thread->entry_point,"bis_job");
	strcat(partitions[partition_id].main_c,"\ttattr.ENTRY_POINT = ");
	strcat(partitions[partition_id].main_c,thread->entry_point);
	strcat(partitions[partition_id].main_c,";\n");

	strcat(partitions[partition_id].main_c,"\tCREATE_PROCESS (&(tattr), &(arinc_threads[");
	strcat(partitions[partition_id].main_c,number_to_string[thread->thread_nb]);
	strcat(partitions[partition_id].main_c,"]), &(ret));\n\n");
}


void init_periodic_thread(int partition_id, thread_t thread) {
/*
	strcpy(thread.entry_point,"thr");
	strcat(thread.entry_point,number_to_string[partition_id+1]);
	strcat(thread.entry_point,"_");
	strcat(thread.entry_point,number_to_string[thread.thread_nb]);
	strcat(thread.entry_point,"_job");
*/

	debug_printf("init_periodic_thread\n");
	strcat(partitions[partition_id].activity_h,"void* ");
	strcat(partitions[partition_id].activity_h,thread.entry_point);
	strcat(partitions[partition_id].activity_h," ();\n");

	strcat(partitions[partition_id].activity_c,"void* ");
	strcat(partitions[partition_id].activity_c,thread.entry_point);
	strcat(partitions[partition_id].activity_c," ()\n{\n\tRETURN_CODE_TYPE ret;\n");

}

//
void init_source_port_variables  (int partition_id, struct port_list_item_t *tmp_output_ports) {

	strcat(partitions[partition_id].activity_c,"\tunsigned char val");
	strcat(partitions[partition_id].activity_c,number_to_string[tmp_output_ports->port_id + 1]);
	strcat(partitions[partition_id].activity_c," = ");
	strcat(partitions[partition_id].activity_c,number_to_string[tmp_output_ports->port_id + 1]);
	strcat(partitions[partition_id].activity_c,";\n\t");
	strcat(partitions[partition_id].activity_c,ports[tmp_output_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg = &val");
	strcat(partitions[partition_id].activity_c,number_to_string[tmp_output_ports->port_id + 1]);
	strcat(partitions[partition_id].activity_c,";\n\t");
	strcat(partitions[partition_id].activity_c,ports[tmp_output_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg_size = sizeof(unsigned char);\n\n\n");
}

//	while (1){
//		printf("Partition n. 2 - Thread n.1\n");
//		/***************************************************************/
//		/* Message processing code should be placed here               */
//		/***************************************************************/
void while_in_thread(int partition_id, int thread_nb) {
	strcat(partitions[partition_id].activity_c,"\twhile (1)\n\t{\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf(\"Partition n. ");
	strcat(partitions[partition_id].activity_c,number_to_string[partition_id+1]);
	strcat(partitions[partition_id].activity_c," - Thread n.");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_nb]);
	strcat(partitions[partition_id].activity_c,"\\n\");\n");
	strcat(partitions[partition_id].activity_c, MESSAGE_IN_PROCESS);
}

//		WRITE_SAMPLING_MESSAGE (pr1_SS_id, pr1_SS_msg, pr1_SS_msg_size, &(ret));
void write_sampling_message(int partition_id,struct port_list_item_t *tmp_output_ports){

	strcat(partitions[partition_id].activity_c,"\t\tWRITE_SAMPLING_MESSAGE (");
	strcat(partitions[partition_id].activity_c,ports[tmp_output_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_id, ");
	strcat(partitions[partition_id].activity_c,ports[tmp_output_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg, ");
	strcat(partitions[partition_id].activity_c,ports[tmp_output_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg_size, &(ret));\n");

	strcat(partitions[partition_id].activity_c,"\t\tprintf(\"  WRITTEN message 0x%x  to port ");
	strcat(partitions[partition_id].activity_c,number_to_string[ports[tmp_output_ports->port_id].user_port_id]);
	strcat(partitions[partition_id].activity_c,", size: %d, ret: %d\\n\", ((unsigned char *)");
	strcat(partitions[partition_id].activity_c,ports[tmp_output_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg)[0], ");
	strcat(partitions[partition_id].activity_c,ports[tmp_output_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg_size, ret);\n\n");
}


void send_queuing_message(int partition_id, struct port_list_item_t *tmp_output_ports){

	strcat(partitions[partition_id].activity_c,"\t\tSEND_QUEUING_MESSAGE (");
	strcat(partitions[partition_id].activity_c,ports[tmp_output_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_id, ");
	strcat(partitions[partition_id].activity_c,ports[tmp_output_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg, ");
	strcat(partitions[partition_id].activity_c,ports[tmp_output_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg_size, ");
	strcat(partitions[partition_id].activity_c,	DEFAULT_SEND_QUEUING_MESSAGE_TIME_OUT);
	strcat(partitions[partition_id].activity_c,", &(ret));\n");

	strcat(partitions[partition_id].activity_c,"\t\tprintf(\"  SENT message 0x%x to port ");
	strcat(partitions[partition_id].activity_c,number_to_string[ports[tmp_output_ports->port_id].user_port_id]);
 	strcat(partitions[partition_id].activity_c,", size: %d, ret: %d\\n\", ((unsigned char *)");
	strcat(partitions[partition_id].activity_c,ports[tmp_output_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg)[0], ");
	strcat(partitions[partition_id].activity_c,ports[tmp_output_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg_size, ret);\n\n");
}


//		READ_SAMPLING_MESSAGE (pr1_SDa_id, pr1_SDa_msg, &pr1_SDa_msg_size, &(valid), &(ret));
void read_sampling_message(int partition_id, struct port_list_item_t *tmp_input_ports){

	strcat(partitions[partition_id].activity_c,"\t\tREAD_SAMPLING_MESSAGE (");
	strcat(partitions[partition_id].activity_c,ports[tmp_input_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_id, ");
	strcat(partitions[partition_id].activity_c,ports[tmp_input_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg, &");
	strcat(partitions[partition_id].activity_c,ports[tmp_input_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg_size, &(valid), &(ret));\n");

	strcat(partitions[partition_id].activity_c,"\t\tif (ret == 0){\n");
	strcat(partitions[partition_id].activity_c,"\t\t\tprintf(\" READ message: 0x%x from port ");
	strcat(partitions[partition_id].activity_c,number_to_string[ports[tmp_input_ports->port_id].user_port_id]);
	strcat(partitions[partition_id].activity_c,", message size: %d, ret:%i\\n\",");
	strcat(partitions[partition_id].activity_c,"((unsigned char *)");
	strcat(partitions[partition_id].activity_c,ports[tmp_input_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg)[0], ");
	strcat(partitions[partition_id].activity_c,ports[tmp_input_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg_size, ret);\n\t\t}else{\n\t\t\tprintf(\"	Port empty or READ error:%d\\n\\n\", ret);\n\t\t}\n\n");
}

void receive_queuing_message(int partition_id, struct port_list_item_t *tmp_input_ports){
	strcat(partitions[partition_id].activity_c,"\t\tRECEIVE_QUEUING_MESSAGE (");
	strcat(partitions[partition_id].activity_c,ports[tmp_input_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_id, ");
	strcat(partitions[partition_id].activity_c,DEFAULT_RECEIVE_QUEUING_MESSAGE_TIME_OUT);
	strcat(partitions[partition_id].activity_c,", ");
	strcat(partitions[partition_id].activity_c,ports[tmp_input_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg, &");
	strcat(partitions[partition_id].activity_c,ports[tmp_input_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg_size, &(ret));\n");

	strcat(partitions[partition_id].activity_c,"\t\tif (ret == 0){\n");
	strcat(partitions[partition_id].activity_c,"\t\t\tprintf(\"  RECEIVED message   0x%x  from port ");
	strcat(partitions[partition_id].activity_c,number_to_string[ ports[tmp_input_ports->port_id].user_port_id ]);
	strcat(partitions[partition_id].activity_c,", size %d, ret: %i\\n\",");
	strcat(partitions[partition_id].activity_c,"((unsigned char *)");
	strcat(partitions[partition_id].activity_c,ports[tmp_input_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg)[0], ");
	strcat(partitions[partition_id].activity_c,ports[tmp_input_ports->port_id].local_id);
	strcat(partitions[partition_id].activity_c,"_msg_size, ret);\n\t\t}else{\n\t\t\tprintf(\"	Port empty or RECEIVE error: %d\\n\\n\", ret);\n\t\t}\n\n");
}

// build macros and variable definitions related to port preload/postflush in slot
int build_slots_config(){
	
	debug_printf("\n*********** BUILD SLOTS CONFIG **********\n");
	
	macro_enabled[POK_CONFIG_SCHEDULING_SLOTS] 		 = TRUE;
	macro_enabled[POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION]  = TRUE;
	
	char pok_config_scheduling_slots[100];
	strcpy(pok_config_scheduling_slots,"{");

	char pok_config_scheduling_slots_allocation[100];
	strcpy(pok_config_scheduling_slots_allocation,"{");
	
	
	char pok_config_partitions_postwrite_time[100];
	if (macro_enabled[POK_NEEDS_PORTS_SLOT]){
		macro_enabled[POK_CONFIG_PARTITIONS_POSTWRITE_TIME] = TRUE;
		strcpy(pok_config_partitions_postwrite_time,"{");
		strcpy(pok_outputports_to_flush,"uint8_t* ");
		strcat(pok_outputports_to_flush,DEFAULT_OUTPUTPORTS_TO_FLUSH_ARRAYNAME);
		strcat(pok_outputports_to_flush,"[POK_CONFIG_SCHEDULING_NBSLOTS] = {");
		
		strcpy(pok_nb_outputports_to_flush,"uint8_t ");
		strcat(pok_nb_outputports_to_flush,DEFAULT_NB_OUTPUTPORTS_TO_FLUSH_ARRAYNAME);
		strcat(pok_nb_outputports_to_flush,"[POK_CONFIG_SCHEDULING_NBSLOTS] = {");

		strcpy(pok_inputports_to_preload, "uint8_t* ");
		strcat(pok_inputports_to_preload, DEFAULT_INPUT_PORT_TO_PRELOAD_ARRAYNAME);
		strcat(pok_inputports_to_preload, "[POK_CONFIG_SCHEDULING_NBSLOTS] = {");

		strcpy(pok_nb_inputports_to_preload,"uint8_t ");
		strcat(pok_nb_inputports_to_preload,DEFAULT_NB_INPUT_PORT_TO_PRELOAD_ARRAYNAME);
		strcat(pok_nb_inputports_to_preload,"[POK_CONFIG_SCHEDULING_NBSLOTS] = {");
		
		strcpy(slot_pok_outputports_to_flush,"");
		strcpy(slot_pok_inputports_to_preload,"");
	}
		 
		

	int i;
	for ( i=0; i<nb_slots; i++){
		
		if (i>0){
			strcat(pok_config_scheduling_slots,",");
			strcat(pok_config_scheduling_slots_allocation,",");
			if (macro_enabled[POK_NEEDS_PORTS_SLOT]){
				strcat(pok_config_partitions_postwrite_time,",");
				strcat(pok_outputports_to_flush,",");
				strcat(pok_nb_outputports_to_flush,",");
				strcat(pok_nb_inputports_to_preload,",");
				strcat(pok_inputports_to_preload,",");
			}
		}
		
		char tmp_slot_duration[4];		
        sprintf(tmp_slot_duration,"%d",slots_duration[i]);
		strcat(pok_config_scheduling_slots,tmp_slot_duration);
		
		char tmp_partition[4];		
        sprintf(tmp_partition,"%d",slots_partition[i]);
		strcat(pok_config_scheduling_slots_allocation,tmp_partition);
		
		

		if (macro_enabled[POK_NEEDS_PORTS_SLOT]){
			char tmp_post_write_time[3];		
			sprintf(tmp_post_write_time,"%d",postwrite_times[i]);
			strcat(pok_config_partitions_postwrite_time,tmp_post_write_time);
	
			char output_name[100];
			strcpy(output_name,"slot");
			strcat(output_name,number_to_string[i+1]);
			strcat(output_name,"_outputports_to_flush");
			char input_name[100];
			strcpy(input_name,"slot");
			strcat(input_name,number_to_string[i+1]);
			strcat(input_name,"_inputports_to_preload");
			strcat(slot_pok_outputports_to_flush,"uint8_t ");
			strcat(slot_pok_outputports_to_flush,output_name);
			strcat(slot_pok_outputports_to_flush,"[");
			strcat(slot_pok_inputports_to_preload,"uint8_t ");
			strcat(slot_pok_inputports_to_preload,input_name);
			strcat(slot_pok_inputports_to_preload,"[");
			char tmp_outputports[1000];
			strcpy(tmp_outputports,"");
			char tmp_inputports[1000];
			strcpy(tmp_inputports,"");
			struct port_list_item_t *tmp = partitions[slots_partition[i]].ports;
			int j = 0;
			int nb_outputports_to_flush = 0;
			int nb_inputports_to_preload = 0;
			int firstTime_flush = 0;
			int firstTime_preload = 0;
			while (tmp != NULL){
				
				if (ports[tmp->port_id].direction == SOURCE){
					nb_outputports_to_flush++;
					if (firstTime_flush){
						strcat(tmp_outputports,",");
					}
					firstTime_flush = 1;
					strcat(tmp_outputports,ports[tmp->port_id].local_id);
				}else{
					nb_inputports_to_preload++;
					if (firstTime_preload){
						strcat(tmp_inputports,",");
					}
					firstTime_preload =1;
					strcat(tmp_inputports,ports[tmp->port_id].local_id);
				}
				tmp = tmp->next;
				j++;
			}
	
			strcat(slot_pok_outputports_to_flush,number_to_string[nb_outputports_to_flush]);
			strcat(slot_pok_outputports_to_flush,"] = {");
			strcat(slot_pok_outputports_to_flush,tmp_outputports);
			strcat(slot_pok_outputports_to_flush,"};\n");
			
			strcat(slot_pok_inputports_to_preload,number_to_string[nb_inputports_to_preload]);
			strcat(slot_pok_inputports_to_preload,"] = {");
			strcat(slot_pok_inputports_to_preload,tmp_inputports);
			strcat(slot_pok_inputports_to_preload,"};\n");
			
			if (nb_outputports_to_flush >0){
				strcat(pok_outputports_to_flush,output_name);
			}else{
				strcat(pok_outputports_to_flush,"NULL");
			}
			
			if (nb_inputports_to_preload >0){
				strcat(pok_inputports_to_preload,input_name);
			}else{
				strcat(pok_inputports_to_preload,"NULL");
			}
			
			char nb_outputports_to_flush_string[3];
			sprintf(nb_outputports_to_flush_string,"%d",nb_outputports_to_flush);
			strcat(pok_nb_outputports_to_flush,nb_outputports_to_flush_string);
			
			char nb_inputports_to_preload_string[3];
			sprintf(nb_inputports_to_preload_string,"%d",nb_inputports_to_preload);
			strcat(pok_nb_inputports_to_preload,nb_inputports_to_preload_string);
		}
		
	}//for
	
	
	strcat(pok_config_scheduling_slots, "}");
	strcpy(macro_values[POK_CONFIG_SCHEDULING_SLOTS],pok_config_scheduling_slots);
	debug_printf("pok_config_scheduling_slots %s\n",macro_values[POK_CONFIG_SCHEDULING_SLOTS]);

	strcat(pok_config_scheduling_slots_allocation, "}");
	strcpy(macro_values[POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION],pok_config_scheduling_slots_allocation);
	debug_printf("pok_config_scheduling_slots allocation %s\n",macro_values[POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION]);
	
	
	if (macro_enabled[POK_NEEDS_PORTS_SLOT]){
		strcat(pok_outputports_to_flush,"};");
		strcat(pok_nb_outputports_to_flush,"};");
		strcat(pok_nb_inputports_to_preload,"};");
		strcat(pok_inputports_to_preload,"};");
		strcat(pok_config_partitions_postwrite_time,"}");
		strcpy(macro_values[POK_CONFIG_PARTITIONS_POSTWRITE_TIME],pok_config_partitions_postwrite_time);
		debug_printf("pok_nb_outputports_to_flush: %s\n",pok_nb_outputports_to_flush);
	}
	return 0;
}



int build_partition_related_macros_in_kernel_deployment_h(){
	
	int nb_of_threads = 2; // kernel thread + idle thread 
	char *one = "1";

	debug_printf("\n*********** BUILD PARTITIONS CONFIG **********\n");
	// build kernel macros
	macro_enabled[POK_CONFIG_PARTITIONS_SCHEDULER]	= TRUE;
	macro_enabled[POK_CONFIG_PARTITIONS_NTHREADS]	= TRUE;
	macro_enabled[POK_CONFIG_NB_THREADS]			= TRUE;
	macro_enabled[POK_CONFIG_SCHEDULING_MAJOR_FRAME] = TRUE;
	macro_enabled[POK_CONFIG_PARTITIONS_SIZE]		= TRUE;
	macro_enabled[POK_CONFIG_PARTITIONS_BASE_VADDR] = TRUE;
	macro_enabled[POK_CONFIG_PARTITIONS_LOADADDR]	= TRUE;
	
	macro_enabled[POK_CONFIG_SUCCESSOR_THREADS]		= FALSE;	
	macro_enabled[POK_CONFIG_SUCCESSORS_ID]	 		= FALSE;	
	macro_enabled[POK_CONFIG_PREDECESSOR_THREADS]	= FALSE;	
	macro_enabled[POK_CONFIG_PREDECESSORS_EVENTS]	= FALSE;	
	macro_enabled[POK_CONFIG_SPORADIC_TARDINESS]	= FALSE;	
	macro_enabled[POK_CONFIG_SPORADIC_DEADLINE]		= FALSE;	

	char pok_config_partitions_scheduler[400];
	strcpy(pok_config_partitions_scheduler,"{");
	
	char pok_config_partitions_nthreads[400];
	strcpy(pok_config_partitions_nthreads,"{");

	char pok_config_partitions_size[400];
	strcpy(pok_config_partitions_size,"{");
	
	char pok_config_partitions_base_vaddr[400];
	strcpy(pok_config_partitions_base_vaddr,"{");
	
	char pok_config_partitions_loadaddr[400];
	strcpy(pok_config_partitions_loadaddr,"{");

	strcpy(var_for_pok_ports_by_partition,"");
	strcpy(pok_ports_by_partition,"uint8_t*\tpok_ports_by_partition[POK_CONFIG_NB_PARTITIONS] = {");
	strcpy(pok_ports_nb_ports_by_partition,"uint8_t\t\tpok_ports_nb_ports_by_partition[POK_CONFIG_NB_PARTITIONS] = {");

	char pok_config_partitions_nb_asynch_events[400];
	if (macro_enabled[POK_NEEDS_SCHED_O1]){
		macro_enabled[POK_CONFIG_PARTITIONS_NB_ASYNCH_EVENTS] = TRUE;
		strcpy(pok_config_partitions_nb_asynch_events,"{");
	}


	char pok_config_partitions_nblockobjects[400];	
	if (nb_events_blackb_buffers >0){
		macro_enabled[POK_CONFIG_PARTITIONS_NLOCKOBJECTS] = TRUE;	
		strcpy(pok_config_partitions_nblockobjects,"{");
	}
	
	char pok_config_partition_period[400];
	if (nb_slots > nb_partitions){
		macro_enabled[POK_CONFIG_PARTITIONS_PERIOD] = TRUE;
		strcpy(pok_config_partition_period,"{");
	}


	char pok_config_successor_threads[4000];
	char pok_config_successors_id[4000];
	char pok_config_predecessor_threads[4000];
	char pok_config_predecessor_events[4000];
	char pok_config_sporadic_tardiness[4000];
	char pok_config_sporadic_deadline[4000];

	if (macro_enabled[POK_NEEDS_SCHED_O1_SPLIT]){
		macro_enabled[POK_CONFIG_SUCCESSOR_THREADS]		= TRUE;	
		strcpy(pok_config_successor_threads,"{");
		macro_enabled[POK_CONFIG_SUCCESSORS_ID]	 		= TRUE;	
		strcpy(pok_config_successors_id,"{");
		macro_enabled[POK_CONFIG_PREDECESSOR_THREADS]	= TRUE;	
		strcpy(pok_config_predecessor_threads,"{");
		macro_enabled[POK_CONFIG_PREDECESSORS_EVENTS]	= TRUE;	
		strcpy(pok_config_predecessor_events,"{");
		macro_enabled[POK_CONFIG_SPORADIC_TARDINESS]	= TRUE;	
		strcpy(pok_config_sporadic_tardiness,"{");
		macro_enabled[POK_CONFIG_SPORADIC_DEADLINE]		= TRUE;		
		strcpy(pok_config_sporadic_deadline,"{");
	}

	// build macro for EACH partition
	int i;
	for (i=0; i<nb_partitions; i++){
		
		char tmp_var[400];
		strcat(var_for_pok_ports_by_partition,"uint8_t ");
		strcpy(tmp_var,"pr");
		strcat(tmp_var,number_to_string[i+1]);
		strcat(tmp_var,"_partport");
		strcat(var_for_pok_ports_by_partition,tmp_var);
		strcat(var_for_pok_ports_by_partition,"[");
		char nr_port_tmp[3];
		sprintf(nr_port_tmp,"%d",partitions[i].nb_of_ports);
		strcat(var_for_pok_ports_by_partition,nr_port_tmp);
		strcat(var_for_pok_ports_by_partition,"] = {");

		
		if (i>0){
			strcat(pok_config_partitions_scheduler,", ");
			strcat(pok_config_partitions_nthreads,", ");
			strcat(pok_config_partitions_size,", ");
			strcat(pok_config_partitions_base_vaddr,",");
			strcat(pok_config_partitions_loadaddr,",");
			strcat(pok_ports_by_partition,", ");
			strcat(pok_ports_nb_ports_by_partition,", ");
			if (macro_enabled[POK_NEEDS_SCHED_O1]){
				strcat(pok_config_partitions_nb_asynch_events,", ");
			}
			strcat(pok_config_partitions_nblockobjects,", ");
			if (nb_slots > nb_partitions){
				strcat(pok_config_partition_period,", ");
			}

			if (macro_enabled[POK_NEEDS_SCHED_O1_SPLIT]){
				strcat(pok_config_successor_threads,", ");
				strcat(pok_config_successors_id,", ");
				strcat(pok_config_predecessor_threads,", ");
				strcat(pok_config_predecessor_events,", ");
				strcat(pok_config_sporadic_tardiness,", ");
				strcat(pok_config_sporadic_deadline,", ");				
			}
		}
		
		strcat(pok_config_partitions_size,partitions[i].size);
		strcat(pok_config_partitions_base_vaddr,partitions[i].base_vaddr);
		strcat(pok_config_partitions_loadaddr,partitions[i].loadaddr);
		
		switch (partitions[i].scheduler_type)
		{	
			case FPPS:	
				strcat(pok_config_partitions_scheduler,"POK_SCHED_FPPS");
				break;
			case O1:
				strcat(pok_config_partitions_scheduler,"POK_SCHED_O1");				
				break;
			case O1_SPLIT:
				strcat(pok_config_partitions_scheduler,"POK_SCHED_O1_SPLIT");				
				break;
		}
	
		nb_of_threads += partitions[i].nb_of_threads;
		strcat(pok_config_partitions_nthreads,number_to_string[partitions[i].nb_of_threads]);

		strcat(pok_ports_by_partition,tmp_var);
		strcat(pok_ports_nb_ports_by_partition,number_to_string[partitions[i].nb_of_ports]);
		
		struct port_list_item_t *tmp = partitions[i].ports;
		int j = 0;
		while (tmp != NULL){
			if (j>0){
				strcat(var_for_pok_ports_by_partition,",");
			}
			strcat(var_for_pok_ports_by_partition,ports[tmp->port_id].local_id);
			tmp = tmp->next;
			j++;
		}

		strcat(var_for_pok_ports_by_partition,"};\n");
		
		if (macro_enabled[POK_NEEDS_SCHED_O1]){
			strcat(pok_config_partitions_nb_asynch_events,number_to_string[partitions[i].nb_asynch_events]);
		}
		
		if (nb_events_blackb_buffers > 0){
			strcat(pok_config_partitions_nblockobjects,number_to_string[partitions[i].nb_events_blackb_buffers]);
		}
		
		if (nb_slots > nb_partitions){
			char partition_perdiod[5];
			sprintf(partition_perdiod,"%d",partitions[i].period);
			strcat(pok_config_partition_period,partition_perdiod);
		}

		if (macro_enabled[POK_NEEDS_SCHED_O1_SPLIT]){
			strcat(pok_config_successor_threads,"FALSE, ");
			strcat(pok_config_successors_id,"0, ");
			strcat(pok_config_predecessor_threads,"FALSE, ");
			strcat(pok_config_predecessor_events,"0, ");
			strcat(pok_config_sporadic_tardiness,"FALSE, ");
			strcat(pok_config_sporadic_deadline,"0, ");
			int j;			
			for (j=0; j<partitions[i].nb_of_threads-1; j++){
				
				debug_printf("threads[%d].type %d\n", j, partitions[i].threads[j].type);
				if (j>0){
					strcat(pok_config_successor_threads,", ");
					strcat(pok_config_successors_id,", ");
					strcat(pok_config_predecessor_threads,", ");
					strcat(pok_config_predecessor_events,", ");
					strcat(pok_config_sporadic_tardiness,", ");
					strcat(pok_config_sporadic_deadline,", ");
				}
				if (partitions[i].threads[j].type == EVENT_SUCCESSOR){
					strcat(pok_config_successor_threads,"TRUE");
					strcat(pok_config_sporadic_tardiness,"TRUE");
					char tmp_deadline[5];
					sprintf(tmp_deadline,"%d",((major_frame*10)+20));
					strcat(pok_config_sporadic_deadline,tmp_deadline);					
					//strcat(pok_config_sporadic_deadline,", ");
				}else{
					strcat(pok_config_successor_threads,"FALSE");
					strcat(pok_config_sporadic_tardiness,"FALSE");
					strcat(pok_config_sporadic_deadline,"0");
				}
				if (partitions[i].threads[j].type == EVENT_PREDECESSOR){
					strcat(pok_config_successors_id,number_to_string[j+2]);					
					//strcat(pok_config_successors_id,",");
					strcat(pok_config_predecessor_threads,"TRUE");
					strcat(pok_config_predecessor_events, number_to_string[partitions[i].threads[j].event_nb +1]);
					//strcat(pok_config_predecessor_events,", ");
				}else{
					strcat(pok_config_successors_id,"0");
					strcat(pok_config_predecessor_threads,"FALSE");
					strcat(pok_config_predecessor_events,"0");
				}
			}			
		}
		
		strcat(pok_config_sporadic_tardiness,", FALSE, TRUE");

	}
	
	strcpy(macro_values[POK_CONFIG_NB_THREADS],number_to_string[nb_of_threads]);

	strcat(pok_config_partitions_scheduler, "}");
	strcpy(macro_values[POK_CONFIG_PARTITIONS_SCHEDULER],pok_config_partitions_scheduler);
	debug_printf("pok_config_partitions_scheduler %s\n",macro_values[POK_CONFIG_PARTITIONS_SCHEDULER] );

	strcat(pok_config_partitions_nthreads, "}");
	strcpy(macro_values[POK_CONFIG_PARTITIONS_NTHREADS],pok_config_partitions_nthreads);
	debug_printf("pok_config_partitions_nbthreads %s\n",macro_values[POK_CONFIG_PARTITIONS_NTHREADS]);

	strcat(pok_config_partitions_size, "}");
	strcpy(macro_values[POK_CONFIG_PARTITIONS_SIZE],pok_config_partitions_size);
	debug_printf("pok_config_partitions_size %s\n",macro_values[POK_CONFIG_PARTITIONS_SIZE]);
	
	strcat(pok_config_partitions_base_vaddr, "}");
	strcpy(macro_values[POK_CONFIG_PARTITIONS_BASE_VADDR],pok_config_partitions_base_vaddr);
	debug_printf("pok_config_partitions_base_vaddr %s\n",macro_values[POK_CONFIG_PARTITIONS_BASE_VADDR]);

	strcat(pok_config_partitions_loadaddr, "}");
	strcpy(macro_values[POK_CONFIG_PARTITIONS_LOADADDR],pok_config_partitions_loadaddr);
	debug_printf("pok_config_partitions_loadaddr %s\n",macro_values[POK_CONFIG_PARTITIONS_LOADADDR]);

	strcat(pok_ports_by_partition,"};");
	strcat(pok_ports_nb_ports_by_partition,"};");
	
	if (nb_slots > nb_partitions){
		strcat(pok_config_partition_period,"}");
		strcpy(macro_values[POK_CONFIG_PARTITIONS_PERIOD],pok_config_partition_period);
	}
	
	char major_frame_string[5];
 	sprintf(major_frame_string,"%d",major_frame);
	strcpy(macro_values[POK_CONFIG_SCHEDULING_MAJOR_FRAME],major_frame_string);

	if (macro_enabled[POK_NEEDS_SCHED_O1]){
		strcat(pok_config_partitions_nb_asynch_events,"}");
		strcpy(macro_values[POK_CONFIG_PARTITIONS_NB_ASYNCH_EVENTS],pok_config_partitions_nb_asynch_events);
	}

	if (nb_events_blackb_buffers > 0){
		strcat(pok_config_partitions_nblockobjects,"}");
		strcpy(macro_values[POK_CONFIG_PARTITIONS_NLOCKOBJECTS],pok_config_partitions_nblockobjects);
	}
		
	if (macro_enabled[POK_NEEDS_SCHED_O1_SPLIT]){
		strcat(pok_config_successor_threads,"}");
		strcpy(macro_values[POK_CONFIG_SUCCESSOR_THREADS],pok_config_successor_threads);
		strcat(pok_config_successors_id,"}");
		strcpy(macro_values[POK_CONFIG_SUCCESSORS_ID],pok_config_successors_id);
		strcat(pok_config_predecessor_threads,"}");
		strcpy(macro_values[POK_CONFIG_PREDECESSOR_THREADS],pok_config_predecessor_threads);
		strcat(pok_config_predecessor_events,"}");
		strcpy(macro_values[POK_CONFIG_PREDECESSORS_EVENTS],pok_config_predecessor_events);
		strcat(pok_config_sporadic_tardiness,"}");	
		strcpy(macro_values[POK_CONFIG_SPORADIC_TARDINESS],pok_config_sporadic_tardiness);
		strcat(pok_config_sporadic_deadline,"}");
		strcpy(macro_values[POK_CONFIG_SPORADIC_DEADLINE],pok_config_sporadic_deadline);
	}

	debug_printf("end: build_partition_related_macros_in_kernel_deployment_h\n");

	return 0;
}




void init_source_ports_variables(int partition_id, thread_t thread){
	
	struct port_list_item_t *port_item = thread.ports;
	while (port_item != NULL){
		if (ports[port_item->port_id].direction == SOURCE){
			init_source_port_variables  (partition_id, port_item);
		}
		port_item = port_item->next;
	}
}

void init_valid_variable(int partition_id, thread_t thread){

	if ( thread.hasInputSamplingPort ){
			strcat(partitions[partition_id].activity_c,"\tVALIDITY_TYPE valid;\n");
	}

}


void build_activity_ports(int partition_id, thread_t thread){
   
	int j = 0;
	struct port_list_item_t *port_item = thread.ports;
	while (port_item != NULL){
		if (ports[port_item->port_id].kind == SAMPLING) {
			if (ports[port_item->port_id].direction == SOURCE) {
				write_sampling_message(partition_id, port_item);			
			}else if (ports[port_item->port_id].direction == DESTINATION) {
				read_sampling_message(partition_id, port_item);			
			}
		}else if (ports[port_item->port_id].kind == QUEUING) {
			if (ports[port_item->port_id].direction == SOURCE) {
				send_queuing_message(partition_id, port_item);			
			}else if (ports[port_item->port_id].direction == DESTINATION) {
				receive_queuing_message(partition_id, port_item);			
			}
		}
		port_item = port_item->next;
	}
}

void init_bb_display_variables(int partition_id, thread_t thread){

	if (thread.type == BB_DISPLAY){
		strcat(partitions[partition_id].activity_c,"\tMESSAGE_ADDR_TYPE msg_to_display;\n");
		strcat(partitions[partition_id].activity_c,"\tMESSAGE_SIZE_TYPE msg_to_display_size;\n");
		strcat(partitions[partition_id].activity_c,"\tmsg_to_display_size = sizeof (int);\n");
		strcat(partitions[partition_id].activity_c,"\tint j = 0;\n");
	}
}

void init_buffer_send_variables(int partition_id, thread_t thread){

	if (thread.type == BUFFER_SEND){
		strcat(partitions[partition_id].activity_c,"\n\tMESSAGE_ADDR_TYPE msg_to_send;");
		strcat(partitions[partition_id].activity_c,"\n\tMESSAGE_SIZE_TYPE msg_to_send_size;\n");
		strcat(partitions[partition_id].activity_c,"\tint j = 0;\n");
	}
}

int build_activity_periodic_thread(int partition_id, int thread_number){

	debug_printf("in build_activity_periodic_thread\n");
	
	thread_t thread = partitions[partition_id].threads[thread_number-1];	

	init_periodic_thread(partition_id, thread);

	init_valid_variable(partition_id, thread);

	init_source_ports_variables(partition_id, thread);

	init_bb_display_variables(partition_id, thread);
	
	while_in_thread(partition_id, thread.thread_nb);
	
	build_activity_ports(partition_id, thread);

	
	char message_to_print[200];
	strcpy(message_to_print,"\"");
	strcat(message_to_print,number_to_string[thread.thread_nb]);
	strcat(message_to_print,": -- do something\\n\\t\\t");
	strcat(message_to_print,number_to_string[thread.thread_nb]);

	switch ( thread.type ) {
		case EVENT_SET:
			build_activity_thread_event_set(partition_id, thread);
			break;
		case EVENT_RESUME:
			//      printf(" 1:Waiting on an event\n\n");
			strcat(message_to_print," :: Waiting on an event\\n\\n\"");
			build_activity_thread_resume(partition_id, thread.thread_nb, message_to_print);
			break;
		case BB_RESUME:
			strcat(message_to_print," :: Resuming Reader\\n\\n\"");
			build_activity_thread_resume(partition_id, thread.thread_nb, message_to_print);
			break;		
		case BB_DISPLAY:
			debug_printf("BB_DISPLAY\n");
			build_activity_thread_blackboard_display(partition_id, thread);	
			break;
		case BUFFER_RESUME_SENDER:
			strcat(message_to_print," :: Resuming Sender\\n\\n\"");
			build_activity_thread_resume(partition_id, thread.thread_nb, message_to_print);
			break;	
		case BUFFER_RESUME_RECEIVER:
			strcat(message_to_print," :: Resuming Receiver\\n\\n\"");
			build_activity_thread_resume(partition_id, thread.thread_nb, message_to_print);
			break;	
		case EVENT_PREDECESSOR:
			build_activity_thread_pre_wait(partition_id, thread.thread_nb);
			break;	
		case EVENT_SETTER:
			build_activity_thread_set(partition_id, thread.thread_nb);
			break;	
	}
	
	strcat(partitions[partition_id].activity_c,"\t\tPERIODIC_WAIT (&(ret));\n\t}\n}\n\n");
	return 0;
}

int init_aperiodic_thread(int partition_id, thread_t thread) {

  /*  strcpy(thread.entry_point,"thr");
    strcat(thread.entry_point,number_to_string[partition_id+1]);
    strcat(thread.entry_point,"_");
    strcat(thread.entry_point,number_to_string[thread.thread_nb-1]);
    strcat(thread.entry_point,"bis_job ()");
*/
    strcat(partitions[partition_id].activity_h,"void* ");
    strcat(partitions[partition_id].activity_h,thread.entry_point);
    strcat(partitions[partition_id].activity_h,"();\n");

    strcat(partitions[partition_id].activity_c,"void* ");
	strcat(partitions[partition_id].activity_c,thread.entry_point);		
	strcat(partitions[partition_id].activity_c,"()\n{\n\tint flag = 0;\n");
	strcat(partitions[partition_id].activity_c,"\tRETURN_CODE_TYPE ret;\n");
	strcat(partitions[partition_id].activity_c,"\tSYSTEM_TIME_TYPE time = APERIODIC_PROCESS;\n");

}

int build_activity_thread_event_set(int partition_id, thread_t thread){

//	thread_t thread = partitions[partition_id].threads[thread_number-1];
/*	init_periodic_thread(partition_id, thread_number);
	while_in_thread(partition_id, thread_number);
*/
	strcat(partitions[partition_id].activity_c,"\t\tprintf(\" ");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c,": -- do something\\n\");\n");

	//	printf(" 1:Waiting on an event\n\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf(\" ");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c,": Setting an event UP\\n\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\t\tSET_EVENT (");
	strcat(partitions[partition_id].activity_c,DEFAULT_EVENTS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.event_nb]);
	strcat(partitions[partition_id].activity_c,"], &(ret));\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf(\" ");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c,": Event SET -- Resume sporadic\\n\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\t\tRESUME (arinc_threads[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.thread_nb+1]);
	strcat(partitions[partition_id].activity_c,"], &(ret));\n");
//	strcat(partitions[partition_id].activity_c,"\t\tPERIODIC_WAIT (&(ret));\n\t}\n}\n\n");
		
	return 0;
}

int build_activity_thread_event_reset(int partition_id, thread_t thread){

	init_aperiodic_thread(partition_id, thread);
	strcat(partitions[partition_id].activity_c,"\tprintf(\"\\t");	
	strcat(partitions[partition_id].activity_c, number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c, ": Sporadic (after set evt)\\n\");\n"); 
	strcat(partitions[partition_id].activity_c,"\twhile (1)\n\t{\n");
	strcat(partitions[partition_id].activity_c,"\t\tif (flag){\n");
	strcat(partitions[partition_id].activity_c,"\t\t\tprintf(\"\\t");	
	strcat(partitions[partition_id].activity_c, number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c, ": Resume - Continuation after event is set\\n\\n\");\n"); 
	strcat(partitions[partition_id].activity_c,"\t\t\tRESET_EVENT (");
	strcat(partitions[partition_id].activity_c,DEFAULT_EVENTS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.event_nb]);
	strcat(partitions[partition_id].activity_c,"], &(ret));\n");
	strcat(partitions[partition_id].activity_c,"\t\t\tprintf(\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c,": Event %i is DOWN [\%i]\\n\", ");
	strcat(partitions[partition_id].activity_c,DEFAULT_EVENTS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.event_nb]);
	strcat(partitions[partition_id].activity_c,"], ret);\n");
	strcat(partitions[partition_id].activity_c,"\t\t\tprintf(\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c,": Th self-suspending.........zzzz\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\t\t} else {\n\t\t\tflag=1;\n\t\t}\n");
	strcat(partitions[partition_id].activity_c,"\t\tSUSPEND_SELF (time, &ret);\n\t}\n}\n\n");

	return 0;
}
/*
int build_activity_thread_event_resume(int partition_id, int thread_number){

	thread_t thread = partitions[partition_id].threads[thread_number-1];

		//	printf(" 1:-- do something\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf(\"");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,":-- do something\\n\");\n");
	//	printf(" 1:Waiting on an event\n\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf(\"");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,":Waiting on an event\\n\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\t\tRESUME (arinc_threads[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number+1]);
	strcat(partitions[partition_id].activity_c,"], &(ret));\n");
//	strcat(partitions[partition_id].activity_c,"\t\tPERIODIC_WAIT (&(ret));\n\t}\n}\n\n");


	return 0;
}

*/


int build_activity_thread_resume(int partition_id, int thread_number, char *message_to_print){

	thread_t thread = partitions[partition_id].threads[thread_number-1];

/*	//	printf(" 1:-- do something\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf(\"");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,":-- do something\\n\");\n");
	//	printf(" 1:Waiting on an event\n\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf(\"");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,":Waiting on an event\\n\\n\");\n");
*/
	strcat(partitions[partition_id].activity_c,"\t\tprintf(");
	strcat(partitions[partition_id].activity_c,message_to_print);
	strcat(partitions[partition_id].activity_c,");\n");

	strcat(partitions[partition_id].activity_c,"\t\tRESUME (arinc_threads[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number+1]);
	strcat(partitions[partition_id].activity_c,"], &(ret));\n");
//	strcat(partitions[partition_id].activity_c,"\t\tPERIODIC_WAIT (&(ret));\n\t}\n}\n\n");

	return 0;
}

int build_activity_thread_pre_wait(int partition_id, thread_t thread){

//	thread_t thread = partitions[partition_id].threads[thread_number-1];

	strcat(partitions[partition_id].activity_c, "\t\tprintf(\"Partition n. ");
	strcat(partitions[partition_id].activity_c, number_to_string[partition_id+1]);
	strcat(partitions[partition_id].activity_c,	" - Thread n. ");
	strcat(partitions[partition_id].activity_c, number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c, " : predecessor of wait");
	strcat(partitions[partition_id].activity_c, number_to_string[thread.event_nb+1]);
	strcat(partitions[partition_id].activity_c, "-thread\\n\");\n");
	
	strcat(partitions[partition_id].activity_c, "\t\tprintf(\"\\t-- do pre-wait");
	strcat(partitions[partition_id].activity_c, number_to_string[thread.event_nb+1]);
	strcat(partitions[partition_id].activity_c, " activity\\n\");\n");

	strcat(partitions[partition_id].activity_c, "\t\tprintf(\"\\t-- end predecessor (wait");
	strcat(partitions[partition_id].activity_c, number_to_string[thread.event_nb+1]);
	strcat(partitions[partition_id].activity_c, ") activity, suspend itself waiting for the next period!\\n\");\n");

	return 0;
}

int build_activity_thread_set(int partition_id, thread_t thread){

	strcat(partitions[partition_id].activity_c, "\t\tprintf(\" Partition n. ");
	strcat(partitions[partition_id].activity_c, number_to_string[partition_id+1]);
	strcat(partitions[partition_id].activity_c,	" - Thread n. ");
	strcat(partitions[partition_id].activity_c, number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c, " : set");
	strcat(partitions[partition_id].activity_c, number_to_string[thread.event_nb+1]);
	strcat(partitions[partition_id].activity_c, "-thread\\n\");\n");
	
	strcat(partitions[partition_id].activity_c, "\t\tprintf(\"\\t-- setting UP the event");
	strcat(partitions[partition_id].activity_c, number_to_string[thread.event_nb+1]);
	strcat(partitions[partition_id].activity_c, "\\n\");\n");

	strcat(partitions[partition_id].activity_c,"\t\tSET_EVENT (");
	strcat(partitions[partition_id].activity_c,DEFAULT_EVENTS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.event_nb]);
	strcat(partitions[partition_id].activity_c,"], &(ret));\n");

	strcat(partitions[partition_id].activity_c, "\t\tprintf(\"\\t-- end setter (event");
	strcat(partitions[partition_id].activity_c, number_to_string[thread.event_nb+1]);
	strcat(partitions[partition_id].activity_c, ") activity, suspend itself waiting for the next period!\\n\");\n");

	return 0;
}

int build_activity_thread_successor(int partition_id, thread_t thread){
	

/*	strcpy(thread.entry_point,"thr");
	strcat(thread.entry_point,number_to_string[partition_id+1]);
	strcat(thread.entry_point,"_");
	strcat(thread.entry_point,number_to_string[thread.thread_nb]);
	strcat(thread.entry_point,"_set");
	strcat(thread.entry_point,number_to_string[thread.event_nb+1]);
	strcat(thread.entry_point,"_job");*/

	
    strcat(partitions[partition_id].activity_h,"void* ");
    strcat(partitions[partition_id].activity_h,thread.entry_point);
    strcat(partitions[partition_id].activity_h,"();\n");

    strcat(partitions[partition_id].activity_c,"void* ");
	strcat(partitions[partition_id].activity_c,thread.entry_point);		
	strcat(partitions[partition_id].activity_c,"()\n{\n\tRETURN_CODE_TYPE ret;\n");
	strcat(partitions[partition_id].activity_c,"\twhile (1)\n\t{\n");

	strcat(partitions[partition_id].activity_c, "\t\tprintf(\" Partition n. ");
	strcat(partitions[partition_id].activity_c, number_to_string[partition_id+1]);
	strcat(partitions[partition_id].activity_c,	" - Thread n. ");
	strcat(partitions[partition_id].activity_c, number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c, " : successor of wait");
	strcat(partitions[partition_id].activity_c, number_to_string[thread.event_nb+1]);
	strcat(partitions[partition_id].activity_c, "-thread\\n\");\n");
	strcat(partitions[partition_id].activity_c, "\t\tprintf(\"\\t-- suspend itself waiting for activation event\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\t\tWAIT_EVENT ("); 
	strcat(partitions[partition_id].activity_c,DEFAULT_EVENTS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.event_nb]);
	strcat(partitions[partition_id].activity_c,"], 0, &(ret));\n");
	strcat(partitions[partition_id].activity_c, "\t\tprintf(\" Partition n. ");
	strcat(partitions[partition_id].activity_c, number_to_string[partition_id+1]);
	strcat(partitions[partition_id].activity_c,	" - Thread n. ");
	strcat(partitions[partition_id].activity_c, number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c, " : successor of wait");
	strcat(partitions[partition_id].activity_c, number_to_string[thread.event_nb+1]);
	strcat(partitions[partition_id].activity_c, "-thread\\n\");\n");
	strcat(partitions[partition_id].activity_c, "\t\tprintf(\"\\t-- after activation event (event");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.event_nb+1]);
	strcat(partitions[partition_id].activity_c,")! Do post-wait activity\\n\");\n");

	strcat(partitions[partition_id].activity_c,"\t\tRESET_EVENT (");
	strcat(partitions[partition_id].activity_c,DEFAULT_EVENTS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");	
	strcat(partitions[partition_id].activity_c,number_to_string[thread.event_nb]);
	strcat(partitions[partition_id].activity_c,"], &(ret));\n");
	strcat(partitions[partition_id].activity_c,"\t}\n}\n\n");
 
}

int build_activity_thread_event_wait(int partition_id, int thread_number){

	thread_t thread = partitions[partition_id].threads[thread_number-1];

	init_aperiodic_thread(partition_id, thread);

	strcat(partitions[partition_id].activity_c,"\tprintf(\"\tPartition n. ");
	strcat(partitions[partition_id].activity_c,number_to_string[partition_id+1]);
	strcat(partitions[partition_id].activity_c," - Thread n.");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c,"\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\t\t\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c,": Sporadic (wait evt)\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\twhile (1)\n\t{\n");
	strcat(partitions[partition_id].activity_c,"\t\tif (flag){\n");
	strcat(partitions[partition_id].activity_c,"\t\t\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c,": Th self-suspending.........zzzz\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\t\t} else {\n\t\t\tflag=1;\n\t\t}\n");
	strcat(partitions[partition_id].activity_c,"\t\tSUSPEND_SELF (time, &ret);\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c,": Resume - Going to WAIT_EVENT \\n\");\n"); 
	strcat(partitions[partition_id].activity_c,"\t\tWAIT_EVENT ("); 
	strcat(partitions[partition_id].activity_c,DEFAULT_EVENTS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.event_nb]);
	strcat(partitions[partition_id].activity_c,"], 0, &(ret));\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.thread_nb]);
	strcat(partitions[partition_id].activity_c,": Event %i is UP\\n\", ");
	strcat(partitions[partition_id].activity_c,DEFAULT_EVENTS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.event_nb]);
	strcat(partitions[partition_id].activity_c,"]);\n\t}\n}\n\n");
	return 0;
}

int build_activity_thread_blackboard_read(int partition_id, int thread_number) {
	
	thread_t thread = partitions[partition_id].threads[thread_number-1];

	init_aperiodic_thread(partition_id, thread);

	strcat(partitions[partition_id].activity_c,"\tMESSAGE_ADDR_TYPE msg_to_read;");
	strcat(partitions[partition_id].activity_c,"\n\tMESSAGE_SIZE_TYPE msg_to_read_size;");
	strcat(partitions[partition_id].activity_c,"\n\tint output_value;");
	strcat(partitions[partition_id].activity_c,"\n\tmsg_to_read = &output_value;");
	
	strcat(partitions[partition_id].activity_c,"\n\tprintf(\"\tPartition n. ");
	strcat(partitions[partition_id].activity_c,number_to_string[partition_id+1]);
	strcat(partitions[partition_id].activity_c," - Thread n.");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,"\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,": Read sporadic\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\twhile (1)\n\t{\n");
	strcat(partitions[partition_id].activity_c,"\t\tif (flag)\n\t\t{\n");
	strcat(partitions[partition_id].activity_c,"\t\t\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,": Read Th self-suspending.........zzzz\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\t\t} else {\n\t\t\tflag=1;\n\t\t}\n");
	strcat(partitions[partition_id].activity_c,"\t\tSUSPEND_SELF (time, &ret);\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,": --> Resume - Read sporadic \\n\");\n"); 
	strcat(partitions[partition_id].activity_c,"\t\tREAD_BLACKBOARD (");
	strcat(partitions[partition_id].activity_c,DEFAULT_BLACKBOARDS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.blackboard_nb]);
	strcat(partitions[partition_id].activity_c,"], 2000, msg_to_read, &msg_to_read_size, &(ret));\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,
	": --> Sporadic - Read: %i in BLACKBOARD %i, size: %i, ret: [%i]\\n\", *((int*)msg_to_read), ");
	strcat(partitions[partition_id].activity_c,DEFAULT_BLACKBOARDS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.blackboard_nb]);	
	strcat(partitions[partition_id].activity_c,"], msg_to_read_size, ret);\n\t}\n}\n\n");
	return 0;
}

int build_activity_thread_blackboard_display(int partition_id, int thread_number){

	thread_t thread = partitions[partition_id].threads[thread_number];

	strcat(partitions[partition_id].activity_c,"\t\tprintf(\"");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c," :: Display\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\t\tmsg_to_display = &(blackboards_values[j]);\n");
	strcat(partitions[partition_id].activity_c,"\t\tDISPLAY_BLACKBOARD (");
	strcat(partitions[partition_id].activity_c,DEFAULT_BLACKBOARDS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.blackboard_nb]);
	strcat(partitions[partition_id].activity_c,"], msg_to_display, msg_to_display_size, &(ret));\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf(\"");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,
	": --> Displayed: %i in BLACKBOARD %i, size: %i ret: [%i]\\n\", *((int*)msg_to_display), ");
	strcat(partitions[partition_id].activity_c,DEFAULT_BLACKBOARDS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.blackboard_nb]);
	strcat(partitions[partition_id].activity_c,"], msg_to_display_size, ret);");
	strcat(partitions[partition_id].activity_c,"\n\t\tj = (j+1) % 8;");	
	strcat(partitions[partition_id].activity_c,"\n\t\tprintf(\"");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,": --> Resume sporadic\\n\\n\");");
	strcat(partitions[partition_id].activity_c,"\n\t\tRESUME (arinc_threads[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number +1]);
	strcat(partitions[partition_id].activity_c,"], &(ret));\n");
	return 0;
}

int build_activity_thread_blackboard_suspend(int partition_id, int thread_number){

		
	thread_t thread = partitions[partition_id].threads[thread_number-1];
	init_aperiodic_thread(partition_id, thread);

	strcat(partitions[partition_id].activity_c,"\tprintf(\"\\t");	
	strcat(partitions[partition_id].activity_c, number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c, " :: Display sporadic\\n\");\n"); 
	strcat(partitions[partition_id].activity_c,"\twhile (1)\n\t{\n");
	strcat(partitions[partition_id].activity_c,"\t\tif (flag)\n\t\t{\n");
	strcat(partitions[partition_id].activity_c,"\t\t\tprintf(\"\\t");	
	strcat(partitions[partition_id].activity_c, number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c, ": --> Display Th self-suspending.........zzzz\\n\\n\");\n"); 
	strcat(partitions[partition_id].activity_c,"\t\t} else {\n\t\t\tflag=1;\n\t\t}\n");
	strcat(partitions[partition_id].activity_c,"\t\tSUSPEND_SELF (time, &ret);\n\t}\n}\n\n");

	return 0;
}

int build_activity_thread_buffer_send(int partition_id, int thread_number){

	thread_t thread = partitions[partition_id].threads[thread_number-1];

	init_aperiodic_thread(partition_id, thread);

	init_buffer_send_variables(partition_id, thread);

	strcat(partitions[partition_id].activity_c,"\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,": Sender sporadic\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\twhile (1)\n\t{\n");
	strcat(partitions[partition_id].activity_c,"\t\tif (flag)\n\t\t{\n");
	strcat(partitions[partition_id].activity_c,"\t\t\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,": Sender Th self-suspending.........zzzz\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\t\t} else {\n\t\t\tflag=1;\n\t\t}\n");
	strcat(partitions[partition_id].activity_c,"\t\tSUSPEND_SELF (time, &ret);\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,": --> Resume - Sender sporadic \\n\");\n"); 
	strcat(partitions[partition_id].activity_c,"\t\tmsg_to_send_size = sizeof(buffers_values[j]);\n");
	strcat(partitions[partition_id].activity_c,"\t\tmsg_to_send = &(buffers_values[j]);\n");
	strcat(partitions[partition_id].activity_c,"\t\tSEND_BUFFER(");
	strcat(partitions[partition_id].activity_c,DEFAULT_BUFFERS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.buffer_nb]);
	strcat(partitions[partition_id].activity_c,"],msg_to_send, msg_to_send_size,"); 
	strcat(partitions[partition_id].activity_c,DEFAULT_SEND_BUFFER_TIMEOUT);
	strcat(partitions[partition_id].activity_c,",&(ret));\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,
	": --> Sent: %i to BUFFER %i, return code: [%i]\\n\", buffers_values[j], ");
	strcat(partitions[partition_id].activity_c,DEFAULT_BUFFERS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.buffer_nb]);
	strcat(partitions[partition_id].activity_c,"], ret);\n");
	strcat(partitions[partition_id].activity_c,"\t\tj = (j+1) % 8;\n\t}\n}\n\n");

	return 0;
}

int build_activity_thread_buffer_receive(int partition_id, int thread_number){

	thread_t thread = partitions[partition_id].threads[thread_number-1];

	init_aperiodic_thread(partition_id, thread);

	strcat(partitions[partition_id].activity_c,"\tint tmp;\n");
	strcat(partitions[partition_id].activity_c,"\tMESSAGE_ADDR_TYPE msg_to_receive = &tmp;\n");
	strcat(partitions[partition_id].activity_c,"\tMESSAGE_SIZE_TYPE msg_to_receive_size;\n");
	

	strcat(partitions[partition_id].activity_c,"\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c," --> Receiver sporadic sporadic\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\twhile (1)\n\t{\n");
	strcat(partitions[partition_id].activity_c,"\t\tif (flag)\n\t\t{\n");
	strcat(partitions[partition_id].activity_c,"\t\t\tprintf(\"\\t");	
	strcat(partitions[partition_id].activity_c, number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c, ": --> Receiver self-suspending.........zzzz\\n\\n\");\n"); 
	strcat(partitions[partition_id].activity_c,"\t\t} else {\n\t\t\tflag=1;\n\t\t}\n");
	strcat(partitions[partition_id].activity_c,"\t\tSUSPEND_SELF (time, &ret);\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf(\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,": --> resume - Receiver sporadic\\n\");\n");
	strcat(partitions[partition_id].activity_c,"\t\tRECEIVE_BUFFER (");
	strcat(partitions[partition_id].activity_c,DEFAULT_BUFFERS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.buffer_nb]);
	strcat(partitions[partition_id].activity_c,"], ");
	strcat(partitions[partition_id].activity_c,DEFAULT_RECEIVE_BUFFER_TIMEOUT);
	strcat(partitions[partition_id].activity_c,", msg_to_receive, &msg_to_receive_size, &(ret));\n");
	strcat(partitions[partition_id].activity_c,"\t\tprintf (\"\\t");
	strcat(partitions[partition_id].activity_c,number_to_string[thread_number]);
	strcat(partitions[partition_id].activity_c,
	": --> Received: %i from BUFFER %i, (size: %i) return code: [%i]\\n\", *((int*)msg_to_receive), "); 
	strcat(partitions[partition_id].activity_c,DEFAULT_BUFFERS_IDS_ARRAYNAME);
	strcat(partitions[partition_id].activity_c,"[");
	strcat(partitions[partition_id].activity_c,number_to_string[thread.buffer_nb]);
	strcat(partitions[partition_id].activity_c, "], msg_to_receive_size, ret);\n\t}\n}\n\n");

}

