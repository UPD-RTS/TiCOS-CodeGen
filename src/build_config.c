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
#include "build_utils.h"


extern int nb_events_blackb_buffers;
extern int major_frame;
extern int nb_blackboards;
extern int nb_events;
extern int nb_asynch_events;

/*

The functions implemented in this file are used for build the configuration of the application example
starting form the configuration read from the xml file.

*/ 

int build_configuration(){


	if (build_kernel_configuration () != 0){
		return -1;	
	}
	debug_printf("=============== END Kernel configuration ===============\n");

	if (build_partitions_configuration () != 0){
		return -1;	
	}	

	debug_printf("=============== END partitions configuration ===============\n");
	
	if (build_ports_configuration () != 0){
		return -1;	
	}

	debug_printf("=============== END ports configuration ===============\n");

	return 0;
}


// build kernel/deployment.* macros and variables definition
int build_kernel_configuration (){
	
	// initialization of required macros that have a FIXED value
	init_kernel_macros();
	
	// build slot configuration 
	if (build_slots_config() != 0){
		return -1;	
	}
	
	// build partition related macros in kernel configuration (e.g. nb. of thread for each partitions)
	if (build_partition_related_macros_in_kernel_deployment_h() != 0){
		return -1;		
	}

	debug_printf("end: build_kernel_config\n");

	return 0;
}




int build_partitions_configuration (){
	int i;
	for (i=0; i<nb_partitions; i++){
		if (build_partition_config(i) != 0){
			return -1;		
		}
	}
	return 0;	
}


// build partition's macros and the partition's array
int build_partition_config(int partition_id){

	char *one = "1";

	debug_printf("in build_partition_config, partition id %d\n", partition_id);
	
	macro_partitions_enabled[partition_id][POK_GENERATED_CODE_PART] = TRUE;
	strcpy(macro_partitions_values[partition_id][POK_GENERATED_CODE_PART], one);
	
	macro_partitions_enabled[partition_id][POK_NEEDS_LIBC_STDIO] = TRUE;
	strcpy(macro_partitions_values[partition_id][POK_NEEDS_LIBC_STDIO], one);
	
	macro_partitions_enabled[partition_id][POK_NEEDS_ARINC653_PROCESS] = TRUE;
	strcpy(macro_partitions_values[partition_id][POK_NEEDS_ARINC653_PROCESS], one);
	
	macro_partitions_enabled[partition_id][POK_NEEDS_ARINC653_PARTITION] = TRUE;
	strcpy(macro_partitions_values[partition_id][POK_NEEDS_ARINC653_PARTITION], one);
	
	// Required -- it defines the PERIODIC_WAIT function
	macro_partitions_enabled[partition_id][POK_NEEDS_ARINC653_TIME] = TRUE;
	strcpy(macro_partitions_values[partition_id][POK_NEEDS_ARINC653_TIME], one);
	
	macro_partitions_enabled[partition_id][POK_NEEDS_ARINC653_PART] = TRUE;
	strcpy(macro_partitions_values[partition_id][POK_NEEDS_ARINC653_PART], one);
	
	//macro_partitions_enabled[partition_id][POK_CONFIG_NEEDS_FUNC_STRNCPY] = TRUE;
	//strcpy(macro_partitions_values[partition_id][POK_CONFIG_NEEDS_FUNC_STRNCPY], one);
	
	macro_partitions_enabled[partition_id][POK_CONFIG_PART_NB_THREADS] = TRUE;
	strcpy(macro_partitions_values[partition_id][POK_CONFIG_PART_NB_THREADS], 
			number_to_string[partitions[partition_id].nb_of_threads]);

	debug_printf("partitions[partition_id].nb_of_queuing_port %d\n", partitions[partition_id].nb_of_queuing_port);
	if (partitions[partition_id].nb_of_queuing_port>0){
		macro_partitions_enabled[partition_id][POK_NEEDS_ARINC653_QUEUEING] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_NEEDS_ARINC653_QUEUEING], one);
		macro_partitions_enabled[partition_id][POK_CONFIG_PART_NB_INPUT_QUEUING_PORTS] = TRUE;
	
		debug_printf("partitions[partition_id].nb_of_input_queuing_port %d\n", partitions[partition_id].nb_of_input_queuing_port);	
		char nb_of_input_queuing_port_string[3];
		sprintf(nb_of_input_queuing_port_string,"%d",partitions[partition_id].nb_of_input_queuing_port);
		strcpy(macro_partitions_values[partition_id][POK_CONFIG_PART_NB_INPUT_QUEUING_PORTS], nb_of_input_queuing_port_string);
		if (partitions[partition_id].scheduler_type == O1){
			char input_buffers_queuing[10000];
			char queuing_input_msg_data_buffers[10000];
			strcpy(input_buffers_queuing,"unsigned char *");
			strcat(input_buffers_queuing,DEFAULT_INPUT_BUFFERS_QUEUING_ARRAYNAME);
			strcat(input_buffers_queuing,"[POK_CONFIG_PART_NB_INPUT_QUEUING_PORTS] = {\n");
			strcpy(queuing_input_msg_data_buffers,"// input buffers used by the message pre load phase (queuing ports)\n");
			int j;
			int firstTime = 1;
			int buffer_index = 1;
			for (j=0; j<nb_ports; j++){
				debug_printf("nb_port %d\n", j);
				debug_printf("partition_id %d\n", partition_id);	
				debug_printf("ports[j].direction %d\n", ports[j].direction);	
				debug_printf("ports[j].kind %d\n", ports[j].kind);	
				if ((ports[j].partition_id == partition_id) &&
					(ports[j].direction == DESTINATION)&&
					(ports[j].kind == QUEUING)){
					if (!firstTime){
						strcat(input_buffers_queuing,", ");
					}
					debug_printf("input_buffers_queuing %s\n",input_buffers_queuing);
					firstTime=0;
					strcat(queuing_input_msg_data_buffers,"unsigned char input_queuing_buffer_");
					//strcat(queuing_input_msg_data_buffers,ports[j].receiving_var_name);
					strcat(queuing_input_msg_data_buffers, number_to_string[buffer_index]);
					strcat(queuing_input_msg_data_buffers,"_msgs [POK_CONFIG_PART_MAX_QUEUING_MESSAGES][POK_CONFIG_MAX_MESSAGE_SIZE];\n");
					strcat(queuing_input_msg_data_buffers,"unsigned char *input_queuing_buffer_");
					//strcat(queuing_input_msg_data_buffers,ports[j].receiving_var_name);
					strcat(queuing_input_msg_data_buffers, number_to_string[buffer_index]);
					strcat(queuing_input_msg_data_buffers,"[POK_CONFIG_PART_MAX_QUEUING_MESSAGES] = {\n");
					debug_printf("queuing_input_msg_data_buffers %s\n", queuing_input_msg_data_buffers);
					debug_printf("max_queueing_messages %d\n", max_queueing_messages);
					int k;
					for ( k=0; k<max_queueing_messages; k++){
						if (k>0){
							strcat(queuing_input_msg_data_buffers,",\n");
						}
						strcat(queuing_input_msg_data_buffers,"{");
						strcat(queuing_input_msg_data_buffers,"input_queuing_buffer_");
						//strcat(queuing_input_msg_data_buffers,ports[j].receiving_var_name);
						strcat(queuing_input_msg_data_buffers, number_to_string[buffer_index]);
						strcat(queuing_input_msg_data_buffers,"_msgs[");
						char tmp[2];
						sprintf(tmp,"%d",k);
						strcat(queuing_input_msg_data_buffers,tmp);
						strcat(queuing_input_msg_data_buffers,"]");
						strcat(queuing_input_msg_data_buffers,"}");
					}
					debug_printf("queuing_input_msg_data_buffers %s\n", queuing_input_msg_data_buffers);
					strcat(queuing_input_msg_data_buffers,"};\n\n");
					//strcat(input_buffers_queuing,"{");
					strcat(input_buffers_queuing,"input_queuing_buffer_");
					//strcat(input_buffers_queuing,ports[j].receiving_var_name);
					strcat(input_buffers_queuing, number_to_string[buffer_index]);
					debug_printf("input_buffers_queuing %s\n",input_buffers_queuing);
					buffer_index++;
				}		
			}
			//strcat(input_buffers_queuing,"}\n");
			strcat(input_buffers_queuing,"};\n");
			debug_printf("input_buffers_queuing %s\n",input_buffers_queuing);
//			debug_printf("partitions[partition_id].nb_of_input_queuing_port %d\n", partitions[partition_id].nb_of_input_queuing_port);	
			strcpy(partitions[partition_id].input_buffers_queuing,input_buffers_queuing);
			strcpy(partitions[partition_id].queuing_input_msg_data_buffers,queuing_input_msg_data_buffers);
		}
	}
	
	debug_printf("(partitions[partition_id].nb_of_sampling_port %d\n", partitions[partition_id].nb_of_sampling_port);
	if (partitions[partition_id].nb_of_sampling_port>0){
		debug_printf("(partitions[partition_id].nb_of_sampling_port\n");
		macro_partitions_enabled[partition_id][POK_NEEDS_ARINC653_SAMPLING] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_NEEDS_ARINC653_SAMPLING], one);
		macro_partitions_enabled[partition_id][POK_CONFIG_PART_NB_INPUT_SAMPLING_PORTS] = TRUE;
		char nb_of_input_sampling_port_string[3];
		sprintf(nb_of_input_sampling_port_string,"%d",partitions[partition_id].nb_of_input_sampling_port);
		strcpy(macro_partitions_values[partition_id][POK_CONFIG_PART_NB_INPUT_SAMPLING_PORTS], nb_of_input_sampling_port_string);
		
		if (partitions[partition_id].scheduler_type == O1){
			char input_buffers[10000];
			strcpy(input_buffers, "unsigned char *");
			strcat(input_buffers, DEFAULT_INPUT_BUFFERS_SAMPLING_ARRAYNAME);
			strcat(input_buffers, "[POK_CONFIG_PART_NB_INPUT_SAMPLING_PORTS ]= {\n");
			strcpy(partitions[partition_id].input_buffers,"");
			strcpy(partitions[partition_id].sampling_input_msg_data,"// input buffers used by the message pre load phase (sampling ports)\n");
			int j;

			int firstTime=1;
			int buffer_index = 1;
			for (j=0; j<nb_ports; j++){
				
				if ((ports[j].partition_id == partition_id) &&
					(ports[j].direction == DESTINATION)&&
					(ports[j].kind == SAMPLING)){
					if (!firstTime){
						strcat(input_buffers,",");
					}
					firstTime = 0;
					strcat(input_buffers,"{");
					/*strcat(input_buffers,ports[j].receiving_var_name);
					strcat(input_buffers,"_input_msg_data}");
					strcat(partitions[partition_id].sampling_input_msg_data,"unsigned char ");
					strcat(partitions[partition_id].sampling_input_msg_data,ports[j].receiving_var_name);
					strcat(partitions[partition_id].sampling_input_msg_data,"input_msg_data[POK_CONFIG_MAX_MESSAGE_SIZE];\n");
					*/
					strcat(input_buffers, "input_sampling_buffer_");
					strcat(input_buffers, number_to_string[buffer_index]);
					strcat(input_buffers, "}");
					strcat(partitions[partition_id].sampling_input_msg_data, "unsigned char input_sampling_buffer_");
					strcat(partitions[partition_id].sampling_input_msg_data, number_to_string[buffer_index]);
					strcat(partitions[partition_id].sampling_input_msg_data, "[POK_CONFIG_MAX_MESSAGE_SIZE];\n");
					buffer_index++;
				}		
			}
			strcat(input_buffers,"};\n");
			
			strcpy(partitions[partition_id].input_buffers, input_buffers);	
		}
	}
	
	if ((partitions[partition_id].nb_of_sampling_port>0) || 
		(partitions[partition_id].nb_of_queuing_port>0)){
		
		macro_partitions_enabled[partition_id][POK_CONFIG_MAX_MESSAGE_SIZE] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_CONFIG_MAX_MESSAGE_SIZE], max_message_size_string);
	
		macro_partitions_enabled[partition_id][POK_CONFIG_NB_SYSTEM_PORTS] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_CONFIG_NB_SYSTEM_PORTS], number_to_string[nb_ports]);
	
		if (partitions[partition_id].scheduler_type == O1){
	
			if ((partitions[partition_id].nb_of_input_sampling_port>0) || 
				(partitions[partition_id].nb_of_input_queuing_port>0)){		
				// building receiving_address
				char receiving_addresses[3000];
				strcpy(receiving_addresses,"// mapping between input ports and addresses of the variables used by the RECEIVE calls.\n");
				strcat(receiving_addresses,"MESSAGE_ADDR_TYPE *");
				strcat(receiving_addresses,DEFAULT_RECEIVING_ADDRESSES_ARRAYNAME);
				strcat(receiving_addresses,"[POK_CONFIG_NB_SYSTEM_PORTS] = \n{");
				strcpy(partitions[partition_id].receiving_variables,"// variables used by the RECEIVE calls\n");
				int j;
				for (j=0; j<nb_ports; j++){
					if (j>0){
						strcat(receiving_addresses,",");
					}
					if ((ports[j].partition_id == partition_id) &&
						(ports[j].direction == DESTINATION)){
						strcat(receiving_addresses,"&");
						strcat(receiving_addresses,ports[j].receiving_var_name);
						strcat(receiving_addresses,"_msg");
						strcat(partitions[partition_id].receiving_variables,"extern MESSAGE_ADDR_TYPE ");
						strcat(partitions[partition_id].receiving_variables,ports[j].receiving_var_name);
						strcat(partitions[partition_id].receiving_variables,"_msg");
						strcat(partitions[partition_id].receiving_variables,";\n");
	
					}else{
						strcat(receiving_addresses,"NULL");
					}
					
				}
				strcat(receiving_addresses,"};\n");
				strcpy(partitions[partition_id].receiving_addresses,receiving_addresses);		
			}
		}
	}
	
	
	if (max_queueing_messages>0){
		macro_partitions_enabled[partition_id][POK_CONFIG_PART_MAX_QUEUING_MESSAGES] = TRUE;	
		strcpy(macro_partitions_values[partition_id][POK_CONFIG_PART_MAX_QUEUING_MESSAGES],max_queueing_messages_string);
	}
	
	if (partitions[partition_id].nb_events>0){		
		macro_partitions_enabled[partition_id][POK_NEEDS_EVENTS] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_NEEDS_EVENTS], one);
		macro_partitions_enabled[partition_id][POK_CONFIG_NB_EVENTS] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_CONFIG_NB_EVENTS],number_to_string[partitions[partition_id].nb_events]);
		macro_partitions_enabled[partition_id][POK_NEEDS_ARINC653_EVENT] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_NEEDS_ARINC653_EVENT], one);
		macro_partitions_enabled[partition_id][POK_CONFIG_ARINC653_NB_EVENTS] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_CONFIG_ARINC653_NB_EVENTS],number_to_string[partitions[partition_id].nb_events]);
		// e.g. uint8_t pok_events_ids[1];
		strcpy(partitions[partition_id].events_ids_array,"EVENT_ID_TYPE ");
		strcat(partitions[partition_id].events_ids_array,DEFAULT_EVENTS_IDS_ARRAYNAME);
		strcat(partitions[partition_id].events_ids_array,"[");
		strcat(partitions[partition_id].events_ids_array,number_to_string[partitions[partition_id].nb_events]);
		strcat(partitions[partition_id].events_ids_array,"];\n");
		// e.g. char* pok_arinc653_events_names[POK_CONFIG_ARINC653_NB_EVENTS] = {"p1ev1"};
		strcpy(partitions[partition_id].events_names_array,"char* ");
		strcat(partitions[partition_id].events_names_array,DEFAULT_EVENTS_NAMES_ARRAYNAME);
		strcat(partitions[partition_id].events_names_array,"[POK_CONFIG_ARINC653_NB_EVENTS]");	
		strcpy(partitions[partition_id].events_names_array_values," = {");
		int i;
		for (i=0; i<partitions[partition_id].nb_events;i++){
			if (i>0){
				strcat(partitions[partition_id].events_names_array_values,",");
			}
			strcat(partitions[partition_id].events_names_array_values,"\"");
			strcat(partitions[partition_id].events_names_array_values,partitions[partition_id].events_names[i]);
			strcat(partitions[partition_id].events_names_array_values,"\"");
		}
		strcat(partitions[partition_id].events_names_array_values,"};");

/*	// uint8_t part1_evt[1];
	
		strcpy(partitions[partition_id].part_evt,"part");
		strcat(partitions[partition_id].part_evt,number_to_string[partition_id+1]);
		strcat(partitions[partition_id].part_evt,"_evt");
		strcpy(partitions[partition_id].events,"uint8_t ");
		strcat(partitions[partition_id].events,partitions[partition_id].part_evt);
		strcat(partitions[partition_id].events,"[");
		strcat(partitions[partition_id].events,number_to_string[partitions[partition_id].nb_events]);
		strcat(partitions[partition_id].events,"];");
		// char* pok_arinc653_events_names[POK_CONFIG_ARINC653_NB_EVENTS] = {"p1ev1"};
		strcpy(partitions[partition_id].events_names_array,
			"char* pok_arinc653_events_names[POK_CONFIG_ARINC653_NB_EVENTS] = {");	
		int i;
		for (i=0; i<partitions[partition_id].nb_events;i++){
			if (i>1){
				strcat(partitions[partition_id].events_names_array,",");
			}
			strcat(partitions[partition_id].events_names_array,"\"");
			strcat(partitions[partition_id].events_names_array,partitions[partition_id].events_names[i]);
			strcat(partitions[partition_id].events_names_array,"\"");
		}
		strcat(partitions[partition_id].events_names_array,"};");
*/
	}


	if (partitions[partition_id].nb_blackboards>0){		
		macro_partitions_enabled[partition_id][POK_NEEDS_BLACKBOARDS] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_NEEDS_BLACKBOARDS], one);
		macro_partitions_enabled[partition_id][POK_CONFIG_NB_BLACKBOARDS] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_CONFIG_NB_BLACKBOARDS],
		number_to_string[partitions[partition_id].nb_blackboards]);
		macro_partitions_enabled[partition_id][POK_NEEDS_ARINC653_BLACKBOARD] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_NEEDS_ARINC653_BLACKBOARD], one);
		//macro_partitions_enabled[partition_id][POK_NEEDS_PART_MIDDLEWARE] = TRUE;
		//strcpy(macro_partitions_values[partition_id][POK_NEEDS_PART_MIDDLEWARE], one);
		// e.g. uint8_t pok_blackboards_ids[1];
		strcpy(partitions[partition_id].blackboards_ids_array,"BLACKBOARD_ID_TYPE ");
		strcat(partitions[partition_id].blackboards_ids_array,DEFAULT_BLACKBOARDS_IDS_ARRAYNAME);
		strcat(partitions[partition_id].blackboards_ids_array,"[");
		strcat(partitions[partition_id].blackboards_ids_array,number_to_string[partitions[partition_id].nb_blackboards]);
		strcat(partitions[partition_id].blackboards_ids_array,"];\n");
		// e.g. char* pok_arinc653_blackboards_names[POK_CONFIG_ARINC653_NB_BLACKBOARDS] = {"p1ev1"};
		strcpy(partitions[partition_id].blackboards_names_array,"char* ");
		strcat(partitions[partition_id].blackboards_names_array,DEFAULT_BLACKBOARDS_NAMES_ARRAYNAME);
		strcat(partitions[partition_id].blackboards_names_array,"[POK_CONFIG_NB_BLACKBOARDS]");	
		strcpy(partitions[partition_id].blackboards_names_array_values," = {");
		int i;
		for (i=0; i<partitions[partition_id].nb_blackboards;i++){
			if (i>0){
				strcat(partitions[partition_id].blackboards_names_array_values,",");
			}
			strcat(partitions[partition_id].blackboards_names_array_values,"\"");
			strcat(partitions[partition_id].blackboards_names_array_values,partitions[partition_id].blackboards_names[i]);
			strcat(partitions[partition_id].blackboards_names_array_values,"\"");
		}
		strcat(partitions[partition_id].blackboards_names_array_values,"};");
	}

	if (partitions[partition_id].nb_buffers>0){		
		macro_partitions_enabled[partition_id][POK_NEEDS_BUFFERS] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_NEEDS_BUFFERS], one);
		macro_partitions_enabled[partition_id][POK_CONFIG_NB_BUFFERS] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_CONFIG_NB_BUFFERS],
		number_to_string[partitions[partition_id].nb_buffers]);
		macro_partitions_enabled[partition_id][POK_NEEDS_ARINC653_BUFFER] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_NEEDS_ARINC653_BUFFER], one);
		macro_partitions_enabled[partition_id][POK_CONFIG_MAX_BUFFERS_MESSAGES] = TRUE;
		strcpy(macro_partitions_values[partition_id][POK_CONFIG_MAX_BUFFERS_MESSAGES], 
						partitions[partition_id].max_buffers_msgs);
		//macro_partitions_enabled[partition_id][POK_NEEDS_PART_MIDDLEWARE] = TRUE;
		//strcpy(macro_partitions_values[partition_id][POK_NEEDS_PART_MIDDLEWARE], one);
		
		strcpy(partitions[partition_id].buffers_ids_array,"BUFFER_ID_TYPE ");
		strcat(partitions[partition_id].buffers_ids_array,DEFAULT_BUFFERS_IDS_ARRAYNAME);
		strcat(partitions[partition_id].buffers_ids_array,"[");
		strcat(partitions[partition_id].buffers_ids_array,number_to_string[partitions[partition_id].nb_buffers]);
		strcat(partitions[partition_id].buffers_ids_array,"];\n");
		// char* pok_arinc653_events_names[POK_CONFIG_ARINC653_NB_BUFFERS] = {"p1ev1"};
		strcpy(partitions[partition_id].buffers_names_array,"char* ");
		strcat(partitions[partition_id].buffers_names_array,DEFAULT_BUFFERS_NAMES_ARRAYNAME);
		strcat(partitions[partition_id].buffers_names_array,"[POK_CONFIG_NB_BUFFERS]");	
		strcpy(partitions[partition_id].buffers_names_array_values," = {");
		int i;
		for (i=0; i<partitions[partition_id].nb_buffers;i++){
			if (i>0){
				strcat(partitions[partition_id].buffers_names_array_values,",");
			}
			strcat(partitions[partition_id].buffers_names_array_values,"\"");
			strcat(partitions[partition_id].buffers_names_array_values,partitions[partition_id].buffers_names[i]);
			strcat(partitions[partition_id].buffers_names_array_values,"\"");
		}
		strcat(partitions[partition_id].buffers_names_array_values,"};");
	}

	debug_printf("before build_partition_files, partition id %d\n", partition_id);	
	if (build_partition_files(partition_id) != 0){
			printf("ERROR in build_partition_files \n");
			return -1;
	}
	
	return 0;
} 



int build_main_c_create_ports(int partition_id){

	if (partitions[partition_id].nb_of_sampling_port>0){
		strcat(partitions[partition_id].main_c,"\t// Sampling ports creation \n");
		// 	CREATE_SAMPLING_PORT ("PORT_PRIM2A_Servo_40          ", POK_CONFIG_MAX_MESSAGE_SIZE, SOURCE , 2000, &(pr1_SS_id), &(ret));
		struct port_list_item_t *tmp_part_ports = partitions[partition_id].ports;
		while(tmp_part_ports != NULL){
			if (ports[tmp_part_ports->port_id].kind == SAMPLING){
				strcat(partitions[partition_id].main_c,"\tCREATE_SAMPLING_PORT (\"");
				strcat(partitions[partition_id].main_c,ports[tmp_part_ports->port_id].name);
				strcat(partitions[partition_id].main_c,"\", POK_CONFIG_MAX_MESSAGE_SIZE,");
				if (ports[tmp_part_ports->port_id].direction == SOURCE){
					strcat(partitions[partition_id].main_c," SOURCE, ");
				}else{
					strcat(partitions[partition_id].main_c," DESTINATION, ");
				}
				strcat(partitions[partition_id].main_c,DEFAULT_SAMPLING_PORT_REFRESH_PERIOD);
				strcat(partitions[partition_id].main_c,", &(");
				strcat(partitions[partition_id].main_c,ports[tmp_part_ports->port_id].local_id);
				strcat(partitions[partition_id].main_c,"_id), &(ret));\n");
				strcat(partitions[partition_id].main_c,"\tprintf(\"\tSampling port created! id: ");
				strcat(partitions[partition_id].main_c,number_to_string[ports[tmp_part_ports->port_id].user_port_id]);
				strcat(partitions[partition_id].main_c,", name: ");
				strcat(partitions[partition_id].main_c,ports[tmp_part_ports->port_id].name);
				strcat(partitions[partition_id].main_c,", core id:%d, ret:%d\\n\", ");
 				strcat(partitions[partition_id].main_c,ports[tmp_part_ports->port_id].local_id);
 				strcat(partitions[partition_id].main_c,"_id, ret);\n");
			}
			tmp_part_ports = tmp_part_ports->next;
		}
		strcat(partitions[partition_id].main_c,"\n");
	}

	if (partitions[partition_id].nb_of_queuing_port>0){
		strcat(partitions[partition_id].main_c,"\t// Queuing ports creation \n");
		// 	 CREATE_QUEUING_PORT ("PORT_SEC3B_Frame1_120         ",           
        //          POK_CONFIG_MAX_MESSAGE_SIZE, POK_CONFIG_PART_MAX_QUEUING_MESSAGES, DESTINATION, 0, &(pr1_QDa_id), &(ret));
		struct port_list_item_t *tmp_part_ports = partitions[partition_id].ports;
		while(tmp_part_ports != NULL){
			if (ports[tmp_part_ports->port_id].kind == QUEUING){
				strcat(partitions[partition_id].main_c,"\tCREATE_QUEUING_PORT (\"");
				strcat(partitions[partition_id].main_c,ports[tmp_part_ports->port_id].name);
				strcat(partitions[partition_id].main_c,"\", POK_CONFIG_MAX_MESSAGE_SIZE, ");
				strcat(partitions[partition_id].main_c," POK_CONFIG_PART_MAX_QUEUING_MESSAGES, ");
				if (ports[tmp_part_ports->port_id].direction == SOURCE){
					strcat(partitions[partition_id].main_c,"SOURCE, ");
				}else{
					strcat(partitions[partition_id].main_c,"DESTINATION, ");
				}
				strcat(partitions[partition_id].main_c,DEFAULT_QUEUING_DISCIPLINE_TYPE);
				strcat(partitions[partition_id].main_c,", &(");
				strcat(partitions[partition_id].main_c,ports[tmp_part_ports->port_id].local_id);
				strcat(partitions[partition_id].main_c,"_id), &(ret));\n");
				strcat(partitions[partition_id].main_c,"\tprintf(\"\tQueueing port created! id: ");
				strcat(partitions[partition_id].main_c,number_to_string[ports[tmp_part_ports->port_id].user_port_id]);
				strcat(partitions[partition_id].main_c,", name: ");
				strcat(partitions[partition_id].main_c,ports[tmp_part_ports->port_id].name);
				strcat(partitions[partition_id].main_c,", core id:%d, ret:%d\\n\", ");
 				strcat(partitions[partition_id].main_c,ports[tmp_part_ports->port_id].local_id);
 				strcat(partitions[partition_id].main_c,"_id, ret);\n");


			}
			tmp_part_ports = tmp_part_ports->next;
		}
		strcat(partitions[partition_id].main_c,"\n");
	}	
	return 0;
}

void build_main_c_create_events_blackb_buffers(int partition_id) {
	
	int j;
	for (j=0; j<partitions[partition_id].nb_events; j++){
		// CREATE_EVENT (pok_arinc653_events_names[0], &(part_evt[0]), &(ret));
		strcat(partitions[partition_id].main_c,"\n\tCREATE_EVENT (");
		strcat(partitions[partition_id].main_c,DEFAULT_EVENTS_NAMES_ARRAYNAME);
		strcat(partitions[partition_id].main_c, "[");
		strcat(partitions[partition_id].main_c,number_to_string[j]);
		strcat(partitions[partition_id].main_c, "], &(");
		strcat(partitions[partition_id].main_c,DEFAULT_EVENTS_IDS_ARRAYNAME);
		strcat(partitions[partition_id].main_c, "[");
		strcat(partitions[partition_id].main_c,number_to_string[j]);
		strcat(partitions[partition_id].main_c, "]), &(ret));\n");		
		// printf("        Event %s created! Event id: %u, ret: %d \n", pok_arinc653_events_names[0], pok_arinc653_events_ids[0], ret);
		strcat(partitions[partition_id].main_c,"\tprintf(\"\tEvent %s created! EVENT ID: %u, ret: %d \\n\", ");
		strcat(partitions[partition_id].main_c,DEFAULT_EVENTS_NAMES_ARRAYNAME);
		strcat(partitions[partition_id].main_c, "[");
		strcat(partitions[partition_id].main_c,number_to_string[j]);
		strcat(partitions[partition_id].main_c, "], ");
		strcat(partitions[partition_id].main_c,DEFAULT_EVENTS_IDS_ARRAYNAME);
		strcat(partitions[partition_id].main_c, "[");
		strcat(partitions[partition_id].main_c,number_to_string[j]);
		strcat(partitions[partition_id].main_c, "], ret);\n");
	}
	if (partitions[partition_id].nb_events>0){
		strcat(partitions[partition_id].main_c, "\n");
	}
	for (j=0; j<partitions[partition_id].nb_blackboards; j++){
		// CREATE_BLACKBOARD ("input", 8*sizeof (int), &(pok_blackboards_ids[0]), &(ret));
		strcat(partitions[partition_id].main_c,"\n\tCREATE_BLACKBOARD (");
		strcat(partitions[partition_id].main_c,DEFAULT_BLACKBOARDS_NAMES_ARRAYNAME);
		strcat(partitions[partition_id].main_c, "[");
		strcat(partitions[partition_id].main_c,number_to_string[j]);
		strcat(partitions[partition_id].main_c, "], blackboards_max_message_size, &(");
		strcat(partitions[partition_id].main_c, DEFAULT_BLACKBOARDS_IDS_ARRAYNAME);
		strcat(partitions[partition_id].main_c, "[");
		strcat(partitions[partition_id].main_c, number_to_string[j]);
		strcat(partitions[partition_id].main_c, "]), &(ret));\n");
		strcat(partitions[partition_id].main_c,"\tprintf(\"\tBlackboard %s created! BLACKBOARD ID: %u, ret: %d \\n\", ");
		strcat(partitions[partition_id].main_c,DEFAULT_BLACKBOARDS_NAMES_ARRAYNAME);
		strcat(partitions[partition_id].main_c, "[");
		strcat(partitions[partition_id].main_c,number_to_string[j]);
		strcat(partitions[partition_id].main_c, "], ");
		strcat(partitions[partition_id].main_c,DEFAULT_BLACKBOARDS_IDS_ARRAYNAME);
		strcat(partitions[partition_id].main_c, "[");
		strcat(partitions[partition_id].main_c,number_to_string[j]);
		strcat(partitions[partition_id].main_c, "], ret);\n");
	}
	if (partitions[partition_id].nb_blackboards>0){
		strcat(partitions[partition_id].main_c, "\n");
	}

	for (j=0; j<partitions[partition_id].nb_buffers; j++){
		//   CREATE_BUFFER ("input", 256/*8*sizeof (int)*/, 3, FIFO, &(input_id), &(ret));
		strcat(partitions[partition_id].main_c,"\n\tCREATE_BUFFER (");
		strcat(partitions[partition_id].main_c,DEFAULT_BUFFERS_NAMES_ARRAYNAME);
		strcat(partitions[partition_id].main_c, "[");
		strcat(partitions[partition_id].main_c,number_to_string[j]);
		strcat(partitions[partition_id].main_c, "], buffers_max_message_size, POK_CONFIG_MAX_BUFFERS_MESSAGES, FIFO,  &(");
		strcat(partitions[partition_id].main_c, DEFAULT_BUFFERS_IDS_ARRAYNAME);
		strcat(partitions[partition_id].main_c, "[");
		strcat(partitions[partition_id].main_c, number_to_string[j]);
		strcat(partitions[partition_id].main_c, "]), &(ret));\n");
		strcat(partitions[partition_id].main_c,"\tprintf(\"\tBuffer %s created! BUFFER ID: %u, ret: %d \\n\", ");
		strcat(partitions[partition_id].main_c,DEFAULT_BUFFERS_NAMES_ARRAYNAME);
		strcat(partitions[partition_id].main_c, "[");
		strcat(partitions[partition_id].main_c,number_to_string[j]);
		strcat(partitions[partition_id].main_c, "], ");
		strcat(partitions[partition_id].main_c,DEFAULT_BUFFERS_IDS_ARRAYNAME);
		strcat(partitions[partition_id].main_c, "[");
		strcat(partitions[partition_id].main_c,number_to_string[j]);
		strcat(partitions[partition_id].main_c, "], ret);\n");
	}
	if (partitions[partition_id].nb_buffers>0){
		strcat(partitions[partition_id].main_c, "\n");
	}
}

int build_main_c(int partition_id){

	int thread_nb = 0;


	// create ports		
	if (build_main_c_create_ports(partition_id)!=0){
			return -1;
	}
	
	// create events/blackboards/buffer
	build_main_c_create_events_blackb_buffers(partition_id);

	// create threads 
	debug_printf("nb of threads %d\n",partitions[partition_id].nb_of_threads);
	for (thread_nb=1; thread_nb<partitions[partition_id].nb_of_threads; thread_nb++){
		thread_t thread = partitions[partition_id].threads[thread_nb-1];
		debug_printf("thread type %d, thread nb %d\n",thread.type,thread_nb);
			switch ( thread.type ) {
			case NORMAL:
			case EVENT_SET:
			case EVENT_RESUME:
			case BB_DISPLAY:
			case BB_RESUME:
			case BUFFER_RESUME_SENDER:
			case BUFFER_RESUME_RECEIVER:
				build_main_c_normal_thread (partition_id, thread_nb);
				//printf("entry point: %s\n",thread.entry_point);
				break;
			case EVENT_RESET:
            case EVENT_WAIT:
            case BB_SUSPEND:
            case BB_READ:
            case BUFFER_SEND:
            case BUFFER_RECEIVE:
				build_main_c_aperiodic_thread (partition_id, thread_nb);
				break;
			case EVENT_SETTER:
				build_main_c_set_thread (partition_id, thread_nb);
				debug_printf("setter entry point: %s\n",partitions[partition_id].threads[thread_nb-1].entry_point);
				break;
			case EVENT_PREDECESSOR:
				build_main_c_pre_wait_thread (partition_id, thread_nb);
				debug_printf("pre_wait entry point: %s\n",partitions[partition_id].threads[thread_nb-1].entry_point);
				break;
			case EVENT_SUCCESSOR:
				build_main_c_succ_wait_thread (partition_id, thread_nb);
				break;
				
		}
	}


	strcat(partitions[partition_id].main_c,"\n");

	for (thread_nb=1; thread_nb<partitions[partition_id].nb_of_threads; thread_nb++){
		strcat(partitions[partition_id].main_c,"\tSTART (arinc_threads[");
		strcat(partitions[partition_id].main_c,number_to_string[thread_nb]);
		strcat(partitions[partition_id].main_c,"],&(ret));\n");
	}
	
	strcat(partitions[partition_id].main_c,"\n\tSET_PARTITION_MODE (NORMAL, &(ret));");

	return 0;
}


int build_activity(int partition_id){

	strcpy(partitions[partition_id].activity_h,"");
	debug_printf("in build activity\n");
	char tmp_entry_point[20];
	//activity.c and activity.h
	int thread_number;	
	for (thread_number = 1; thread_number < partitions[partition_id].nb_of_threads; thread_number++) {
		
		thread_t thread = partitions[partition_id].threads[thread_number-1];
		debug_printf("thread type %d\n", thread.type);
		//debug_printf("thread type %d\n", thread.type);
		
		switch ( thread.type ) {
		// periodic threads
		case NORMAL:
		case EVENT_SET:
		case EVENT_RESUME:
		case BB_RESUME:
		case BB_DISPLAY:
		case BUFFER_RESUME_SENDER:
		case BUFFER_RESUME_RECEIVER:
		case EVENT_SETTER:
		case EVENT_PREDECESSOR:
			build_activity_periodic_thread(partition_id, thread_number);
			break;
		// aperiodic threads
		case EVENT_RESET:
			build_activity_thread_event_reset(partition_id, thread);
			break;
		case EVENT_WAIT:
			build_activity_thread_event_wait(partition_id, thread_number);
			break;
		case BB_SUSPEND:
			build_activity_thread_blackboard_suspend(partition_id, thread_number);
			break;
		case BB_READ:
			build_activity_thread_blackboard_read(partition_id, thread_number);
			break;
		case BUFFER_SEND:
			build_activity_thread_buffer_send(partition_id, thread_number);
			break;
		case BUFFER_RECEIVE:
			build_activity_thread_buffer_receive(partition_id, thread_number);
			break;
		case EVENT_SUCCESSOR:
			build_activity_thread_successor(partition_id, thread);
			break;
		default:
			printf("ERROR: no action for thread type: %d\n", thread.type);
		}
	}
	
	debug_printf("after all threads\n");

	return 0;
}

// build files: main.c, activity.c, activity.h
int build_partition_files(int partition_id){

	if (build_init(partition_id) != 0){
		return -1;
	}
	
	if (build_main_c(partition_id) != 0){
		return -1;
	}
	
	if (build_activity(partition_id) != 0){
		return -1;
	}

	return 0;
}


int build_init(int partition_id){
	
	// main.c
	strcpy(partitions[partition_id].main_c,"#include \"activity.h\"\n");
	strcat(partitions[partition_id].main_c,"#include <arinc653/types.h>\n");
	strcat(partitions[partition_id].main_c,"#include <arinc653/process.h>\n");
	strcat(partitions[partition_id].main_c,"#include <arinc653/partition.h>\n");
	strcat(partitions[partition_id].main_c,"#include \"deployment.h\"\n");
	if (partitions[partition_id].nb_of_sampling_port>0){
		strcat(partitions[partition_id].main_c,"#include <arinc653/sampling.h>\n");
	}
	if (partitions[partition_id].nb_of_queuing_port>0){
		strcat(partitions[partition_id].main_c,"#include <arinc653/queueing.h>\n");
	}
	
	if (partitions[partition_id].nb_events>0){
		strcat(partitions[partition_id].main_c,"#include <arinc653/event.h>\n");	
	}

	if (partitions[partition_id].nb_blackboards>0){
		strcat(partitions[partition_id].main_c,"#include <arinc653/blackboard.h>\n");	
	}

	if (partitions[partition_id].nb_buffers>0){
		strcat(partitions[partition_id].main_c,"#include <arinc653/buffer.h>\n");	
	}

	strcat(partitions[partition_id].main_c,"\n\n");
	strcat(partitions[partition_id].main_c,"PROCESS_ID_TYPE ");
	strcat(partitions[partition_id].main_c, DEFAULT_ARINC_THREADS_ARRAYNAME);
	strcat(partitions[partition_id].main_c,"[POK_CONFIG_NB_THREADS];\n\n");

	if (partitions[partition_id].nb_events>0){
		
		strcat(partitions[partition_id].main_c,"extern ");
		strcat(partitions[partition_id].main_c,partitions[partition_id].events_ids_array);
		strcat(partitions[partition_id].main_c, "extern ");
		strcat(partitions[partition_id].main_c,partitions[partition_id].events_names_array);
		//strcat(partitions[partition_id].main_c,partitions[partition_id].events_names_array_values);
		strcat(partitions[partition_id].main_c, ";\n");
	}

	if (partitions[partition_id].nb_blackboards>0){
		
		strcat(partitions[partition_id].main_c,"extern ");
		strcat(partitions[partition_id].main_c,partitions[partition_id].blackboards_ids_array);
		strcat(partitions[partition_id].main_c, "extern ");
		strcat(partitions[partition_id].main_c,partitions[partition_id].blackboards_names_array);
		//strcat(partitions[partition_id].main_c,partitions[partition_id].blackboards_names_array_values);
		strcat(partitions[partition_id].main_c, ";\n");
	}

	if (partitions[partition_id].nb_buffers>0){
		
		strcat(partitions[partition_id].main_c,"extern ");
		strcat(partitions[partition_id].main_c,partitions[partition_id].buffers_ids_array);
		strcat(partitions[partition_id].main_c, "extern ");
		strcat(partitions[partition_id].main_c,partitions[partition_id].buffers_names_array);
		//strcat(partitions[partition_id].main_c,partitions[partition_id].buffers_names_array_values);
		strcat(partitions[partition_id].main_c, ";\n");
	}

	// activity.c
	strcpy(partitions[partition_id].activity_c,"#include <arinc653/types.h>\n");
	strcat(partitions[partition_id].activity_c,"#include <arinc653/time.h>\n"); // reference to PERIODIC_WAIT function
//	strcat(partitions[partition_id].activity_c,"#include <core/time.h>\n\n"); 
	if (partitions[partition_id].nb_of_sampling_port>0){
		strcat(partitions[partition_id].activity_c,"#include <arinc653/sampling.h>\n");
	}
	if (partitions[partition_id].nb_of_queuing_port>0){
		strcat(partitions[partition_id].activity_c,"#include <arinc653/queueing.h>\n");
	}

	if (partitions[partition_id].nb_events_blackb_buffers > 0){
		strcat(partitions[partition_id].activity_c,"#include <arinc653/process.h>\n");	
	}

	if (partitions[partition_id].nb_events > 0){
		strcat(partitions[partition_id].activity_c,"#include <arinc653/event.h>\n");	
	}

	if (partitions[partition_id].nb_blackboards > 0){
		strcat(partitions[partition_id].activity_c,"#include <arinc653/blackboard.h>\n");	
	}

	if (partitions[partition_id].nb_buffers>0){
		strcat(partitions[partition_id].activity_c,"#include <arinc653/buffer.h>\n");	
	}

	strcat(partitions[partition_id].activity_c,"#include \"deployment.h\"\n");

	strcat(partitions[partition_id].activity_c,"\n\n");

	if (partitions[partition_id].nb_events > 0){
		strcat(partitions[partition_id].activity_c,"extern ");
		strcat(partitions[partition_id].activity_c,partitions[partition_id].events_ids_array);
	}

	if (partitions[partition_id].nb_blackboards > 0){
		strcat(partitions[partition_id].activity_c,"extern ");
		strcat(partitions[partition_id].activity_c,partitions[partition_id].blackboards_ids_array);
	}

	if (partitions[partition_id].nb_buffers > 0){
		strcat(partitions[partition_id].activity_c,"extern ");
		strcat(partitions[partition_id].activity_c,partitions[partition_id].buffers_ids_array);
	}

	if (partitions[partition_id].nb_events_blackb_buffers>0){
		// extern EVENT_ID_TYPE part1_evt[1];
		// extern char* pok_arinc653_events_names[POK_CONFIG_ARINC653_NB_EVENTS];	
		strcat(partitions[partition_id].activity_c,"\nextern PROCESS_ID_TYPE ");
		strcat(partitions[partition_id].activity_c,DEFAULT_ARINC_THREADS_ARRAYNAME);
		strcat(partitions[partition_id].activity_c,"[POK_CONFIG_NB_THREADS];\n\n");
	}

	if (partitions[partition_id].nb_blackboards > 0){
		strcat(partitions[partition_id].activity_c,"int blackboards_values[8] = {0,1,2,3,4,5,6,7};\n\n");
	}

	if (partitions[partition_id].nb_buffers > 0){
		strcat(partitions[partition_id].activity_c,"int buffers_values[8] = {10,11,12,13,14,15,16,17};\n\n");
	}

	// sampling ports ids, msg and msg size
	if (partitions[partition_id].nb_of_sampling_port>0){
	struct port_list_item_t *tmp_part_ports = partitions[partition_id].ports;
		strcat(partitions[partition_id].main_c,"// Sampling ports ids\n");
		strcat(partitions[partition_id].activity_c,"// Sampling ports ids, msg and msg size\n");
		while(tmp_part_ports != NULL){
			if (ports[tmp_part_ports->port_id].kind == SAMPLING){
				strcat(partitions[partition_id].main_c,"SAMPLING_PORT_ID_TYPE ");
				strcat(partitions[partition_id].main_c,ports[tmp_part_ports->port_id].local_id);
				strcat(partitions[partition_id].main_c,"_id;\n");
				strcat(partitions[partition_id].activity_c,"extern SAMPLING_PORT_ID_TYPE ");
				strcat(partitions[partition_id].activity_c,ports[tmp_part_ports->port_id].local_id);
				strcat(partitions[partition_id].activity_c,"_id;\n");
				strcat(partitions[partition_id].activity_c,"MESSAGE_ADDR_TYPE ");
				strcat(partitions[partition_id].activity_c,ports[tmp_part_ports->port_id].local_id);
				strcat(partitions[partition_id].activity_c,"_msg;\n");
				strcat(partitions[partition_id].activity_c,"MESSAGE_SIZE_TYPE ");
				strcat(partitions[partition_id].activity_c,ports[tmp_part_ports->port_id].local_id);
				strcat(partitions[partition_id].activity_c,"_msg_size;\n\n");
			}
			tmp_part_ports = tmp_part_ports->next;
		}
	}

	if (partitions[partition_id].nb_of_queuing_port>0){
		struct port_list_item_t *tmp_part_ports = partitions[partition_id].ports;
		strcat(partitions[partition_id].main_c,"// Queuing ports ids\n");
		strcat(partitions[partition_id].activity_c,"// Queuing ports ids, msg and msg size\n");
		while(tmp_part_ports != NULL){
			if (ports[tmp_part_ports->port_id].kind == QUEUING){
				strcat(partitions[partition_id].main_c,"QUEUING_PORT_ID_TYPE ");
				strcat(partitions[partition_id].main_c,ports[tmp_part_ports->port_id].local_id);
				strcat(partitions[partition_id].main_c,"_id;\n");
				strcat(partitions[partition_id].activity_c,"extern QUEUING_PORT_ID_TYPE ");
				strcat(partitions[partition_id].activity_c,ports[tmp_part_ports->port_id].local_id);
				strcat(partitions[partition_id].activity_c,"_id;\n");
				strcat(partitions[partition_id].activity_c,"MESSAGE_ADDR_TYPE ");
				strcat(partitions[partition_id].activity_c,ports[tmp_part_ports->port_id].local_id);
				strcat(partitions[partition_id].activity_c,"_msg;\n");
				strcat(partitions[partition_id].activity_c,"MESSAGE_SIZE_TYPE ");
				strcat(partitions[partition_id].activity_c,ports[tmp_part_ports->port_id].local_id);
				strcat(partitions[partition_id].activity_c,"_msg_size;\n\n");
			}
			tmp_part_ports = tmp_part_ports->next;
		}
	}

	strcat(partitions[partition_id].main_c,"\n");
	strcat(partitions[partition_id].activity_c,"\n");
	
	strcat(partitions[partition_id].main_c,"int main ()\n{\n\n");
	strcat(partitions[partition_id].main_c,"\tPROCESS_ATTRIBUTE_TYPE tattr;\n");
	strcat(partitions[partition_id].main_c,"\tRETURN_CODE_TYPE ret;\n");
	if (partitions[partition_id].nb_blackboards > 0){
		strcat(partitions[partition_id].main_c,"\tuint8_t blackboards_max_message_size = ");
		strcat(partitions[partition_id].main_c,partitions[partition_id].blackboards_max_message_size);
		strcat(partitions[partition_id].main_c,";\n");
	}
	if (partitions[partition_id].nb_buffers > 0){
		strcat(partitions[partition_id].main_c,"\tuint8_t buffers_max_message_size = ");
		strcat(partitions[partition_id].main_c,partitions[partition_id].buffers_max_message_size);
		strcat(partitions[partition_id].main_c,";\n");
	}
	strcat(partitions[partition_id].main_c,"\n");
	strcat(partitions[partition_id].main_c,"\tprintf(\"\t");
	strcat(partitions[partition_id].main_c,partitions[partition_id].part_name);
	strcat(partitions[partition_id].main_c," - Main thread\\n\");\n");
	
	return 0;
}


int build_read_ports_activity_c(int partition_id){
	// READ_SAMPLING_MESSAGE (pr1_SDa_id, pr1_SDa_msg, &pr1_SDa_msg_size, &(valid), &(ret));
	if (partitions[partition_id].nb_of_input_sampling_port>0){
		strcat(partitions[partition_id].activity_c,"\t\t//Reading input sampling ports...\n");
		struct port_list_item_t *tmp_input_ports = partitions[partition_id].ports;
		while(tmp_input_ports != NULL){
			if ((ports[tmp_input_ports->port_id].kind == SAMPLING)
				&& (ports[tmp_input_ports->port_id].direction == DESTINATION)){
				read_sampling_message(partition_id,tmp_input_ports);
			}
			tmp_input_ports = tmp_input_ports->next;
		}
	}

	//  RECEIVE_QUEUING_MESSAGE (pr1_QDa_id, 0, pr1_QDa_msg,&size_tmp, &(ret));
	if (partitions[partition_id].nb_of_input_queuing_port>0){
		strcat(partitions[partition_id].activity_c,"\t\t//Receiving input queuing ports...\n");
	
		struct port_list_item_t *tmp_input_ports = partitions[partition_id].ports;
		while(tmp_input_ports != NULL){
			if ((ports[tmp_input_ports->port_id].kind == QUEUING) 
				&& (ports[tmp_input_ports->port_id].direction == DESTINATION)){
				receive_queuing_message(partition_id, tmp_input_ports);
			}
			tmp_input_ports = tmp_input_ports->next;
		}
	}
	return 0;
}


int build_write_ports_activity_c(int partition_id){

//printf("///////////////////////////////// IN BUILD WRITE PORTSSSSSSSSSSS \n");
	
	if (partitions[partition_id].nb_of_sampling_port>0){
		struct port_list_item_t *tmp_output_ports = partitions[partition_id].ports;	
		while(tmp_output_ports != NULL){
			if ((ports[tmp_output_ports->port_id].kind == SAMPLING)
			&& (ports[tmp_output_ports->port_id].direction == SOURCE)){
				// WRITE_SAMPLING_MESSAGE (pr1_SS_id, pr1_SS_msg, pr1_SS_msg_size, &(ret));
				write_sampling_message(partition_id, tmp_output_ports);
			}
			tmp_output_ports = tmp_output_ports->next;
		}//while
	}//if

	if (partitions[partition_id].nb_of_queuing_port>0){
		struct port_list_item_t *tmp_output_ports = partitions[partition_id].ports;
		while(tmp_output_ports != NULL){
			if ((ports[tmp_output_ports->port_id].kind == QUEUING)
				&& (ports[tmp_output_ports->port_id].direction == SOURCE)){
				//     SEND_QUEUING_MESSAGE (pr1_QS_id, msg_to_be_written1 , sizeof(unsigned char),0, &(ret))
				send_queuing_message(partition_id,tmp_output_ports);
			}
			tmp_output_ports = tmp_output_ports->next;
		}
	}
	debug_printf("queuing: %d, sampling %d\n",partitions[partition_id].nb_of_input_sampling_port,
		partitions[partition_id].nb_of_input_queuing_port);
	debug_printf("IN BUILD WRITE PORTSSSSSSSSSSS %s\n",partitions[partition_id].activity_c);
	return 0;
}

int build_ports_configuration (){

	// build PORTS macro
	if (nb_ports>0){
		
		macro_enabled[POK_CONFIG_NB_GLOBAL_PORTS] = TRUE;
		macro_enabled[POK_CONFIG_NB_PORTS]  = TRUE;
		macro_enabled[POK_CONFIG_PARTITIONS_PORTS] = TRUE;
	

		strcpy(macro_values[POK_CONFIG_NB_PORTS],number_to_string[nb_ports]);
		macro_enabled[POK_CONFIG_NB_PORTS] = TRUE;

		strcpy(macro_values[POK_CONFIG_NB_GLOBAL_PORTS],number_to_string[nb_ports]);
		macro_enabled[POK_CONFIG_NB_GLOBAL_PORTS] = TRUE;
	
		if (nb_of_ports_sampling >0){
			macro_enabled[POK_NEEDS_PORTS_SAMPLING] = TRUE;	
			strcpy(macro_values[POK_NEEDS_PORTS_SAMPLING],"1");
		}

		if (nb_of_ports_queuing >0){
			macro_enabled[POK_NEEDS_PORTS_QUEUEING] = TRUE;	
			strcpy(macro_values[POK_NEEDS_PORTS_QUEUEING],"1");
			macro_enabled[POK_CONFIG_MAX_QUEUING_MESSAGES] = TRUE;	
			strcpy(macro_values[POK_CONFIG_MAX_QUEUING_MESSAGES],max_queueing_messages_string);
		}


		char pok_config_partitions_ports[60];
		strcpy(pok_config_partitions_ports,"{");

		strcat(pok_port_local_identifier_t,"typedef enum\n{\n");
		strcat(pok_port_identifier_t,"typedef enum\n{\n");

		strcat(pok_global_ports_to_local_ports,"uint8_t pok_global_ports_to_local_ports[POK_CONFIG_NB_GLOBAL_PORTS] = {\n\t\t\t");
		strcat(pok_local_ports_to_global_ports,"uint8_t pok_local_ports_to_global_ports[POK_CONFIG_NB_PORTS] = {\n\t\t\t");
		strcat(pok_ports_names,"char* pok_ports_names[POK_CONFIG_NB_PORTS] = {\n\t\t\t");
		strcat(pok_ports_nb_destinations,"// Number of DESTINATION ports for each partition\n");
		strcat(pok_ports_nb_destinations,"uint8_t pok_ports_nb_destinations[POK_CONFIG_NB_PORTS] = {");
		strcat(pok_ports_kind,"// Configuration-time KIND attribute for each port\n");
		strcat(pok_ports_kind,"uint8_t		pok_ports_kind[POK_CONFIG_NB_PORTS]  = {\n\t\t\t");
		strcpy(pok_ports_destinations,"// Array of destination ports for each port (destination ports must be NULL)\n");
		strcat(pok_ports_destinations,"uint8_t* 	pok_ports_destinations[POK_CONFIG_NB_PORTS] = {\n\t\t\t");
		
		strcpy(deployment_destinations,"");
		
		int i;
		for (i=0; i<nb_ports; i++){
			if (i>0){
				strcat(pok_config_partitions_ports,",");
				strcat(pok_port_local_identifier_t,",\n");
		 		strcat(pok_port_identifier_t,",\n");
		 		strcat(pok_global_ports_to_local_ports,",\n\t\t\t");
		 		strcat(pok_local_ports_to_global_ports,",\n\t\t\t");
		 		strcat(pok_ports_names,",\n\t\t\t");
		 		strcat(pok_ports_nb_destinations,", ");
		 		strcat(pok_ports_kind,",\n\t\t\t");
		 		strcat(pok_ports_destinations,",\n\t\t\t");		 		
			}

			char partition_id_string[3];
			sprintf(partition_id_string,"%d",ports[i].partition_id);
			strcat(pok_config_partitions_ports,partition_id_string);

			strcat(pok_port_local_identifier_t,"\t");
			strcat(pok_port_local_identifier_t,ports[i].local_id);
			strcat(pok_port_local_identifier_t," = ");
			char string_i[2];
			sprintf(string_i,"%d",i);
			strcat(pok_port_local_identifier_t,string_i);
	
			strcat(pok_port_identifier_t,"\t");
			strcat(pok_port_identifier_t,ports[i].global_id);
			strcat(pok_port_identifier_t," = ");
			strcat(pok_port_identifier_t,string_i);
			
			port_numberic_id_t tmp_global_id = ports[i].global_numeric_id;
			strcat(pok_global_ports_to_local_ports,ports[tmp_global_id].local_id);
			
			port_numberic_id_t tmp_local_id = ports[i].local_numeric_id;
			strcat(pok_local_ports_to_global_ports,ports[tmp_local_id].global_id);
			
			strcat(pok_ports_names,"\"");
			strcat(pok_ports_names,ports[i].name);
			strcat(pok_ports_names,"\"");
			
			
			if (ports[i].kind == SAMPLING){
				strcat(pok_ports_kind,"POK_PORT_KIND_SAMPLING");
			}else if (ports[i].kind == QUEUING){
				strcat(pok_ports_kind,"POK_PORT_KIND_QUEUEING");
			}
			
			char tmp_dest[2];
			sprintf(tmp_dest,"%d",ports[i].nb_of_destinations);
			strcat(pok_ports_nb_destinations,tmp_dest);
			
			
			if (ports[i].nb_of_destinations > 0){
				
				
				struct port_list_item_t *tmp_dest_ptr = ports[i].destinations;
				strcat(deployment_destinations,"uint8_t ");
				strcat(deployment_destinations,ports[i].local_id);
				strcat(deployment_destinations,"_deployment_destinations [");
				strcat(deployment_destinations,number_to_string[ports[i].nb_of_destinations]);
				strcat(deployment_destinations,"] = {");
				
				strcat(pok_ports_destinations,ports[i].local_id);
				strcat(pok_ports_destinations,"_deployment_destinations");
				int j=0;
				while(tmp_dest_ptr != NULL){
					if (j>0){
						strcat(deployment_destinations,",");
					}
					strcat(deployment_destinations,ports[tmp_dest_ptr->port_id].global_id);
					tmp_dest_ptr = tmp_dest_ptr->next;
					j++;
				}
				strcat(deployment_destinations,"};\n");
			}else{
				strcat(pok_ports_destinations,"NULL");			
			}
			
		}

		strcat(pok_config_partitions_ports, "}");
		strcat(pok_port_identifier_t, "\n} pok_port_identifier_t;");
		strcat(pok_port_local_identifier_t, "\n} pok_port_local_identifier_t;");

		strcpy(macro_values[POK_CONFIG_PARTITIONS_PORTS],pok_config_partitions_ports);
		debug_printf("pok_config_partitions_PORTS %s\n",macro_values[POK_CONFIG_PARTITIONS_PORTS]);

		strcat(pok_global_ports_to_local_ports,"};");
		strcat(pok_local_ports_to_global_ports,"};");
		strcat(pok_ports_names,"};");
		strcat(pok_ports_nb_destinations,"};");
		strcat(pok_ports_kind,"};");
		strcat(pok_ports_destinations,"};");
		

	} // if nb_ports > 0

	return 0;
}

int read_default_port_messages(const char *filename){

	FILE *file = fopen ( filename, "r" );
	if ( file != NULL ){
		char line [3000];
		while ( fgets ( line, sizeof line, file ) != NULL ){
			char *newline = strchr ( line, '\n' );	
			if ( (newline != NULL) && (line[0]=='{') && (strlen(line)>2) ){
				*newline = '\0';
				fputs ( line, stdout );
				printf("\n\n");
			}
		}
		fclose ( file );
	}else{
		perror ( filename );
	}
	return 0;
}

