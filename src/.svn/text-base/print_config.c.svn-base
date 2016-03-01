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
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#include "arinc653-xml-conf.h"
#include "Makefile.h"

/*

The functions implemented in this file are used for print the configuration of the application example
(i.e. all files needed by POK)

*/ 

char *macro_names[NUMBER_OF_KERNEL_MACROS]={
	"POK_TEST",						// macro not used any more				 	
	"POK_NEEDS_CONSOLE",				 				         
	"POK_GENERATED_CODE",			// macro not used any more	
	"POK_NEEDS_THREADS",				                         
	"POK_NEEDS_PARTITIONS",			
	"POK_NEEDS_SCHED",					
	"POK_NEEDS_DEBUG",					
	"POK_CONFIG_NB_PARTITIONS",		
	"POK_CONFIG_NB_THREADS",			
	"POK_CONFIG_PARTITIONS_NTHREADS",  
	"POK_NEEDS_SCHED_O1", 				
	"POK_NEEDS_SCHED_FPPS", 			
	"POK_CONFIG_PRIORITY_LEVELS",		
	"POK_CONFIG_PARTITIONS_SIZE",		
	"POK_CONFIG_DEFINE_BASE_VADDR",	
	"POK_CONFIG_PARTITIONS_BASE_VADDR",
	"POK_SKIP_LOADER",					
	"POK_CONFIG_PARTITIONS_LOADADDR", 
	"POK_CONFIG_PARTITIONS_SCHEDULER", 
	"POK_CONFIG_SCHEDULING_SLOTS",		
	"POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION",
	"POK_CONFIG_SCHEDULING_NBSLOTS",				
	"POK_CONFIG_SCHEDULING_MAJOR_FRAME", 
	"POK_USER_STACK_SIZE", 			
	"POK_BUS_FREQ_MHZ", 				
	"POK_FREQ_DIV",					
	"POK_FREQ_SHIFT", 					
	"POK_LOADER_BLOCK_SIZE",			
	"POK_DISABLE_LOADER_DATA_INIT",	
	"POK_PREFILL_PTE",					
	"POK_NEEDS_ARINC653",		
	"POK_NEEDS_GETTICK",			// macro not used any more		
	"POK_NEEDS_PORTS_SAMPLING",		
	"POK_NEEDS_PORTS_QUEUEING",		
	"POK_NEEDS_PORTS_SLOT",
	"POK_CONFIG_NB_PORTS", 			
	"POK_CONFIG_NB_GLOBAL_PORTS", 		
	"POK_CONFIG_MAX_QUEUING_MESSAGES ",
 	"POK_CONFIG_PARTITIONS_PORTS", 	
	"POK_NEEDS_MIDDLEWARE",			// macro not used any more
	"IDLE_STACK_SIZE", 				
	"POK_CONFIG_NB_ASYNCH_EVENTS",		
	"POK_CONFIG_PARTITIONS_NB_ASYNCH_EVENTS", 
	"POK_CONFIG_PARTITIONS_POSTWRITE_TIME",
	"POK_NEEDS_LOCKOBJECTS",
	"POK_CONFIG_NB_LOCKOBJECTS",
	"POK_CONFIG_PARTITIONS_NLOCKOBJECTS",
	"POK_NEEDS_THREAD_SUSPEND",		// macro not used any more
	"POK_CONFIG_PARTITIONS_PERIOD",
	"POK_NEEDS_SCHED_O1_SPLIT",
	"POK_CONFIG_NB_SPORADIC_THREADS",
	"POK_CONFIG_SUCCESSOR_THREADS",
	"POK_CONFIG_SUCCESSORS_ID",
	"POK_CONFIG_PREDECESSOR_THREADS",
	"POK_CONFIG_PREDECESSORS_EVENTS",
	"POK_CONFIG_SPORADIC_TARDINESS",
	"POK_CONFIG_SPORADIC_DEADLINE"
	
};

char *macro_partitions_names[NUMBER_OF_PARTIONS_MACROS]={
	"POK_TEST",									// macro not used any more
	"POK_GENERATED_CODE",
	"POK_NEEDS_LIBC_STDIO",
	"POK_CONFIG_NB_THREADS",
	"POK_NEEDS_ARINC653_PROCESS",
	"POK_NEEDS_ARINC653_PARTITION",
	"POK_NEEDS_ARINC653_TIME",
	"POK_NEEDS_ARINC653",
	"POK_NEEDS_ARINC653_QUEUEING",
	"POK_NEEDS_ARINC653_SAMPLING",
	"POK_CONFIG_NEEDS_FUNC_STRNCPY",
	"POK_CONFIG_MAX_MESSAGE_SIZE",
	"POK_CONFIG_PART_NB_INPUT_QUEUING_PORTS",
	"POK_CONFIG_PART_NB_INPUT_SAMPLING_PORTS",
	"POK_CONFIG_PART_MAX_QUEUING_MESSAGES", 	// Max number of messages in a queuing port
	"POK_CONFIG_NB_SYSTEM_PORTS",
	"POK_NEEDS_EVENTS",
	"POK_CONFIG_NB_EVENTS",
	"POK_NEEDS_ARINC653_EVENT",
	"POK_CONFIG_ARINC653_NB_EVENTS",
	"POK_NEEDS_BLACKBOARDS",		
	"POK_CONFIG_NB_BLACKBOARDS",	
	"POK_NEEDS_ARINC653_BLACKBOARD",
	"POK_NEEDS_MIDDLEWARE",
	"POK_NEEDS_BUFFERS",
	"POK_CONFIG_NB_BUFFERS",
	"POK_NEEDS_ARINC653_BUFFER",
	"POK_CONFIG_MAX_BUFFERS_MESSAGES"
};



int print_config(){

	if (print_init_config() != 0){
		//printf("Error printing init configuration\n");
		return -1;
	}
	
	if (print_kernel_config() != 0){
		//printf("Error printing kernel configuration\n");
		return -1;
	}
	
	if (print_partitions_config() != 0){
		//printf("Error printing partitions configuration\n");	
		return -1;
	}
	
	return 0;
}


int print_init_config(){
	
	DIR* dirPtr = NULL;
	DIR* dirPtrCPU = NULL;
	FILE *GCMakefile;
	FILE *CPUMakefile;
	
	int result_code;
	
	result_code = mkdir(DEFAULT_GC_DIR, S_IRWXU | S_IRWXG | S_IRWXO);

	if (result_code != 0){
		removedirectoryrecursively(DEFAULT_GC_DIR);
		result_code = mkdir(DEFAULT_GC_DIR, S_IRWXU | S_IRWXG | S_IRWXO);
		//printf("ERROR(%d): can not create %s directory\n", result_code,DEFAULT_GC_DIR);
		//printf("Please remove it using the command: make clean-gen-dir\n");
		//return -1;
	}

 	char makefile_name[1000];
 	strcpy(makefile_name,DEFAULT_GC_DIR);
 	strcat(makefile_name,"Makefile");
 	if ( (GCMakefile = fopen(makefile_name,"w")) == NULL){
		printf("ERROR: %s can not be opened\n",makefile_name);
		return -1;
	}
	fprintf(GCMakefile,GENERATEDCODE_MAKEFILE);
	fclose(GCMakefile);
	
	if((dirPtrCPU = opendir(DEFAULT_CPU_DIR)) == NULL) {
		//printf("Couldn't open directory\n");
		
       	int result_code = mkdir(DEFAULT_CPU_DIR, S_IRWXU | S_IRWXG | S_IRWXO);
	
		if (result_code != 0){
			printf("ERROR: can not create the %s directory\n", DEFAULT_CPU_DIR);
			return -1;
		}
 	}
 	
 	strcpy(makefile_name,DEFAULT_CPU_DIR);
 	strcat(makefile_name,"Makefile");
 	if ( (CPUMakefile	= fopen(makefile_name,"w")) == NULL){
		printf("ERROR: %s can not be opened\n",makefile_name);
		return -1;
	}
	fprintf(CPUMakefile,CPU_MAKEFILE_FIRSTPART);
	int i;
	for (i =0; i<nb_partitions; i++){
		fprintf(CPUMakefile,"%s",partitions[i].part_name);
		fprintf(CPUMakefile,"/");
		fprintf(CPUMakefile,"%s",partitions[i].part_name);
		fprintf(CPUMakefile,".elf ");
	}
	fprintf(CPUMakefile,"\n\n");
	fprintf(CPUMakefile,CPU_MAKEFILE_SECONDPART);
	//$(CD) part1&& $(MAKE)
	for (i =0; i<nb_partitions; i++){
		fprintf(CPUMakefile,"\t$(CD) ");
		fprintf(CPUMakefile,"%s",partitions[i].part_name);
		fprintf(CPUMakefile," && $(MAKE)\n");
	}
	fprintf(CPUMakefile,"\n\n");
	
	fprintf(CPUMakefile,"clean: common-clean\n\t$(CD) kernel && $(MAKE) clean\n");	
	for (i =0; i<nb_partitions; i++){
		fprintf(CPUMakefile,"\t$(CD) ");
		fprintf(CPUMakefile,"%s",partitions[i].part_name);
		fprintf(CPUMakefile," && $(MAKE) clean\n");
	}
	fprintf(CPUMakefile,"\n");
	fprintf(CPUMakefile,"distclean: clean\n\t$(CD) kernel && $(MAKE) distclean\n");
	for (i =0; i<nb_partitions; i++){
		fprintf(CPUMakefile,"\t$(CD) ");
		fprintf(CPUMakefile,"%s",partitions[i].part_name);
		fprintf(CPUMakefile," && $(MAKE) distclean\n");
	}
	fprintf(CPUMakefile,"\n");
	fprintf(CPUMakefile,CPU_MAKEFILE_THIRDPART);
	fclose(CPUMakefile);
	
 	return 0;	
	
 	
}


int print_partitions_config(){

	debug_printf("\n*********** PRINTF PARTITIONS CONFIG **********\n");
	int i;
	for (i =0; i< nb_partitions; i++){
		if (print_partition_config(i) != 0){
			printf("ERROR printing partition config %d\n",i);
		}
	}
	return 0;
}


int print_partition_config(int partition_id){
	DIR* dirPtr = NULL;
	char dirName[1000];
	strcpy(dirName,DEFAULT_CPU_DIR);
	strcat(dirName,partitions[partition_id].part_name);

	if((dirPtr = opendir(dirName)) == NULL) {
		//printf("Couldn't open directory\n");
		
       	int result_code = mkdir(dirName, S_IRWXU | S_IRWXG | S_IRWXO);
	
		if (result_code != 0){
			printf("ERROR: can not create the %s directory\n",dirName);
			return -1;
		}
 	}
 	
 	if (print_partition_makefile(partition_id) != 0){
			return -1;
	}
 	if (print_partition_config_deployment_dot_h(partition_id) != 0){
			return -1;
	}
		
	if (print_partition_config_deployment_dot_c(partition_id) != 0){
			return -1;
	}
	if (print_partition_config_main_dot_c(partition_id) != 0){
			return -1;
	}
	if (print_partition_config_activity_dot_c(partition_id) != 0){
			return -1;
	}
	if (print_partition_config_activity_dot_h(partition_id) != 0){
			return -1;
	}
	return 0;
}

int print_partition_makefile(int partition_id){
	
	FILE *makefile;
	char makefile_filename[1000];
	strcpy(makefile_filename,DEFAULT_CPU_DIR);
	strcat(makefile_filename,partitions[partition_id].part_name);
	strcat(makefile_filename, "/Makefile");
	
	if ( (makefile	= fopen(makefile_filename,"w")) == NULL){
		debug_printf("ERROR: file %s can not be opened\n", makefile_filename);
		return -1;
	}
	
	fprintf(makefile,PART_MAKEFILE_FIRSTPART);
	fprintf(makefile,"%s",partitions[partition_id].part_name);
	fprintf(makefile,".elf\n");
	fprintf(makefile,"OBJS = main.o activity.o deployment.o\n");
	fprintf(makefile,"LINKERSCRIPT = partition_");
	fprintf(makefile,"%s",number_to_string[partition_id+1]);
	fprintf(makefile,".lds\n");
	fprintf(makefile,PART_MAKEFILE_SECONDPART);
	fclose(makefile);
	return 0;

}

int print_partition_config_deployment_dot_h(int partition_id){
	
	FILE *part_deployment_dot_h;
	char part_deployment_dot_h_filename[1000];
	strcpy(part_deployment_dot_h_filename, DEFAULT_CPU_DIR);
	strcat(part_deployment_dot_h_filename,partitions[partition_id].part_name);
	strcat(part_deployment_dot_h_filename, "/deployment.h");
	
	if ( (part_deployment_dot_h	= fopen(part_deployment_dot_h_filename,"w")) == NULL){
		printf("ERROR: file %s can not be opened\n", part_deployment_dot_h_filename);
		return -1;
	}
	
	fprintf(part_deployment_dot_h,DEPLOYMENT_H_);
	
	int i;
	for (i =0; i<NUMBER_OF_PARTIONS_MACROS; i++){
		if (macro_partitions_enabled[partition_id][i]){
			debug_printf("#define %s\t%s\n\n",macro_partitions_names[partition_id,i],(char *)(macro_partitions_values[partition_id][i]));
			fprintf(part_deployment_dot_h,"#define %s\t%s\n\n",macro_partitions_names[partition_id,i],(char *)(macro_partitions_values[partition_id][i]));
		}
	}
	
	fprintf(part_deployment_dot_h,"#endif");
	fclose(part_deployment_dot_h);
	return 0;

}

int print_partition_config_deployment_dot_c(int partition_id){
	FILE *part_deployment_dot_c;
	
	char part_deployment_dot_c_filename[1000];
	strcpy(part_deployment_dot_c_filename, DEFAULT_CPU_DIR);
	strcat(part_deployment_dot_c_filename,partitions[partition_id].part_name);
	strcat(part_deployment_dot_c_filename, "/deployment.c");
	
	
	if ( (part_deployment_dot_c	= fopen(part_deployment_dot_c_filename,"w")) == NULL){
		printf("ERROR: file %s can not be opened\n", part_deployment_dot_c_filename);
		return -1;
	}
	
	//printf ("partition id %d scheduler type %d\n",partition_id,partitions[partition_id].scheduler_type);
	
	if ((partitions[partition_id].scheduler_type == O1) || (partitions[partition_id].scheduler_type == O1_SPLIT)){

		if ((partitions[partition_id].nb_of_input_sampling_port > 0) || 
			(partitions[partition_id].nb_of_input_queuing_port > 0)  ||
			(partitions[partition_id].nb_events_blackb_buffers > 0) ){	
			fprintf(part_deployment_dot_c,"%s\n","#include <arinc653/types.h>");
		}
	
		if ((partitions[partition_id].nb_of_input_sampling_port>0) || 
			(partitions[partition_id].nb_of_input_queuing_port>0)){	
			fprintf(part_deployment_dot_c,"%s\n","#include \"deployment.h\"\n\n");
			fprintf(part_deployment_dot_c,"%s\n",partitions[partition_id].receiving_variables);
			fprintf(part_deployment_dot_c,"%s\n",partitions[partition_id].receiving_addresses);
		}
		
		if (partitions[partition_id].nb_events >0){
			fprintf(part_deployment_dot_c,"%s\n","#include <arinc653/event.h>");
		} 
		
		if (partitions[partition_id].nb_blackboards >0){
			fprintf(part_deployment_dot_c,"%s\n","#include <arinc653/blackboard.h>");
		} 

		if (partitions[partition_id].nb_buffers >0){
			fprintf(part_deployment_dot_c,"%s\n","#include <arinc653/buffer.h>");
		} 
		
		
		fprintf(part_deployment_dot_c,"%s\n","");
	
	if (partitions[partition_id].nb_of_input_sampling_port>0){	
			fprintf(part_deployment_dot_c,"%s\n",partitions[partition_id].sampling_input_msg_data);
			fprintf(part_deployment_dot_c,"%s\n",partitions[partition_id].input_buffers);
		
		}
		if (partitions[partition_id].nb_of_input_queuing_port>0){	
			fprintf(part_deployment_dot_c,"%s\n",partitions[partition_id].queuing_input_msg_data_buffers);
			fprintf(part_deployment_dot_c,"%s\n",partitions[partition_id].input_buffers_queuing);
		
		}	
	}	

	if (partitions[partition_id].nb_events>0){	
			fprintf(part_deployment_dot_c,"%s\n",partitions[partition_id].events_ids_array);
			fprintf(part_deployment_dot_c,"%s",partitions[partition_id].events_names_array);
			fprintf(part_deployment_dot_c,"%s\n",partitions[partition_id].events_names_array_values);
	}
	
	if (partitions[partition_id].nb_blackboards>0){	
			fprintf(part_deployment_dot_c,"%s\n",partitions[partition_id].blackboards_ids_array);
			fprintf(part_deployment_dot_c,"%s",partitions[partition_id].blackboards_names_array);
			fprintf(part_deployment_dot_c,"%s\n",partitions[partition_id].blackboards_names_array_values);
	}
	
	if (partitions[partition_id].nb_buffers>0){	
			fprintf(part_deployment_dot_c,"%s\n",partitions[partition_id].buffers_ids_array);
			fprintf(part_deployment_dot_c,"%s",partitions[partition_id].buffers_names_array);
			fprintf(part_deployment_dot_c,"%s\n",partitions[partition_id].buffers_names_array_values);
	}
	fclose(part_deployment_dot_c);
	return 0;

}

int print_partition_config_main_dot_c(int partition_id){

	FILE *part_main_dot_c;
	
	char part_main_dot_c_filename[1000];
	strcpy(part_main_dot_c_filename, DEFAULT_CPU_DIR);
	strcat(part_main_dot_c_filename, partitions[partition_id].part_name);
	strcat(part_main_dot_c_filename, "/main.c");
	
	if ( (part_main_dot_c	= fopen(part_main_dot_c_filename,"w")) == NULL){
		printf("ERROR: file %s can not be opened\n", part_main_dot_c_filename);
		return -1;
	}
	
	
	fprintf(part_main_dot_c,"%s\n",partitions[partition_id].main_c);
	
	
	fprintf(part_main_dot_c,"%s\n","\treturn 0;\n}");
	
	fclose(part_main_dot_c);
	
	return 0;
	
}

int print_partition_config_activity_dot_c(int partition_id){
	
	FILE *part_activity_dot_c;
	
	char part_activity_dot_c_filename[1000];
	strcpy(part_activity_dot_c_filename, DEFAULT_CPU_DIR);
	strcat(part_activity_dot_c_filename, partitions[partition_id].part_name);
	strcat(part_activity_dot_c_filename, "/activity.c");
	
	if ( (part_activity_dot_c	= fopen(part_activity_dot_c_filename,"w")) == NULL){
		printf("ERROR: file %s can not be opened\n", part_activity_dot_c_filename);
		return -1;
	}

	fprintf(part_activity_dot_c,"%s\n",partitions[partition_id].activity_c);
	
	fclose(part_activity_dot_c);
	
	return 0;
	
}

int print_partition_config_activity_dot_h(int partition_id){
	
	FILE *part_activity_dot_h;
	
	char part_activity_dot_h_filename[1000];
	strcpy(part_activity_dot_h_filename, DEFAULT_CPU_DIR);
	strcat(part_activity_dot_h_filename, partitions[partition_id].part_name);
	strcat(part_activity_dot_h_filename, "/activity.h");
	
	if ( (part_activity_dot_h	= fopen(part_activity_dot_h_filename,"w")) == NULL){
		printf("ERROR: file %s can not be opened\n", part_activity_dot_h_filename);
		return -1;
	}

	fprintf(part_activity_dot_h,"%s\n",ACTIVITY_H_);	
	fprintf(part_activity_dot_h,"%s\n",partitions[partition_id].activity_h);

	fprintf(part_activity_dot_h,"%s\n","#endif");

	fclose(part_activity_dot_h);
	
	return 0;
}

int print_kernel_config(){
	
	debug_printf("\n*********** PRINT KERNEL CONFIG **********\n");
	
	DIR* dirPtr = NULL;
	char dirName[1000];
	strcpy(dirName,DEFAULT_KERNEL_DIR);
	
	if ((dirPtr = opendir(dirName)) == NULL) {
		//printf("Couldn't open %s directory\n", dirName);
		
       	int result_code = mkdir(dirName, S_IRWXU | S_IRWXG | S_IRWXO);
	
		if (result_code != 0){
			printf("ERROR: can not create %s directory\n",dirName);
			return -1;
		}
 	}
 	
 	if (print_kernel_makefile() != 0){
		return -1;
	}
	
	if (print_kernel_config_deployment_dot_h() != 0){
		return -1;
	}
		
	if (print_kernel_config_deployment_dot_c() != 0){
		return -1;
	}	
	
 	return 0;
	
}

int print_kernel_makefile(){
	
	FILE *makefile;
	char makefile_name[1000];
	strcpy(makefile_name,DEFAULT_KERNEL_DIR);
	strcat(makefile_name,"Makefile");
	
	if ( (makefile	= fopen(makefile_name,"w")) == NULL){
		printf("ERROR: file %s can not be opened\n",makefile_name);
		return -1;
	}
	int i=0;
	
	fprintf(makefile,KERNEL_MAKEFILE);
	fclose(makefile);
	
	return 0;
}

int print_kernel_config_deployment_dot_h(){
	
	FILE *kernel_deployment_dot_h;
	
	char file_name[1000];
	strcpy(file_name,DEFAULT_KERNEL_DIR);
	strcat(file_name,"deployment.h");
	

	
	if ( (kernel_deployment_dot_h	= fopen(file_name,"w")) == NULL){
		printf("ERROR: file %s can not be opened\n",file_name);
		return -1;
	}
	int i=0;
	
	fprintf(kernel_deployment_dot_h,DEPLOYMENT_H_);
	
	if (!(macro_enabled[POK_NEEDS_SCHED_O1])){
		macro_enabled[POK_CONFIG_NB_ASYNCH_EVENTS] =  FALSE;
		macro_enabled[POK_CONFIG_PARTITIONS_NB_ASYNCH_EVENTS] =  FALSE;
		macro_enabled[POK_CONFIG_PARTITIONS_POSTWRITE_TIME] =  FALSE;
		macro_enabled[POK_NEEDS_PORTS_SLOT] =  FALSE;
	}
	
	
	fprintf(kernel_deployment_dot_h,"#include <core/schedvalues.h>\n\n");
	
	for (i=0; i<NUMBER_OF_KERNEL_MACROS;i++){
		if (macro_enabled[i]){
			
			debug_printf("%d - #define %s\t%s\n\n",i,macro_names[i],macro_values[i]);
			fprintf(kernel_deployment_dot_h,"#define %s\t%s\n\n",macro_names[i],macro_values[i]);
		}
	}
	debug_printf("%s\n\n",pok_port_local_identifier_t);
	fprintf(kernel_deployment_dot_h,"%s\n\n",pok_port_local_identifier_t);
	debug_printf("%s\n\n",pok_port_identifier_t);
	fprintf(kernel_deployment_dot_h,"%s\n\n",pok_port_identifier_t);
	
	fprintf(kernel_deployment_dot_h,"#endif");
	fclose(kernel_deployment_dot_h);
	
	return 0;
	
}

int print_kernel_config_deployment_dot_c(){
	
	FILE *kernel_deployment_dot_c;
	char file_name[1000];
	strcpy(file_name,DEFAULT_KERNEL_DIR);
	strcat(file_name,"deployment.c");

	if ( (kernel_deployment_dot_c	= fopen(file_name,"w")) == NULL){
		printf("ERROR: file  %s can not be opened\n", file_name);
		return -1;
	}

	
	if (nb_ports>0){
		
		fprintf(kernel_deployment_dot_c,"%s\n","#include \"deployment.h\"");
/*		fprintf(kernel_deployment_dot_c,"%s\n","#include <core/partition.h>");
		fprintf(kernel_deployment_dot_c,"%s\n","#include <core/error.h>");
		fprintf(kernel_deployment_dot_c,"%s\n","#include <core/kernel.h>");
		fprintf(kernel_deployment_dot_c,"%s\n","#include <types.h>");
		fprintf(kernel_deployment_dot_c,"%s\n","#include <middleware/token.h>");
		fprintf(kernel_deployment_dot_c,"%s\n\n\n","#include <middleware/sizes.h>");
*/
	
		if (nb_ports>0){
			fprintf(kernel_deployment_dot_c,"%s\n","#include <types.h>");
			fprintf(kernel_deployment_dot_c,"%s\n","#include <middleware/port.h>\n\n");
		}


		debug_printf("%s\n",	var_for_pok_ports_by_partition);
		fprintf(kernel_deployment_dot_c,"%s\n",var_for_pok_ports_by_partition);
		
		debug_printf("%s\n",	pok_ports_by_partition);
		fprintf(kernel_deployment_dot_c,"%s\n\n",pok_ports_by_partition);
		
		debug_printf("%s\n",	pok_ports_nb_ports_by_partition);
		fprintf(kernel_deployment_dot_c,"%s\n\n",pok_ports_nb_ports_by_partition);
		
		debug_printf("%s\n",	pok_global_ports_to_local_ports);
		fprintf(kernel_deployment_dot_c,"%s\n\n",pok_global_ports_to_local_ports);
		
		debug_printf("%s\n",	pok_local_ports_to_global_ports);
		fprintf(kernel_deployment_dot_c,"%s\n\n",pok_local_ports_to_global_ports);
		
		debug_printf("%s\n",	pok_ports_names);
		fprintf(kernel_deployment_dot_c,"%s\n\n",pok_ports_names);
		
		debug_printf("%s\n",	pok_ports_nb_destinations);
		fprintf(kernel_deployment_dot_c,"%s\n\n",pok_ports_nb_destinations);

		debug_printf("%s\n",	pok_ports_kind);
		fprintf(kernel_deployment_dot_c,"%s\n\n",pok_ports_kind);
		
		debug_printf("%s\n",	deployment_destinations);
		fprintf(kernel_deployment_dot_c,"%s",deployment_destinations);
		debug_printf("%s\n",	pok_ports_destinations);
		fprintf(kernel_deployment_dot_c,"%s\n\n",pok_ports_destinations);
	
		if (macro_enabled[POK_NEEDS_PORTS_SLOT]){
			
			debug_printf("%s\n",	slot_pok_outputports_to_flush);
			fprintf(kernel_deployment_dot_c,"%s",slot_pok_outputports_to_flush);
			debug_printf("%s\n",	pok_outputports_to_flush);
			fprintf(kernel_deployment_dot_c,"%s\n",pok_outputports_to_flush);
		
			debug_printf("%s\n",	pok_nb_outputports_to_flush);
			fprintf(kernel_deployment_dot_c,"%s\n\n",pok_nb_outputports_to_flush);
		
			debug_printf("%s\n",slot_pok_inputports_to_preload);
			fprintf(kernel_deployment_dot_c,"%s",slot_pok_inputports_to_preload);
			debug_printf("%s\n",pok_inputports_to_preload);
			fprintf(kernel_deployment_dot_c,"%s\n",pok_inputports_to_preload);
			debug_printf("%s\n",	pok_nb_inputports_to_preload);
			fprintf(kernel_deployment_dot_c,"%s\n\n",pok_nb_inputports_to_preload);
		
		}
	}

	fclose(kernel_deployment_dot_c);
	
	return 0;
}

int removedirectoryrecursively(const char *dirname)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

   
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("ERROR opendir()");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(path, (size_t) PATH_MAX, "%s/%s", dirname, entry->d_name);
            if (entry->d_type == DT_DIR) {
                removedirectoryrecursively(path);
            }

            /*
             * Here, the actual deletion must be done.  Beacuse this is
             * quite a dangerous thing to do, and this program is not very
             * well tested, we are just printing as if we are deleting.
             */
            debug_printf("Deleting: %s\n", path);
			remove(path);
            /*
             * When you are finished testing this and feel you are ready to do the real
             * deleting, use this: remove*STUB*(path);
             * (see "man 3 remove")
             * Please note that I DONT TAKE RESPONSIBILITY for data you delete with this!
             */
        }

    }
    closedir(dir);

    /*
     * Now the directory is emtpy, finally delete the directory itself. (Just
     * printing here, see above) 
     */
    debug_printf("Deleting: %s\n", dirname);
	rmdir(dirname);

    return 0;
}


