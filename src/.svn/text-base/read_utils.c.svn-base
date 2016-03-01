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
#include <assert.h>

#include "arinc653-xml-conf.h"
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/xmlschemas.h>

extern char *scheduler_types[NB_SCHEDULER_TYPES];
extern xmlDocPtr doc;
extern xmlXPathContextPtr xpathCtx;
extern int major_frame;
extern int nb_of_sporadic_threads;
extern int priority_for_aperiodic;

xmlChar *xpathExpr;	
xmlXPathObjectPtr xpathObj_local;
char expression[100];

/*

In this file they are implemented functions used in the file read_xml.c

*/

int get_number_of_threads_elem(char *base_expression, int partition_id){

	char expression[100];
	xmlXPathObjectPtr xpathObj_local;
	xmlChar *xpathExpr;

	strcpy(expression,base_expression);
	debug_printf("expression: %s\n",expression);
	strcat(expression, "/nb_of_threads");
	debug_printf("expression: %s\n",expression);
	xpathExpr = BAD_CAST expression;
	debug_printf("xpathE %s\n",xpathExpr);

 	if (getNode(xpathExpr,&xpathObj_local) != 0){
		return -1;
	}

	partitions[partition_id].nb_of_threads = 1 + atoi(xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[0]));
	debug_printf("getContent %d\n", partitions[partition_id].nb_of_threads);
	
	return 0;
}


int get_scheduler_elem(char *base_expression, int partition_id, xmlXPathObjectPtr xpathObj){

	xmlXPathObjectPtr xpathObj_local;
	char expression[100];
	
	strcpy(expression,base_expression);
	strcat(expression, "/scheduler");
	debug_printf("expression: %s\n",expression);

	xmlChar *xpathExpr =  BAD_CAST expression;
	debug_printf("xpathE %s\n",xpathExpr);

 	if (getNode(xpathExpr,&xpathObj_local) != 0){
		return -1;
	}

	if (xpathObj_local->nodesetval->nodeNr == 0){
		debug_printf("scheduler element not present\n");
		partitions[partition_id].scheduler_type = O1;
		strcpy(macro_values[POK_NEEDS_SCHED_O1],"1");
		macro_enabled[POK_NEEDS_SCHED_O1] = TRUE;
	}else{
		debug_printf("Scheduler element value %s\n",xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));	
		char *partition_scheduler_type = xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[0]);
		debug_printf("GetContent %s\n", partition_scheduler_type);
		
		if (strcmp(partition_scheduler_type,scheduler_types[O1]) == 0){
			partitions[partition_id].scheduler_type = O1;	
			debug_printf("Scheduler O1\n");
			strcpy(macro_values[POK_NEEDS_SCHED_O1],"1");
			macro_enabled[POK_NEEDS_SCHED_O1] = TRUE;
		}else if (strcmp(partition_scheduler_type,scheduler_types[FPPS]) == 0){
			debug_printf("Scheduler FPPS\n");
			partitions[partition_id].scheduler_type = FPPS;
			strcpy(macro_values[POK_NEEDS_SCHED_FPPS],"1");
			macro_enabled[POK_NEEDS_SCHED_FPPS] = TRUE;
		}else if (strcmp(partition_scheduler_type,scheduler_types[O1_SPLIT]) == 0){
			debug_printf("Scheduler O1_SPLIT\n");
			partitions[partition_id].scheduler_type = O1_SPLIT;
			strcpy(macro_values[POK_NEEDS_SCHED_O1_SPLIT],"1");
			macro_enabled[POK_NEEDS_SCHED_O1_SPLIT] = TRUE;
			strcpy(macro_values[POK_NEEDS_SCHED_O1],"1");
			macro_enabled[POK_NEEDS_SCHED_O1] = TRUE;
		}
	}

	return 0;
}

int get_size_elem(char *base_expression, int partition_id){

		xmlXPathObjectPtr xpathObj_local;
		char expression[100];
	

		strcpy(expression,base_expression);
		debug_printf("expression: %s\n",expression);
 		strcat(expression, "/size");
		debug_printf("expression: %s\n",expression);
		xmlChar *xpathExpr =  BAD_CAST expression;
		debug_printf("xpathE %s\n",xpathExpr);


	 	if (getNode(xpathExpr,&xpathObj_local) != 0){
			return -1;
		}

		if ((xpathObj_local != NULL) && (xpathObj_local->nodesetval != NULL)
			&& (xpathObj_local->nodesetval->nodeNr >0)){
			strcpy(partitions[partition_id].size , xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[0]));
			debug_printf("getContent %s\n", partitions[partition_id].size);		
		}else{
			if (partition_id >= MAX_DEFAULT_PARTITIONS_SUPPORTED){
				printf("ERROR: No default SIZE has been specified for partition %d\n", partitions[partition_id].user_partition_id);
				return -1;
			}
			char tmp[10];
			sprintf(tmp,"%d",(((int[])DEFAULT_PARTITIONS_SIZE)[partition_id]));
			strcat(partitions[partition_id].size,tmp);
			debug_printf("partitions[partition_id].size %s\n",partitions[partition_id].size);
		}
		
		// looking for base vaddr element
		strcpy(expression,base_expression);
		debug_printf("expression: %s\n",expression);
 		strcat(expression, "/base_vaddr");
		debug_printf("expression: %s\n",expression);
		xpathExpr =  BAD_CAST expression;
		debug_printf("xpathE %s\n",xpathExpr);
		
	 	if (getNode(xpathExpr,&xpathObj_local) != 0){
			return -1;
		}

		if ((xpathObj_local != NULL) && (xpathObj_local->nodesetval != NULL)
			&& (xpathObj_local->nodesetval->nodeNr >0)){
			strcpy(partitions[partition_id].base_vaddr,xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[0]));
			debug_printf("getContent %s\n", partitions[partition_id].base_vaddr);
		}else{
			if (partition_id >= MAX_DEFAULT_PARTITIONS_SUPPORTED){
				printf("ERROR: No default BASE VADDR has been specified for partition %d\n", partitions[partition_id].user_partition_id);
				return -1;
			}
			char tmp[10];
			sprintf(tmp,"%x",(((int[])DEFAULT_PARTITIONS_BASE_VADDR)[partition_id]));
			strcpy(partitions[partition_id].base_vaddr,"0x");
			strcat(partitions[partition_id].base_vaddr,tmp);
			debug_printf("partitions[partition_id].base_vaddr %s\n",partitions[partition_id].base_vaddr);
		}

	return 0;
}

int get_loadaddr_elem(char *base_expression, int partition_id){


		xmlXPathObjectPtr xpathObj_local;
		char expression[100];
	
		strcpy(expression,base_expression);
		debug_printf("expression: %s\n",expression);
 		strcat(expression, "/load_addr");
		debug_printf("expression: %s\n",expression);
		xpathExpr =  BAD_CAST expression;
		debug_printf("xpathE %s\n",xpathExpr);
		
		
	 	if (getNode(xpathExpr,&xpathObj_local) != 0){
			return -1;
		}

		if ((xpathObj_local != NULL) && (xpathObj_local->nodesetval != NULL)
			&& (xpathObj_local->nodesetval->nodeNr >0)){
			strcpy(partitions[partition_id].loadaddr,xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[0]));
			debug_printf("getContent %s\n", partitions[partition_id].loadaddr);
		}else{
			if (partition_id >= MAX_DEFAULT_PARTITIONS_SUPPORTED){
				printf("ERROR: No default LOAD ADDR has been specified for partition %d\n", partitions[partition_id].user_partition_id);
				return -1;
			}
			char tmp[10];
			sprintf(tmp,"%x",(((int[])DEFAULT_PARTITIONS_LOADADDR)[partition_id]));
			strcpy(partitions[partition_id].loadaddr,"0x");
			strcat(partitions[partition_id].loadaddr,tmp);
			debug_printf("partitions[partition_id].loadaddr %s\n",partitions[partition_id].loadaddr);
		}
	return 0;
}


int get_asynch_events_elem(char *base_expression, int partition_id){

	strcpy(expression,base_expression);
	strcat(expression, "/nb_asynch_events");
	debug_printf("expression: %s\n",expression);
	xpathExpr =  BAD_CAST expression;
	debug_printf("xpathE %s\n",xpathExpr);
 	if (getNode(xpathExpr,&xpathObj_local) != 0){
		return -1;
	}

	if ((xpathObj_local != NULL) && (xpathObj_local->nodesetval != NULL)
		&& (xpathObj_local->nodesetval->nodeNr >0)){
		/*if (partitions[partition_id].scheduler_type != O1){
			printf("ERROR: you can not specify asynch events if the scheduler \n",);
		}*/
		partitions[partition_id].nb_asynch_events = atoi(xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[0]));
		debug_printf("nb_asynch_events %d\n", partitions[partition_id].nb_asynch_events);	
		nb_asynch_events += partitions[partition_id].nb_asynch_events;
	}else{
		if (partitions[partition_id].scheduler_type == O1){
			partitions[partition_id].nb_asynch_events = 0;
			debug_printf("nb_asynch_events %d\n", partitions[partition_id].nb_asynch_events);	
			nb_asynch_events += partitions[partition_id].nb_asynch_events;
		}
	}
}

int get_nb_of_partition_output_ports(int partition_id){
	
	partition_t partition = partitions[partition_id];
	return (partition.nb_of_ports - partition.nb_of_input_sampling_port -partition.nb_of_input_queuing_port);	

}

int get_slots_elem(char *base_expression, int partition_id){
		strcpy(expression,base_expression);
		strcat(expression, "/slot");
		debug_printf("expression: %s\n",expression);
		xpathExpr =  BAD_CAST expression;
		debug_printf("xpathE %s\n",xpathExpr);

	 	if (getNode(xpathExpr,&xpathObj_local) != 0){
			return -1;
		}
		
		int j;
		for (j =0; j<xpathObj_local->nodesetval->nodeNr; j++){
		
			xmlNodePtr slot_node = xpathObj_local->nodesetval->nodeTab[j];
			xmlChar *attr_duration = "duration";
			int slot_duration = atoi(xmlGetProp(slot_node,attr_duration));
			major_frame += slot_duration;
			partitions[partition_id].nb_slots += 1;
			xmlChar *attr_position = "position";
			int slot_position = atoi(xmlGetProp(slot_node,attr_position));
			if ((slot_position >= MAX_SLOTS) /*|| (slot_position >= nb_partitions) */ || (slot_position < 0)){
				printf ("ERROR: slot position %d not in range.\n", slot_position);
				return -1;
			}
			if (slots_duration[slot_position] > 0 ){
				printf ("ERROR: slot position %d already occupied.\n", slot_position);
				return -1;
			}
			slots_partition[slot_position] = partition_id;
			slots_duration[slot_position] = slot_duration;
			xmlChar *attr_postwrite_time = "postwrite_time";
			if (xmlHasProp(slot_node,attr_postwrite_time)){
				
				xmlChar *postwrite_time_string = xmlGetProp(slot_node,attr_postwrite_time);
				macro_enabled[POK_NEEDS_PORTS_SLOT] = TRUE;
				strcpy(macro_values[POK_NEEDS_PORTS_SLOT],"1");						
				int postwrite_time = atoi(postwrite_time_string);
				debug_printf("postwrite time: %d\n",postwrite_time);
				debug_printf("slots_duration[slot_position]: %d\n",slots_duration[slot_position]);
				if (postwrite_time >= slots_duration[slot_position]){
					printf ("ERROR: postwrite slot (%d) duration >= slots_duration.\n",slot_position);
					return -1;
				}
				postwrite_times[slot_position] = postwrite_time;
			}else if ((partitions[partition_id].scheduler_type == O1) && 
					(get_nb_of_partition_output_ports(partition_id)>0) ){
				printf("ERROR: no post write slot for partition %d\n", partitions[partition_id].user_partition_id);
				return -1;
			}
			nb_slots++;
		}
	return 0;
}

int get_events_elem(char *base_expression, int partition_id){

		strcpy(expression,base_expression);
		strcat(expression, "/events/event_name");
		debug_printf("expression: %s\n",expression);
 
		xpathExpr =  BAD_CAST expression;
		debug_printf("xpathE %s\n",xpathExpr);
		
	 	if (getNode(xpathExpr,&xpathObj_local) != 0){
			return -1;
		}

		if ((xpathObj_local != NULL) && (xpathObj_local->nodesetval != NULL)
			&& (xpathObj_local->nodesetval->nodeNr >0)){
		
			debug_printf("nodeNr	%d\n",xpathObj_local->nodesetval->nodeNr);
			partitions[partition_id].nb_events = xpathObj_local->nodesetval->nodeNr;
			debug_printf("nb_events %d\n", partitions[partition_id].nb_events);
			
			nb_events += partitions[partition_id].nb_events;
			partitions[partition_id].nb_events_blackb_buffers += partitions[partition_id].nb_events;
			nb_events_blackb_buffers += partitions[partition_id].nb_events;

			// read event name
			int i;
			for (i=0; i<partitions[partition_id].nb_events; i++){
				debug_printf("event name %s\n",xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[i]));
				strcpy(partitions[partition_id].events_names[i],xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[i]));
				debug_printf("event name %s\n",partitions[partition_id].events_names[i]);
			}
		}else{
			if (partitions[partition_id].scheduler_type == O1){
				partitions[partition_id].nb_events = 0;
			}
		}		
	return 0;
}

int get_buffers_elem(char *base_expression, int partition_id){
		strcpy(expression,base_expression);
		strcat(expression, "/buffers");
		xpathExpr =  BAD_CAST expression;
		debug_printf("xpathE %s\n",xpathExpr);
		
		if (getNode(xpathExpr,&xpathObj_local) != 0){
			printf("ERROR reading buffer\n");			
			return -1;
		}

		if ((xpathObj_local != NULL) && (xpathObj_local->nodesetval != NULL)
			&& (xpathObj_local->nodesetval->nodeNr >0)){
	
			
			if (partitions[partition_id].nb_buffers == 0){
				printf ("ERROR: no buffers in partition %d! \n", partitions[partition_id].user_partition_id);
				return -1;
			} 				

			
			char *max_buffer_msg_size_attr = "max_message_size"; 
			char *max_buffer_msg_size = xmlGetProp(xpathObj_local->nodesetval->nodeTab[0],max_buffer_msg_size_attr);
			debug_printf("buffer max_message_size %s\n",max_buffer_msg_size);
			strcpy(partitions[partition_id].buffers_max_message_size, max_buffer_msg_size);
			
			char *max_buffer_msgs_attr = "max_messages"; 
			char *max_buffer_msgs = xmlGetProp(xpathObj_local->nodesetval->nodeTab[0],max_buffer_msgs_attr);
			debug_printf("buffer max_message_size %s\n",max_buffer_msgs);
			strcpy(partitions[partition_id].max_buffers_msgs, max_buffer_msgs);
	
		}else{
			if (partitions[partition_id].scheduler_type == O1){
				strcpy(partitions[partition_id].max_buffers_msgs,DEFAULT_MAX_BUFFERS_MESSAGES);
				strcpy(partitions[partition_id].buffers_max_message_size, DEFAULT_BUFFER_MAX_MESSAGE_SIZE);
				debug_printf("No buffers element!\n");
			}
		}	

		strcpy(expression,base_expression);
		strcat(expression, "/buffers/max_message_size");
		debug_printf("expression: %s\n",expression);
		xpathExpr =  BAD_CAST expression;
		debug_printf("xpathE %s\n",xpathExpr);

	 	if (getNode(xpathExpr,&xpathObj_local) != 0){
			return -1;
		}

		if ((xpathObj_local != NULL) && (xpathObj_local->nodesetval != NULL)
			&& (xpathObj_local->nodesetval->nodeNr >0)){
			/*if (partitions[partition_id].scheduler_type != O1){
				printf("ERROR: you can not specify events if the scheduler \n",);
			}*/
			debug_printf("nodeNr	%d\n",xpathObj_local->nodesetval->nodeNr);
			debug_printf("buffers max_message_size %s\n",xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[0]));
			strcpy(partitions[partition_id].buffers_max_message_size, xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[0]));
		}else{
			strcpy(partitions[partition_id].buffers_max_message_size, DEFAULT_BUFFER_MAX_MESSAGE_SIZE);
			debug_printf("No buffers max_message_size!\n");
		}		
	return 0;
}

int get_blackboards_elem(char *base_expression, int partition_id){
		//printf("base_expression before strcpy: %s\n",base_expression);
		strcpy(expression,base_expression);
		//printf("base_expression: %s\n",base_expression);
		//printf("expression: %s\n",expression);
		strcat(expression, "/blackboards");
		debug_printf("expression: %s\n",expression);
 
		xpathExpr =  BAD_CAST expression;
		debug_printf("xpathE %s\n",xpathExpr);
		xpathObj_local;

	 	if (getNode(xpathExpr,&xpathObj_local) != 0){
			return -1;
		}

		if ((xpathObj_local != NULL) && (xpathObj_local->nodesetval != NULL)
			&& (xpathObj_local->nodesetval->nodeNr >0)){
			/*if (partitions[partition_id].scheduler_type != O1){
				printf("ERROR: you can not specify events if the scheduler \n",);
			}*/
			
			if (partitions[partition_id].nb_blackboards == 0){
				printf ("ERROR: no blackboards in partition %d! \n", partitions[partition_id].user_partition_id);
				return -1;
			} 				
			char *max_bb_msg_size_attr = "max_message_size"; 
			char *max_bb_msg_size = xmlGetProp(xpathObj_local->nodesetval->nodeTab[0],max_bb_msg_size_attr);
			debug_printf("blackboard max_message_size %s\n",max_bb_msg_size);
			strcpy(partitions[partition_id].blackboards_max_message_size, max_bb_msg_size);
		}else{
			strcpy(partitions[partition_id].blackboards_max_message_size, DEFAULT_BLACKBOARD_MAX_MESSAGE_SIZE);
			debug_printf("No blackboard max_message_size!\n");
		}		

	return 0;
}


int check_port_name(char *name){

	int i;
	for (i=0; i<nb_ports; i++){

		if (strcmp(ports[i].name, name) == 0){
			debug_printf("port_user_id: %d, port name: %s, name %s\n",ports[i].user_port_id, ports[i].name, name);
			printf("ERROR: port name %s already exist! \n", name);
			return -1;
		}	
	}
	return 0;
}

int read_name_size_message(char * line, int *port_nb, FILE *fr){

	int j = 0;
	int i = 0;
	boolean begin_name = FALSE;
	boolean end_name = FALSE;
	strcpy(input_sampling_ports_names_from_file[*port_nb],"");
	while (end_name == FALSE){
		if (line[j]=='"'){
			if (begin_name == FALSE) {
				begin_name = TRUE;
			}else 	if (end_name == FALSE) {
				end_name = TRUE;
			}
		}else if (begin_name){
			input_sampling_ports_names_from_file[*port_nb][i] = line[j];
			i++;
		}
		j++;
	}
	input_sampling_ports_names_from_file[*port_nb][i] = '\0';
	debug_printf("name:*%s*\n",input_sampling_ports_names_from_file[*port_nb]);

	i = 0;
	boolean begin_size = FALSE;
	boolean end_size = FALSE;

	strcpy(input_sampling_msgs_size_from_file[*port_nb],"");

	while (end_size == FALSE){
		switch (line[j]){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				input_sampling_msgs_size_from_file[*port_nb][i] = line[j];
				i++;
			break;
		}
		if (line[j]==','){
			if (begin_size == FALSE) {
				begin_size = TRUE;
			}else 	if (end_size == FALSE) {
				end_size = TRUE;
			}
		}
		j++;
	}
	
	input_sampling_msgs_size_from_file[*port_nb][i] = '\0';
	debug_printf("messages_size:*%s*\n",input_sampling_msgs_size_from_file[*port_nb]);

	i=0;
	
	if (strlen(line) < (j+3)){ // the message is not in this line
		printf("messages_size:*%s*\n",input_sampling_msgs_size_from_file[*port_nb]);
		if (fgets(line, sizeof line, fr) == NULL){
			printf("ERROR: Error reading ports file");
			return -1;
		}
	}

	boolean begin_msg = FALSE;
	boolean end_msg = FALSE;
	strcpy(input_sampling_msgs_from_file[*port_nb],"");
	while (end_msg == FALSE){
		if (line[j]=='{'){
			if (begin_msg == FALSE) {
				begin_msg = TRUE;
			}
		}else if (line[j]=='}'){
			if (end_msg == FALSE) {
				end_msg = TRUE;
			}
		}else if (begin_msg){
			input_sampling_msgs_from_file[*port_nb][i] = line[j];
			debug_printf("%c ",input_sampling_msgs_from_file[*port_nb][i]);
			i++;
		}
		j++;
	}

	input_sampling_msgs_from_file[*port_nb][i] = '\0';
	debug_printf("\nmessage :*%s*\n",input_sampling_msgs_from_file[*port_nb]);

	*port_nb = *port_nb +1;
}



init_events_in_partition(int partition_id){
	
	int k;
	for (k = 0; k<MAX_EVENTS_PER_PARTITION; k++){
		partitions[partition_id].events[k].set = FALSE;
		partitions[partition_id].events[k].wait = FALSE;
	}

}


int action_event_ok(int partition_id){
	
	int k;
	for (k = 0; k<partitions[partition_id].nb_events; k++){
		if ((partitions[partition_id].events[k].set == FALSE) || (partitions[partition_id].events[k].wait == FALSE)){
			printf("ERROR: event %s has not enaugh action\n",partitions[partition_id].events_names[k]);	
			return -1;	
		}
	}
	return 0;
}


int action_bb_ok(int partition_id){
	
	int k;
	for (k = 0; k<partitions[partition_id].nb_blackboards; k++){
		if ((partitions[partition_id].blackboards[k].display == FALSE) || (partitions[partition_id].blackboards[k].read == FALSE)){
			printf("ERROR: blackboard %s has not enaugh action\n",partitions[partition_id].blackboards_names[k]);	
			return -1;	
		}
	}
	return 0;
}

int action_buffer_ok(int partition_id){
	
	int k;
	for (k = 0; k<partitions[partition_id].nb_buffers; k++){
		if ((partitions[partition_id].buffers[k].send == FALSE) || (partitions[partition_id].buffers[k].receive == FALSE)){
			printf("ERROR: buffer %s has not enaugh action\n",partitions[partition_id].buffers_names[k]);	
			return -1;	
		}
	}
	return 0;
}


int read_thread_ports(int partition_id, char *threads_expression, int nb_of_real_threads){

	char threads_port_expression[100];
	strcpy(threads_port_expression,threads_expression);		
	strcat(threads_port_expression, "\"]/port");
	debug_printf(" threads port expression: %s\n",threads_port_expression);
				
	xpathExpr =  BAD_CAST threads_port_expression;
	debug_printf("xpathE %s\n",xpathExpr);
	xmlXPathObjectPtr xpathObj_thread;

 	if (getNode(xpathExpr,&xpathObj_thread) != 0){
		return -1;
	}

	if ((xpathObj_thread != NULL) && (xpathObj_thread->nodesetval != NULL)
		&& (xpathObj_thread->nodesetval->nodeNr >0)){

		debug_printf("nb_children	%d\n",xpathObj_thread->nodesetval->nodeNr);
		int k;
		for (k=0; k<xpathObj_thread->nodesetval->nodeNr; k++){

			// read port id
			if (get_port_id_in_thread_elem (partition_id, k, nb_of_real_threads, xpathObj_thread) != 0){
				return -1;
			}
		}
		
	}
	return 0;
}


int read_thread_events(int partition_id, char *threads_expression, int *nb_of_real_threads){

	debug_printf(" nb_of_real_threads: %d\n",*nb_of_real_threads);
	char threads_event_expression[100];
	strcpy(threads_event_expression,threads_expression);		
	strcat(threads_event_expression, "\"]/event");
	debug_printf(" threads event expression: %s\n",threads_event_expression);
	xmlXPathObjectPtr xpathObj_thread;
		
	xpathExpr =  BAD_CAST threads_event_expression;
	debug_printf("xpathE %s\n",xpathExpr);
	
 	if (getNode(xpathExpr,&xpathObj_thread) != 0){
		return -1;
	}

	if ((xpathObj_thread != NULL) && (xpathObj_thread->nodesetval != NULL)
		&& (xpathObj_thread->nodesetval->nodeNr >0)){
		debug_printf("thread current nr. %d\n",*nb_of_real_threads);
		if (get_event_in_thread_element(partition_id, *nb_of_real_threads, xpathObj_thread) != 0){
			return -1;
		}
		if (partitions[partition_id].scheduler_type == O1_SPLIT){
			if (partitions[partition_id].threads[*nb_of_real_threads + 1].type == EVENT_SUCCESSOR){
				*nb_of_real_threads += 1; // added the aperiodic thread	
			}	
		}else{
			*nb_of_real_threads += 1; // added the aperiodic thread				
		}
		debug_printf("-- thread current nr. %d\n",*nb_of_real_threads);
	} // if

	return 0;
}


int read_thread_blackboards(int partition_id, char *threads_expression, int *nb_of_real_threads){

	char threads_blackboard_expression[100];
	strcpy(threads_blackboard_expression,threads_expression);		
	strcat(threads_blackboard_expression, "\"]/blackboard");
	debug_printf(" threads blackboard expression: %s\n",threads_blackboard_expression);
	xmlXPathObjectPtr xpathObj_thread;
		
	xpathExpr =  BAD_CAST threads_blackboard_expression;
	debug_printf("xpathE %s\n",xpathExpr);
	
 	if (getNode(xpathExpr,&xpathObj_thread) != 0){
		return -1;
	}

	if ((xpathObj_thread != NULL) && (xpathObj_thread->nodesetval != NULL)
		&& (xpathObj_thread->nodesetval->nodeNr >0)){
		debug_printf("thread current nr. %d\n",*nb_of_real_threads);
		if (get_blackboard_in_thread_element(partition_id, *nb_of_real_threads, xpathObj_thread) != 0){
			return -1;				
		}
		*nb_of_real_threads += 1; // added the aperiodic thread				
		debug_printf("-- thread current nr. %d\n",*nb_of_real_threads);
	} // if

	return 0;
}

int read_thread_buffers(int partition_id, char *threads_expression, int *nb_of_real_threads){

	char threads_buffer_expression[100];
	strcpy(threads_buffer_expression,threads_expression);		
	strcat(threads_buffer_expression, "\"]/buffer");
	debug_printf(" threads buffer expression: %s\n",threads_buffer_expression);
	xmlXPathObjectPtr xpathObj_thread;
		
	xpathExpr =  BAD_CAST threads_buffer_expression;
	debug_printf("xpathE %s\n",xpathExpr);
	
 	if (getNode(xpathExpr,&xpathObj_thread) != 0){
		return -1;
	}

	if ((xpathObj_thread != NULL) && (xpathObj_thread->nodesetval != NULL)
		&& (xpathObj_thread->nodesetval->nodeNr >0)){
		debug_printf("thread current nr. %d\n",*nb_of_real_threads);
		if (get_buffer_in_thread_element(partition_id, *nb_of_real_threads, xpathObj_thread) != 0){
			return -1;				
		}
		*nb_of_real_threads += 1; // added the aperiodic thread				
		debug_printf("-- thread current nr. %d\n",*nb_of_real_threads);
	} // if

	return 0;
}


int get_threads_elem(char *base_expression, int partition_id){

	xmlXPathObjectPtr xpathObj_local;
	char expression[100];
	
	init_events_in_partition(partition_id);

	// read thread elements
	strcpy(expression,base_expression);
	debug_printf("expression: %s\n",expression);
	strcat(expression, "/threads/thread");
	debug_printf("expression: %s\n",expression);
	xmlChar *xpathExpr =  BAD_CAST expression;
	debug_printf("xpathE %s\n",xpathExpr);



 	if (getNode(xpathExpr,&xpathObj_local) != 0){
		return -1;
	}

	if ((xpathObj_local != NULL) && (xpathObj_local->nodesetval != NULL)
		&& (xpathObj_local->nodesetval->nodeNr >0)){

	//	printf("GET_CONTENT %s\n", xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[0]));

		debug_printf("nb_threads	%d\n",xpathObj_local->nodesetval->nodeNr);
		int nb_of_threads_declared = xpathObj_local->nodesetval->nodeNr;
		
		int i;
		int nb_of_real_threads = 0; // main thread ?
		for (i=0; i<nb_of_threads_declared; i++){
	
			partitions[partition_id].threads[nb_of_real_threads].type = NORMAL;
			partitions[partition_id].threads[nb_of_real_threads].thread_nb = nb_of_real_threads+1;

			// read priority
			xmlNodePtr thread_node = xpathObj_local->nodesetval->nodeTab[i];
			
			xmlChar *attr_priority = "priority";
			int thread_priority = atoi(xmlGetProp(thread_node,attr_priority));
			if (thread_priority > nb_priority_levels){
					nb_priority_levels = thread_priority;
			}
			debug_printf("thread_priority	%d\n",thread_priority);
			partitions[partition_id].threads[nb_of_real_threads].priority = thread_priority;
			if (partitions[partition_id].priority_levels[thread_priority] == 1){
				printf("ERROR: in partition %d, priority level %d already assigned\n",partition_id, thread_priority);
				return -1;
			}

			// build threads expression
			char threads_expression[100];
			strcpy(threads_expression,base_expression);
			strcat(threads_expression, "/threads");
			strcat(threads_expression, "/thread[@priority=\""); // PRIORITY ATTRIBUTE IS USED TO GET CHILDREN OF THE THREAD NODE
			strcat(threads_expression, xmlGetProp(thread_node,attr_priority));
	
			// read thread ports
			if (read_thread_ports(partition_id, threads_expression, nb_of_real_threads) != 0) {
				return -1;
			}

			// read thread events
			if (read_thread_events(partition_id, threads_expression, &nb_of_real_threads) != 0) {
				return -1;
			}

			// read thread blackboards
			if (read_thread_blackboards(partition_id, threads_expression, &nb_of_real_threads) != 0) {
				return -1;
			}
	
			// read thread buffers
			if (read_thread_buffers(partition_id, threads_expression, &nb_of_real_threads) != 0) {
				return -1;
			}
			
			nb_of_real_threads++; // added the thread 

		} // for

		// check if there are at least two action (SET and WAIT) for every event
		if (action_event_ok(partition_id) != 0){
			return -1;
		}

		// check if there are at least two action (DISPLAY and READ) for every blackboard
		if (action_bb_ok(partition_id) != 0){
			return -1;
		}

		// check if there are at least two action (SEND and RECEIVE) for every buffer
		if (action_buffer_ok(partition_id) != 0){
			return -1;
		}

		partitions[partition_id].nb_of_threads = nb_of_real_threads +1; // main thread
		debug_printf("nb_of_real_threads %d\n", partitions[partition_id].nb_of_threads );



	}else{	
		printf("ERROR: no threads element for partition %d\n!", partitions[partition_id].user_partition_id);
		return -1;
	}

	return 0;
}

int get_event_nb(const char *event_name, int partition_id){

	int k;
	for (k = 0; k<MAX_EVENTS_PER_PARTITION; k++){
		if (strcmp(partitions[partition_id].events_names[k], event_name) == 0){
			return k;		
		}
	}
	return -1;
}


int get_blackboard_nb(const char *blackboard_name, int partition_id){

	int k;
	for (k = 0; k<MAX_BLACKBOARDS_PER_PARTITION; k++){
		if (strcmp(partitions[partition_id].blackboards_names[k], blackboard_name) == 0){
			return k;		
		}
	}
	return -1;
}


int get_buffer_nb(const char *buffer_name, int partition_id){

	int k;
	for (k = 0; k<MAX_BUFFERS_PER_PARTITION; k++){
		if (strcmp(partitions[partition_id].buffers_names[k], buffer_name) == 0){
			return k;		
		}
	}
	return -1;
}

int split_thread_in_sched_O1(int partition_id, int i, event_act event_action, int event_nb){

	if (event_action == SET) {
//		if (partitions[partition_id].events[event_nb].set == 1){
//			printf("ERROR: SET action of event %s already assigned to a thread \n", event_name);
//			return -1;
//		}
		partitions[partition_id].events[event_nb].set = 1;
		// duplicate the thread for event management
		partitions[partition_id].threads[i].type = EVENT_SET;
		partitions[partition_id].threads[i].thread_nb = i+1;

		partitions[partition_id].threads[i+1].type = EVENT_RESET;
		partitions[partition_id].threads[i+1].thread_nb = i+2;

	}else{
/*		if (partitions[partition_id].events[event_nb].wait == 1){
			printf("ERROR: WAIT action of event %s already assigned to a thread \n", event_name);
			return -1;
		}
*/		partitions[partition_id].events[event_nb].wait = 1;
		// duplicate the thread for event management
		partitions[partition_id].threads[i].type = EVENT_RESUME;
		partitions[partition_id].threads[i].thread_nb = i+1;

		partitions[partition_id].threads[i+1].type = EVENT_WAIT;
		partitions[partition_id].threads[i+1].thread_nb = i+2;
	}	
	
	int priority_for_aperiodic = partitions[partition_id].threads[i].priority -1;
	partitions[partition_id].threads[i+1].priority = priority_for_aperiodic;
	if (partitions[partition_id].priority_levels[priority_for_aperiodic] == 1){
		printf("ERROR: in partition %d, priority level %d already assigned\n",partition_id, priority_for_aperiodic);
        return -1;
	}

	// add event to the threads
	debug_printf("Add event %d to the thread \n", event_nb);
	partitions[partition_id].threads[i].event_nb = event_nb;
	partitions[partition_id].threads[i+1].event_nb = event_nb;

	return 0;
	
}

int split_thread_in_sched_O1_SPLIT(int partition_id, int i, event_act event_action, int event_nb){

	if (event_action == SET) {
//		if (partitions[partition_id].events[event_nb].set == 1){
//			printf("ERROR: SET action of event %s already assigned to a thread \n", event_name);
//			return -1;
//		}
		partitions[partition_id].events[event_nb].set = 1;
		// duplicate the thread for event management
		partitions[partition_id].threads[i].type = EVENT_SETTER;
		partitions[partition_id].threads[i].thread_nb = i+1;
//		partitions[partition_id].threads[i+1].type = EVENT_RESET;	
		partitions[partition_id].threads[i].event_nb = event_nb;
		debug_printf("threads[%d].type %d\n", (i), partitions[partition_id].threads[i].type);
		//i++;
	}else{
/*		if (partitions[partition_id].events[event_nb].wait == 1){
			printf("ERROR: WAIT action of event %s already assigned to a thread \n", event_name);
			return -1;
		}
*/		partitions[partition_id].events[event_nb].wait = 1;

		// duplicate the thread for event management
		partitions[partition_id].threads[i].type = EVENT_PREDECESSOR;
		partitions[partition_id].threads[i].successor_id = i+1;
		partitions[partition_id].threads[i].thread_nb = i+1;
		partitions[partition_id].threads[i+1].type = EVENT_SUCCESSOR;
		partitions[partition_id].threads[i+1].thread_nb = i+2;
		debug_printf("threads[%d].type %d\n", (i), partitions[partition_id].threads[i].type);
		debug_printf("threads[%d].type %d\n", (i+1), partitions[partition_id].threads[i+1].type);
		// TODO: which priority I have to assigne to the aperiodic thread successor?
		int priority_for_aperiodic = partitions[partition_id].threads[i].priority -1;
		if (partitions[partition_id].priority_levels[priority_for_aperiodic] == 1){
			printf("ERROR: in partition %d, priority level %d already assigned\n",partition_id, priority_for_aperiodic);
	        return -1;
		}
		partitions[partition_id].threads[i+1].priority = priority_for_aperiodic++;
		partitions[partition_id].threads[i].event_nb = event_nb;
		partitions[partition_id].threads[i+1].event_nb = event_nb;
		//i = i+2;
		nb_of_sporadic_threads++;
	}	
	
		// add event to the threads
	debug_printf("Add event %d to the thread \n", event_nb);

	return 0;
	
}

int get_event_in_thread_element(int partition_id, int i, xmlXPathObjectPtr xpathObj_thread){

	xmlNodePtr event_node = xpathObj_thread->nodesetval->nodeTab[0];
	// debug_printf("thread_node name %s\n",thread_node->name);

	xmlChar *attr_event_name = "name";
	char *event_name = xmlGetProp(event_node,attr_event_name);
	debug_printf("event name %s\n",event_name);

	xmlChar *attr_event_action = "action";
	char  *event_action_string = xmlGetProp(event_node,attr_event_action);
	debug_printf("event action %s\n",event_action_string);

	event_act event_action;
	if (strcmp(event_action_string,"SET") == 0){
		event_action = SET;
	}else{
		event_action = WAIT;
	}
	
	// check if the event is already present in the partititions
	int event_nb = get_event_nb(event_name, partition_id);	
	if (event_nb == -1){
		event_nb = partitions[partition_id].nb_events;
		debug_printf("Add event to the partition \n");
		// add event to the partition
		strcpy(partitions[partition_id].events_names[event_nb],event_name);
		partitions[partition_id].nb_events++;
		nb_events++;
		partitions[partition_id].nb_events_blackb_buffers++;
		nb_events_blackb_buffers++;			
	}


	if (partitions[partition_id].scheduler_type == O1_SPLIT){
		return split_thread_in_sched_O1_SPLIT(partition_id, i, event_action, event_nb);
	}else{
		return split_thread_in_sched_O1(partition_id, i, event_action, event_nb);	
	}

	return 0;

}


int get_blackboard_in_thread_element(int partition_id, int i, xmlXPathObjectPtr xpathObj_thread){

	xmlNodePtr blackboard_node = xpathObj_thread->nodesetval->nodeTab[0];
	// debug_printf("thread_node name %s\n",thread_node->name);

	xmlChar *attr_blackboard_name = "name";
	char *blackboard_name = xmlGetProp(blackboard_node,attr_blackboard_name);
	debug_printf("blackboard name %s\n",blackboard_name);

	xmlChar *attr_blackboard_action = "action";
	char  *blackboard_action_string = xmlGetProp(blackboard_node,attr_blackboard_action);
	debug_printf("blackboard action %s\n",blackboard_action_string);

	blackboard_act blackboard_action;
	if (strcmp(blackboard_action_string,"DISPLAY") == 0){
		blackboard_action = DISPLAY;
	}else{
		blackboard_action = READ;
	}
	

	// check if the blackboard is already present in the partititions
	int blackboard_nb = get_blackboard_nb(blackboard_name, partition_id);	
	if (blackboard_nb == -1){
		blackboard_nb = partitions[partition_id].nb_blackboards;
		debug_printf("Add blackboard to the partition \n");
		// add blackboard to the partition
		strcpy(partitions[partition_id].blackboards_names[blackboard_nb],blackboard_name);
		partitions[partition_id].nb_blackboards++;
		nb_blackboards++;
		partitions[partition_id].nb_events_blackb_buffers++;
		nb_events_blackb_buffers++;			
	}

	if (blackboard_action == DISPLAY) {
/*		if (partitions[partition_id].blackboards[blackboard_nb].display == 1){
			printf("ERROR: DISPLAY action of blackboard %s already assigned to a thread \n", blackboard_name);
			return -1;
		}
*/		partitions[partition_id].blackboards[blackboard_nb].display = TRUE;
		// duplicate the thread for blackboard management
		partitions[partition_id].threads[i].type = BB_DISPLAY;
		partitions[partition_id].threads[i].thread_nb = i+1;
		partitions[partition_id].threads[i+1].type = BB_SUSPEND;
		partitions[partition_id].threads[i+1].thread_nb = i+2;
		
	}else{
/*		if (partitions[partition_id].blackboards[blackboard_nb].read == 1){
			printf("ERROR: READ action of blackboard %s already assigned to a thread \n", blackboard_name);
			return -1;
		}
*/		partitions[partition_id].blackboards[blackboard_nb].read = TRUE;
		// duplicate the thread for event management
		partitions[partition_id].threads[i].type = BB_RESUME;
		partitions[partition_id].threads[i].thread_nb = i+1;
		partitions[partition_id].threads[i+1].type = BB_READ;
		partitions[partition_id].threads[i+1].thread_nb = i+2;
	}	
	
	int priority_for_aperiodic = partitions[partition_id].threads[i].priority -1;
	partitions[partition_id].threads[i+1].priority = priority_for_aperiodic;
	if (partitions[partition_id].priority_levels[priority_for_aperiodic] == 1){
		printf("ERROR: in partition %d, priority level %d already assigned\n",partition_id, priority_for_aperiodic);
        return -1;
	}

	// add blackboard to the threads
	debug_printf("Add blackboard %d to the thread \n", blackboard_nb);
	partitions[partition_id].threads[i].blackboard_nb = blackboard_nb;
	partitions[partition_id].threads[i+1].blackboard_nb = blackboard_nb;

	return 0;

}

int get_buffer_in_thread_element(int partition_id, int i, xmlXPathObjectPtr xpathObj_thread){

	xmlNodePtr buffer_node = xpathObj_thread->nodesetval->nodeTab[0];
	// debug_printf("thread_node name %s\n",thread_node->name);

	xmlChar *attr_buffer_name = "name";
	char *buffer_name = xmlGetProp(buffer_node,attr_buffer_name);
	debug_printf("buffer name %s\n",buffer_name);

	xmlChar *attr_buffer_action = "action";
	char  *buffer_action_string = xmlGetProp(buffer_node,attr_buffer_action);
	debug_printf("buffer action %s\n",buffer_action_string);

	buffer_act buffer_action;
	if (strcmp(buffer_action_string,"SEND") == 0){
		buffer_action = SEND;
	}else{
		buffer_action = RECEIVE;
	}
	

	// check if the buffer is already present in the partititions
	int buffer_nb = get_buffer_nb(buffer_name, partition_id);	
	if (buffer_nb == -1){
		buffer_nb = partitions[partition_id].nb_buffers;
		debug_printf("Add buffer to the partition \n");
		// add buffer to the partition
		strcpy(partitions[partition_id].buffers_names[buffer_nb],buffer_name);
		partitions[partition_id].nb_buffers++;
		nb_buffers++;
		partitions[partition_id].nb_events_blackb_buffers++;
		nb_events_blackb_buffers++;			
	}

	if (buffer_action == SEND) {
/*		if (partitions[partition_id].buffers[buffer_nb].send == TRUE){
			printf("ERROR: SEND action of buffer %s already assigned to a thread \n", buffer_name);
			return -1;
		}
*/		partitions[partition_id].buffers[buffer_nb].send = TRUE;
		// duplicate the thread for buffer management
		partitions[partition_id].threads[i].type = BUFFER_RESUME_SENDER;
		partitions[partition_id].threads[i+1].thread_nb = i+1;
		partitions[partition_id].threads[i+1].type = BUFFER_SEND;
		partitions[partition_id].threads[i+1].thread_nb = i+2;
		
	}else{
/*		if (partitions[partition_id].buffers[buffer_nb].receive == TRUE){
			printf("ERROR: RECEIVE action of buffer %s already assigned to a thread \n", buffer_name);
			return -1;
		}
*/		partitions[partition_id].buffers[buffer_nb].receive = TRUE;
		// duplicate the thread for event management
		partitions[partition_id].threads[i].type = BUFFER_RESUME_RECEIVER;
		partitions[partition_id].threads[i].thread_nb = i+1;
		partitions[partition_id].threads[i+1].type = BUFFER_RECEIVE;
		partitions[partition_id].threads[i+1].thread_nb = i+2;
	}	
	
	int priority_for_aperiodic = partitions[partition_id].threads[i].priority -1;
	partitions[partition_id].threads[i+1].priority = priority_for_aperiodic;
	if (partitions[partition_id].priority_levels[priority_for_aperiodic] == 1){
		printf("ERROR: in partition %d, priority level %d already assigned\n",partition_id, priority_for_aperiodic);
        return -1;
	}

	// add buffer to the threads
	debug_printf("Add buffer %d to the thread \n", buffer_nb);
	partitions[partition_id].threads[i].buffer_nb = buffer_nb;
	partitions[partition_id].threads[i+1].buffer_nb = buffer_nb;

	return 0;

}


int get_port_id_in_thread_elem (int partition_id, int k, int i, xmlXPathObjectPtr xpathObj_thread){

	xmlNodePtr port_node = xpathObj_thread->nodesetval->nodeTab[k];
	// debug_printf("thread_node name %s\n",thread_node->name);

	xmlChar *attr_portid = "id";
	int thread_user_port_id = atoi(xmlGetProp(port_node,attr_portid));
	debug_printf("port id: %d\n",thread_user_port_id);
	int thread_port_id =  getPortIdByUserPortId(thread_user_port_id);

	if ( (ports[thread_port_id].partition_id != -1) /*&& (ports[thread_port_id].partition_id != partition_id) */){
		//printf ("ERROR: port already assigned to another partition (%d)\n",ports[thread_port_id].partition_id);
		printf ("ERROR: port already assigned to another thread \n");							
		return -1;
	}

	if (ports[thread_port_id].partition_id == -1){

		ports[thread_port_id].partition_id = partition_id;

		// update partition values						
		if (ports[thread_port_id].kind == SAMPLING){
			partitions[partition_id].nb_of_sampling_port +=1;
		}else if (ports[thread_port_id].kind == QUEUING){
			partitions[partition_id].nb_of_queuing_port +=1;						
		}

		if (ports[thread_port_id].direction == DESTINATION){
			if (ports[thread_port_id].kind == SAMPLING){
				partitions[partition_id].nb_of_input_sampling_port +=1;
			}else if (ports[thread_port_id].kind == QUEUING){
				partitions[partition_id].nb_of_input_queuing_port +=1;						
			}
		}
		// add the port to the 	partition
		partitions[partition_id].nb_of_ports++;
		struct port_list_item_t * partition_port = ((struct port_list_item_t *) malloc(sizeof(struct port_list_item_t)));
	  	partition_port->user_port_id = thread_user_port_id;
	    partition_port->port_id = thread_port_id;
	    if (partitions[partition_id].ports == NULL){
	            partition_port->next = NULL;
	    }else{
	            partition_port->next = partitions[partition_id].ports;
	    }
	    partitions[partition_id].ports = partition_port;
	}

	// add the port to the thread
	struct port_list_item_t * thread_port = ((struct port_list_item_t *) malloc(sizeof(struct port_list_item_t)));
  	thread_port->user_port_id = thread_user_port_id;
    thread_port->port_id = thread_port_id;
    if (partitions[partition_id].threads[i].ports == NULL){
            thread_port->next = NULL;
    }else{
            thread_port->next = partitions[partition_id].threads[i].ports;
    }
    partitions[partition_id].threads[i].ports = thread_port;

	if ((ports[thread_port_id].kind == SAMPLING) && (ports[thread_port_id].direction == DESTINATION)){
		partitions[partition_id].threads[i].hasInputSamplingPort = TRUE;
	}
	
	return 0;
}

/* set the initial configuration */
void init_config(){

	nb_asynch_events = 0;
	nb_events = 0;	
	nb_partitions = 0;
	nb_ports = 0;
	nb_of_ports_sampling = 0;
	nb_of_ports_queuing = 0;
	nb_of_sporadic_threads = 0;
	priority_for_aperiodic = 2; // 0 is for idle thread, 1 for kernel thread ?
	
 	int i;
	for (i = 0; i < 100; i++){
		sprintf(number_to_string[i],"%d",i);
	}
	
	// initialization of kernel macros that have a DEFAULT value
	strcpy(macro_values[POK_BUS_FREQ_MHZ],DEFAULT_POK_BUS_FREQ_MHZ);
	macro_enabled[POK_BUS_FREQ_MHZ] = TRUE;
	
	strcpy(macro_values[POK_FREQ_DIV],DEFAULT_POK_FREQ_DIV);
	macro_enabled[POK_FREQ_DIV] = TRUE;
	
	strcpy(macro_values[POK_FREQ_SHIFT],DEFAULT_POK_FREQ_SHIFT);
	macro_enabled[POK_FREQ_SHIFT] = TRUE;
	
	strcpy(macro_values[IDLE_STACK_SIZE],DEFAULT_IDLE_STACK_SIZE);
	macro_enabled[IDLE_STACK_SIZE] = TRUE;
	
	strcpy(macro_values[POK_USER_STACK_SIZE],DEFAULT_POK_USER_STACK_SIZE);
	macro_enabled[POK_USER_STACK_SIZE] = TRUE;
	
	strcpy(macro_values[POK_CONFIG_PRIORITY_LEVELS],DEFAULT_PRIORITY_LEVELS);
	macro_enabled[POK_CONFIG_PRIORITY_LEVELS] = TRUE;

}

int getNode(const xmlChar * xpathExpr, xmlXPathObjectPtr *xpathObj_param){

 	assert(xpathExpr);
   *xpathObj_param = xmlXPathEvalExpression(xpathExpr, xpathCtx);

    if(*xpathObj_param == NULL) {
        printf("ERROR: unable to evaluate xpath expression \"%s\"\n", xpathExpr);
        xmlXPathFreeContext(xpathCtx); 
        xmlFreeDoc(doc); 
        return -1;
    }
	return 0;
}
