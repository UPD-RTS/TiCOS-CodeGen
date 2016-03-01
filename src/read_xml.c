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

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/xmlschemas.h>

#include "arinc653-xml-conf.h"
#include "read_utils.h"

/*

The functions implemented in this file are used for:

- Reading the input file .xml; 
- Validating it wrt the Application.xsd
- Saving the configuration information (eg. number of threads, partitions, etc...)

*/

// used to read the xml file
xmlDocPtr doc;
xmlXPathContextPtr xpathCtx; 
xmlXPathObjectPtr xpathObj; 

// total number of ...
extern int nb_partitions;
extern int nb_ports;
extern int nb_slots;
extern int nb_priority_levels;
extern int nb_asynch_events; 
extern int nb_events;
extern int nb_blackboards;
extern int nb_buffers;
extern int nb_events_blackb_buffers;
extern input_sampling_names_from_file;



int nb_of_input_sampling_port;
int major_frame;

char *scheduler_types[NB_SCHEDULER_TYPES]={"FPPS","O1","O1_SPLIT"};

int	debug_printf(const char *format, ...);

// read the xml file
int read_xml(){
	
	init_config();

	debug_printf ("Reading hardware element...\n");	

	// read the hardaware element
	if (read_hardware() != 0){
		return -1;	
	}
	debug_printf ("OK\n");	

	// read the kernel element	
	if (read_kernel() != 0){
		return -1;	
	}

	// read the ports element
	if (read_ports() != 0){
		return -1;	
	}

	// read the partitions element
	if (read_partitions() != 0){
		return -1;	
	}

	// check if every port has been assigned to a partition
	int i;
	for (i=0; i<nb_ports; i++){
		if (ports[i].partition_id == -1){
			printf("WARNING: port %d has not been assigned to any partition!\n",ports[i].user_port_id);
		}	
	}

	return 0;

} 


// check the xml validity
int is_valid(char *filename,char *schema_filename)
{
	
	// Init libxml      
	xmlInitParser();
	LIBXML_TEST_VERSION
	
	assert(filename);

	// Load XML document 
    doc = xmlParseFile(filename);
    if (doc == NULL) {
		fprintf(stderr, "ERROR: unable to parse file \"%s\"\n", filename);
		return(-1);
    }

	// Create xpath evaluation context
    xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL) {
        printf("ERROR: unable to create new XPath context\n");
        xmlFreeDoc(doc); 
        return(-1);
    }

	// Load the schema
	xmlDocPtr schema_doc = xmlReadFile(schema_filename, NULL, XML_PARSE_NONET);
	if (schema_doc == NULL) {
		// the schema cannot be loaded or is not well-formed
		printf("ERROR: the schema cannot be loaded or is not well-formed\n");
		return -1;
	}

	// Create the XML parser
    xmlSchemaParserCtxtPtr parser_ctxt = xmlSchemaNewDocParserCtxt(schema_doc);
    if (parser_ctxt == NULL) {
		// unable to create a parser context for the schema 
		printf("ERROR: unable to create a parser context for the schema\n");  
		xmlFreeDoc(schema_doc);
		return -1;
	}
	
	// Validate the schema
	xmlSchemaPtr schema = xmlSchemaParse(parser_ctxt);
	if (schema == NULL) {
		// the schema itself is not valid 
		printf("ERROR: the schema itself is not valid\n");
		xmlSchemaFreeParserCtxt(parser_ctxt);
		xmlFreeDoc(schema_doc);
		return -1;
	}

	// Validate the schema file 
	xmlSchemaValidCtxtPtr valid_ctxt = xmlSchemaNewValidCtxt(schema);
	if (valid_ctxt == NULL) {
        // Unable to create a validation context for the schema
		printf("ERROR: unable to create a validation context for the schema\n");
        xmlSchemaFree(schema);
        xmlSchemaFreeParserCtxt(parser_ctxt);
        xmlFreeDoc(schema_doc);
        return -1; 
    }

	// Validate the file
    if (xmlSchemaValidateDoc(valid_ctxt, doc) != 0){ // Returns 0 if the document is schemas valid,
		printf("ERROR: Schema file not valid\n");   // a positive error code number otherwise and -1 in case of internal or API error.
    	return -1;
	}

	xmlSchemaFreeValidCtxt(valid_ctxt);
	xmlSchemaFree(schema);
	xmlSchemaFreeParserCtxt(parser_ctxt);
	xmlFreeDoc(schema_doc);
	return 0;
}


// read kernel parameters
int read_kernel(){

	// read idle stack element
	xmlChar * xpathExpr = BAD_CAST "//idle_stack";

	if (getNode(xpathExpr,&xpathObj) != 0){
		return -1;
	}		

	if (xpathObj->nodesetval->nodeNr == 0){
		debug_printf("idle stack not present\n");
	}else{
		debug_printf("idle stack value %s\n",xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));	
		strcpy(macro_values[IDLE_STACK_SIZE],xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));
		int idle_stack_value = atoi	(xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));
		if ((idle_stack_value % 4) != 0)
		{
			printf ("ERROR: idle stack value should be a multiple of 1024\n");
			return -1;
		}
	}
	
	// read user stack element
	xpathExpr = BAD_CAST "//user_stack";
	
	if (getNode(xpathExpr,&xpathObj) != 0){
		return -1;
	}

	if (xpathObj->nodesetval->nodeNr == 0){
		debug_printf("User_stack not present\n");
	}else{
		debug_printf("User_stack value %s\n",xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));
		int user_stack_value = atoi	(xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));
		if ((user_stack_value % 4) != 0)
		{
			printf ("ERROR: user stack value should be a multiple of 1024\n");
			return -1;
		}
		strcpy(macro_values[POK_USER_STACK_SIZE],xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));
	}
	
	// read user priority_levels" element
	xpathExpr = BAD_CAST "//priority_levels";
	
	if (getNode(xpathExpr,&xpathObj) != 0){
		return -1;
	}

	if (xpathObj->nodesetval->nodeNr == 0){
		nb_priority_levels = 2;
		debug_printf("Priority_levels not present\n");
	}else{
		nb_priority_levels = atoi(xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));
		strcpy(macro_values[POK_CONFIG_PRIORITY_LEVELS],xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));
		debug_printf("Priority_levels value %s\n",macro_values[POK_CONFIG_PRIORITY_LEVELS]);	
	}
	return 0;	
}


// read hardware parameters
int read_hardware(){

	// hardware element
	xmlChar * xpathExpr = BAD_CAST "//hardware";
	xmlXPathObjectPtr xpathObj;

	if (getNode(xpathExpr,&xpathObj) != 0){
		return -1;
	}
	
	if (xpathObj->nodesetval->nodeNr == 0){
		debug_printf("Hardware element not present\n");
		return 0;
	}

	// bus frequency
	xpathExpr = BAD_CAST "//hardware/bus_frequency";

	if (getNode(xpathExpr,&xpathObj) != 0){
		return -1;
	}
	
	if (xpathObj->nodesetval->nodeNr == 0){
		debug_printf("bus_frequency element not present\n");
	}else{
		debug_printf("bus frequency %s\n",xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));	
		strcpy(macro_values[POK_BUS_FREQ_MHZ],xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));
	}
	
	// frequency_div
	xpathExpr = BAD_CAST "//hardware/frequency_div";
	
	if (getNode(xpathExpr,&xpathObj) != 0){
		return -1;
	}
	
	if (xpathObj->nodesetval->nodeNr == 0){
		debug_printf("frequency_div element not present\n");
	}else{
		debug_printf("frequency_div %s\n",xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));	
		strcpy(macro_values[POK_FREQ_DIV],xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));	
	}
	
	// frequency_shift
	xpathExpr = BAD_CAST "//hardware/frequency_shift";

	if (getNode(xpathExpr,&xpathObj) != 0){
		return -1;
	}
	
	if (xpathObj->nodesetval->nodeNr == 0){
		debug_printf("frequency_shift element not present\n");
	}else{
		debug_printf("frequency_shift %s\n",xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));	
		strcpy(macro_values[POK_FREQ_SHIFT],xmlNodeGetContent(xpathObj->nodesetval->nodeTab[0]));
	}
	
	return 0;
}


// read partitions parameters
int read_partitions(){

	int partitions_id[MAX_PARTITIONS];

	int z;
	for (z=0; z<MAX_PARTITIONS; z++){
		partitions_id[z]=0;
	}

	debug_printf("\n*********** READ PARTITIONS **********\n");
	
	xmlChar * xpathExpr = BAD_CAST "//partition";

	if (getNode(xpathExpr,&xpathObj) != 0){
		return -1;
	}
	
	nb_partitions = xpathObj->nodesetval->nodeNr;
	strcpy(macro_values[POK_CONFIG_NB_PARTITIONS],number_to_string[nb_partitions]);
	macro_enabled[POK_CONFIG_NB_PARTITIONS] = TRUE;

	debug_printf("Partition number %s\n", macro_values[POK_CONFIG_NB_PARTITIONS]);

	xmlChar *attr_name = "id";
	xmlChar *id;
	char expression[100];
	char base_expression[40];
	//char *expression;
	//char *base_expression;

	int partition_id;
	int user_partition_id;
	int partition_core_id;

	int i;
	for (i=0; i<nb_partitions; i++){

		
		// read user partition id
		xmlNodePtr partition = 	xpathObj->nodesetval->nodeTab[i];			
		id = xmlGetProp(partition,attr_name);
		user_partition_id = atoi(id);
		debug_printf("integer id %d\n",user_partition_id);
		
		if (partitions_id[user_partition_id]){
			printf("ERROR: user_partition_id %d already defined \n",user_partition_id);
			return -1;
		}else{
			partitions_id[user_partition_id]=1;
		}

		
		// set the partition name
		partition_id = i;
		partitions[partition_id].user_partition_id = user_partition_id;
		strcpy(base_expression,"");
		strcat(base_expression, "//partition[@id=\"");
		strcat(base_expression, id);
		strcat(base_expression, "\"]");
		debug_printf("expression: %s\n",base_expression);
		strcpy(partitions[partition_id].part_name,"part");
		strcat(partitions[partition_id].part_name,number_to_string[partition_id+1]);
		debug_printf("part name %s\n", partitions[partition_id].part_name);
	
		// looking for scheduler element
		if (get_scheduler_elem(base_expression, partition_id, xpathObj) != 0){
			return -1;
		}
		if (get_threads_elem(base_expression,partition_id) != 0){
			return -1;
		}

		strcpy(macro_values[POK_CONFIG_PRIORITY_LEVELS], number_to_string[nb_priority_levels]);
		debug_printf("Priority_levels value %s\n", macro_values[POK_CONFIG_PRIORITY_LEVELS]);	

		// looking for size element
		if (get_size_elem(base_expression, partition_id) != 0){
			return -1;
		}

		// looking for loadaddr element
		if (get_loadaddr_elem(base_expression, partition_id) != 0){
			return -1;
		}

		// looking for slot element
		if (get_slots_elem(base_expression, partition_id) != 0){
			return -1;
		}
	
		// looking for nb_asynch_events element
		if (get_asynch_events_elem (base_expression, partition_id) != 0){
			return -1;
		}

		// looking for blackboards element
		if (get_blackboards_elem(base_expression, partition_id) != 0){
			return -1;
		}
		
		// looking for buffers/max_messages element
		if (get_buffers_elem(base_expression, partition_id) != 0){
			return -1;
		}
		
		// check if the nuber of threads are enaugh or not
		if ( (partitions[partition_id].nb_events_blackb_buffers *2) > partitions[partition_id].nb_of_threads ){
			printf("ERROR: Not enought threads\n");
			return -1;
		}
	}// for

//	if (nb_slots > nb_partitions){
	int k;
	for (k=0; k<nb_partitions; k++){
		partitions[k].period = major_frame / partitions[k].nb_slots;
		debug_printf("partition period %d\n",partitions[k].period);
	}
//	}

	strcpy(macro_values[POK_CONFIG_SCHEDULING_NBSLOTS],number_to_string[nb_slots]);
	macro_enabled[POK_CONFIG_SCHEDULING_NBSLOTS] = TRUE;

	return 0;
}


int getPartitionIdByUserPartitionId(int user_partition_id){

	int i;
	for (i =0; i<nb_partitions; i++){
		if (partitions[i].user_partition_id == user_partition_id){
			debug_printf("partitions[i].user_partition_id %d\n",partitions[i].user_partition_id);
			return i;
		}
	}
	return -1;
}


int getPortIdByUserPortId(int user_port_id){

	int i;
	for (i =0; i<nb_ports; i++){
		if (ports[i].user_port_id == user_port_id){
			debug_printf("ports[i].user_port_id: %d\n",ports[i].user_port_id);
			return i;
		}
	}
	return -1;
}


// read ports parameters
int read_ports(){

	debug_printf("\n*********** READ PORTS **********\n");
		

	xmlChar * xpathExpr = BAD_CAST "//ports/port";
	if (getNode(xpathExpr,&xpathObj) != 0){
		return -1;
	}
	
	nb_ports = xpathObj->nodesetval->nodeNr;
	debug_printf("number of ports %d\n",nb_ports);
		
	xmlChar *attr_name;
	char expression[100];
	char base_expression[50];
	int user_port_id;
	int port_id;
	int i;
	
	for (i=0; i<nb_ports; i++){
		
		xmlXPathObjectPtr xpathObj_local;

		attr_name = "id";
		xmlNodePtr port = 	xpathObj->nodesetval->nodeTab[i];			
		user_port_id = atoi(xmlGetProp(port,attr_name));
		debug_printf("user port numeric id %d\n",user_port_id);
		port_id = i;
		ports[port_id].partition_id = -1; // used to check is a port has been already assigned to the partition
		//ports[port_id].thread_number = -1; // used to check is a port has been already assigned to the thread
		ports[port_id].user_port_id = user_port_id;
		ports[port_id].local_numeric_id = port_id;
		ports[port_id].global_numeric_id = port_id;
		
		strcpy(base_expression,"");
		strcat(base_expression, "//port[@id=\"");
		strcat(base_expression, number_to_string[user_port_id]);
		strcat(base_expression, "\"]");
		debug_printf("expression: %s\n",base_expression);
	
		// looking for kind element
		strcpy(expression,base_expression);
		strcat(expression, "/kind");
		debug_printf("expression: %s\n",expression);

		xpathExpr =  BAD_CAST expression;
		debug_printf("xpathE %s\n",xpathExpr);
	
	 	if (getNode(xpathExpr,&xpathObj_local) != 0){
			return -1;
		}
		char *kind = xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[0]);

		debug_printf("kind %s\n", kind);
		if (strcmp(kind,"sampling") == 0){
			debug_printf("SAMPLING\n");
			ports[port_id].kind = SAMPLING;
			nb_of_ports_sampling+=1;
			strcat(ports[port_id].local_id,"S");
		}else if (strcmp(kind,"queueing") == 0){
			debug_printf("QUEUING\n");
			ports[port_id].kind = QUEUING;
			nb_of_ports_queuing+=1;
			strcat(ports[port_id].local_id,"Q");
		}else{
			debug_printf("ERROR: invalid port kind\n");
			return -1;
		}

		
		// looking for direction element
		strcpy(expression,base_expression);
		strcat(expression, "/direction");
		debug_printf("expression: %s\n",expression);

		xpathExpr = BAD_CAST expression;
		debug_printf("xpathE %s\n",xpathExpr);
	
	 	if (getNode(xpathExpr,&xpathObj_local) != 0){
			return -1;
		}
		
		char *direction = xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[0]);
	
		debug_printf("direction %s\n", direction);
		if (strcmp(direction,"destination") == 0){
			debug_printf("DESTINATION\n");
			ports[port_id].direction = DESTINATION;
			strcat(ports[port_id].local_id,"D_");
			if (ports[port_id].kind == SAMPLING){
				nb_of_input_sampling_port +=1;		
			}
		}else if (strcmp(direction,"source") == 0){
			ports[port_id].direction = SOURCE;
			strcat(ports[port_id].local_id,"S_");
			debug_printf("SOURCE\n");
		}else{
			printf("ERROR: invalid port direction\n");
			return -1;
		}


		// looking for port name element
		strcpy(expression,base_expression);
		strcat(expression, "/name");
		debug_printf("expression: %s\n",expression);

		xpathExpr =  BAD_CAST expression;
		debug_printf("xpathE %s\n",xpathExpr);
	
	 	if (getNode(xpathExpr, &xpathObj_local) != 0){
			return -1;
		}

		if ((xpathObj_local != NULL) && (xpathObj_local->nodesetval != NULL) && 
			(xpathObj_local->nodesetval->nodeNr >0)){

			
			char *port_name = xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[0]);
			// check if a port name already exist
			if (check_port_name(port_name) != 0){
				return -1;
			}
			debug_printf("getContent %s\n", port_name);
			strcpy(ports[port_id].name, port_name);
		
		}else{
			printf("ERROR: no port name for port %d\n",ports[port_id].user_port_id);
			return -1;
		}
				

		// set port local_id
		strcat(ports[port_id].local_id,number_to_string[port_id]);
		debug_printf("local_id %s\n",ports[port_id].local_id);

		// set port receiving_var_name
		if (ports[port_id].direction == DESTINATION){
			strcpy(ports[port_id].receiving_var_name,ports[port_id].local_id);
			debug_printf("receiving_var_name %s\n",ports[port_id].receiving_var_name);
		}

		// set port global id
		strcat(ports[port_id].global_id,"");
		strcat(ports[port_id].global_id,ports[port_id].local_id);
		strcat(ports[port_id].global_id,"_global");
		debug_printf("global_id %s\n",ports[port_id].global_id);

		
		// looking for default message element -- not defined in the XML file
	
	}// for

		
	// looking for destination ports -- every source port MUST have a destination port		
	for (i=0; i<nb_ports; i++){

		attr_name = "id";
		xmlNodePtr port = xpathObj->nodesetval->nodeTab[i];			
		//user_port_id_string = xmlGetProp(port,attr_name);
		user_port_id = atoi(xmlGetProp(port,attr_name));
		port_id = i;

		strcpy(base_expression,"");
		strcat(base_expression, "//port[@id=\"");
		strcat(base_expression, number_to_string[user_port_id]);
		strcat(base_expression, "\"]");
		debug_printf("expression: %s\n",base_expression);
	
		strcpy(expression,base_expression);
		strcat(expression, "/destination_port");
		debug_printf("expression: %s\n",expression);

		xpathExpr = BAD_CAST expression;
		debug_printf("xpathE %s\n",xpathExpr);
		xmlXPathObjectPtr xpathObj_local;

		if (getNode(xpathExpr,&xpathObj_local) != 0){
			return -1;
		}

		if ((xpathObj_local != NULL) && (xpathObj_local->nodesetval != NULL)){
			int nb_dest_ports = xpathObj_local->nodesetval->nodeNr;
			debug_printf("destination ports %d\n", nb_dest_ports);
			debug_printf("ports[port_id].direction %d (SOURCE=%d, DESTINATION=%d)\n ", ports[port_id].direction, SOURCE, DESTINATION);
			
			// check if the destination port has been associated with a source port
			if ((nb_dest_ports > 0) && (ports[port_id].direction == DESTINATION)){
				printf("ERROR: DESTINATION port %d has DESTINATION ports\n", user_port_id);
				return -1;				
			}
				
			// check if the sampling port has been associated with more destination ports
			if ((nb_dest_ports>1) && (ports[i].kind == SAMPLING)){
				printf("ERROR: SAMPLING port %d can have only one DESTINATION port\n", user_port_id);
				return -1;								
			}
			
			ports[port_id].nb_of_destinations = nb_dest_ports;

			int j;
			for (j=0; j<ports[port_id].nb_of_destinations; j++){
				//char *destination_user_port_id_string = xmlNodeGetContent(xpathObj_local->nodesetval->nodeTab[j]);
				xmlChar *attr_destination_id = "id";
				char *destination_user_port_id_string = xmlGetProp(xpathObj_local->nodesetval->nodeTab[j],attr_destination_id);
				debug_printf("destination port id %s\n",destination_user_port_id_string);
				int numeric_destination_user_port_id = atoi(destination_user_port_id_string);
				
				int destination_port_id = getPortIdByUserPortId(numeric_destination_user_port_id);
				if (destination_port_id == -1){
					printf("ERROR: User port id %d not found\n",numeric_destination_user_port_id);
					return -1;
				}
				
				if (ports[destination_port_id].direction == SOURCE){
					printf("ERROR: port %d should be a DESTINATION port but it is not\n",numeric_destination_user_port_id);
					return -1;
				}
				
				if (ports[destination_port_id].kind != ports[port_id].kind){
					printf("ERROR: source port %d and its destination port %d have different kind \n",port_id,destination_port_id,user_port_id);
					return -1;
				}
				
				struct port_list_item_t * destination_port = ((struct port_list_item_t *) malloc(sizeof(struct port_list_item_t)));
				destination_port->user_port_id = numeric_destination_user_port_id;
				destination_port->port_id = destination_port_id;
				
				if (ports[port_id].destinations == NULL){
					destination_port->next = NULL;
				}else{
					destination_port->next = ports[port_id].destinations;
				}
				ports[port_id].destinations = destination_port;
				
			}
		}else{
			debug_printf("destination ports %d\n", 0);	
			if (ports[port_id].direction == SOURCE){
				printf("ERROR: SOURCE port %d does not have destination ports\n", user_port_id);
				return -1;
			}	
		}
	}
	
	if (nb_ports >0){
		// looking for max_message_size element
		xmlXPathObjectPtr xpathObj_max;
		xpathExpr = BAD_CAST "//ports/max_message_size";

		debug_printf("max_message_size\n");

		if (getNode(xpathExpr,&xpathObj_max) != 0){
			return -1;
		}
		
		if ((xpathObj_max != NULL) 
			&& (xpathObj_max->nodesetval != NULL)
			&& (xpathObj_max->nodesetval->nodeNr>0)){
				strcpy(max_message_size_string,xmlNodeGetContent(xpathObj_max->nodesetval->nodeTab[0]));
				
		}else{
			debug_printf("default max_message_size %d\n", DEFAULT_MAX_MESSAGE_SIZE);
			sprintf(max_message_size_string,"%d", DEFAULT_MAX_MESSAGE_SIZE);	
		}
		debug_printf("max_message_size_string %s\n", max_message_size_string);
		max_message_size = atoi(max_message_size_string);
	}
	
	if (nb_of_ports_queuing>0){
		
		// looking for max_queueing_messages element
		xmlXPathObjectPtr xpathObj_max;
		xpathExpr = BAD_CAST "//ports/max_queueing_messages";

		debug_printf("max_queueing_messages\n");
		if (getNode(xpathExpr,&xpathObj_max) != 0){
			return -1;
		}
		
		
		if ((xpathObj_max != NULL) 
			&& (xpathObj_max->nodesetval != NULL)
			&& (xpathObj_max->nodesetval->nodeNr>0)){
				strcpy(max_queueing_messages_string, xmlNodeGetContent(xpathObj_max->nodesetval->nodeTab[0]));
				
			}else{
				strcpy(max_queueing_messages_string, DEFAULT_MAX_QUEUING_MESSAGES);			
			}
			max_queueing_messages = atoi(max_queueing_messages_string);
			debug_printf("max_queueing_messages %d\n", max_queueing_messages);
	}
	return 0;
}


// function used for debug
int	debug_printf(const char *format, ...)
{

	int           res = 0;
	va_list       args;

	if (DEBUG){ 
		va_start(args, format);
		res = vprintf (format, args);
		va_end(args);
	}
	return res;
}
