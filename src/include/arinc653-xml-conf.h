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

// program default settings

#define DEFAULT_SCHEMA 								"xml/Application.xsd"

#define DEFAULT_FILENAME 							"xml/arinc653-mix.xml"
#define DEFAULT_GC_DIR								"generated-code/"
#define DEFAULT_CPU_DIR								"generated-code/cpu/"
#define DEFAULT_KERNEL_DIR							"generated-code/cpu/kernel/"
#define DEBUG										1
#define DEFAULT_TIME_CAPACITY_INIT_VALUE			2000	// used to assign time capacity to the thread in the SCHED_01_SPLIT
#define DEFAULT_TIME_CAPACITY_SUCC_THREAD			10		// used to assign time capacity to the succ thread in the SCHED_01_SPLIT

// default values for ports
#define	DEFAULT_SAMPLING_PORT_REFRESH_PERIOD		"2000"
#define	DEFAULT_QUEUING_DISCIPLINE_TYPE				"0"
#define	DEFAULT_RECEIVE_QUEUING_MESSAGE_TIME_OUT	"0"
#define	DEFAULT_SEND_QUEUING_MESSAGE_TIME_OUT		"0"
#define DEFAULT_MAX_MESSAGE_SIZE					512 // must be an integer
#define DEFAULT_MAX_QUEUING_MESSAGES				"2"
#define DEFAULT_BLACKBOARD_MAX_MESSAGE_SIZE			"256"
#define DEFAULT_BUFFER_MAX_MESSAGE_SIZE				"256"

// default values for kernel array
#define	DEFAULT_ARINC_THREADS_ARRAYNAME				"arinc_threads"

//default names for slot arrays
#define DEFAULT_OUTPUTPORTS_TO_FLUSH_ARRAYNAME		"pok_outputports_to_flush"		// NOT TO MODIFY -- declared as extern in kernels files
#define DEFAULT_NB_OUTPUTPORTS_TO_FLUSH_ARRAYNAME	"pok_nb_outputports_to_flush"	// NOT TO MODIFY -- declared as extern in kernels files
#define DEFAULT_INPUT_PORT_TO_PRELOAD_ARRAYNAME		"pok_inputports_to_preload"		// NOT TO MODIFY -- declared as extern in kernels files
#define DEFAULT_NB_INPUT_PORT_TO_PRELOAD_ARRAYNAME	"pok_nb_inputports_to_preload"	// NOT TO MODIFY -- declared as extern in kernels files

// default names for queuing&sampling arrays
#define DEFAULT_INPUT_BUFFERS_QUEUING_ARRAYNAME		"input_buffers_queuing" 	// NOT TO MODIFY -- declared as extern in libpok files
#define DEFAULT_INPUT_BUFFERS_SAMPLING_ARRAYNAME	"input_buffers"				// NOT TO MODIFY -- declared as extern in libpok files
#define DEFAULT_RECEIVING_ADDRESSES_ARRAYNAME		"receiving_addresses"		// NOT TO MODIFY -- declared as extern in libpok files


// default names for event array
#define	DEFAULT_EVENTS_IDS_ARRAYNAME				"pok_arinc653_events_ids"
#define	DEFAULT_EVENTS_NAMES_ARRAYNAME				"pok_arinc653_events_names" // NOT TO MODIFY -- this array is declared as extern in 
																				// arinc653/event.c file

// default names for blackboard array
#define	DEFAULT_BLACKBOARDS_IDS_ARRAYNAME			"pok_blackboards_ids"
#define	DEFAULT_BLACKBOARDS_NAMES_ARRAYNAME			"pok_blackboards_names"		// NOT TO MODIFY -- this array is declared as extern in 
																				// middleware/blackboardcreate.c and 																					// middleware/blackboardid.c file
// default names for buffer array
#define	DEFAULT_BUFFERS_IDS_ARRAYNAME				"pok_buffers_ids"
#define	DEFAULT_BUFFERS_NAMES_ARRAYNAME				"pok_buffers_names" 		// NOT TO MODIFY -- this array is declared as extern in 

/*
// default name for TEST array
#define	DEFAULT_PORTS_MESSAGE_SIZE				"unsigned int pok_ports_message_size[POK_CONFIG_NB_PORTS] "
#define	DEFAULT_PORTS_MESSAGES					"unsigned char pok_ports_messages[POK_CONFIG_NB_PORTS][512] " 
																				// DEFAULT_MAX_MESSAGE_SIZE 	
																				// NOT TO MODIFY -- this array is declared as extern in 
																				// middleware/portflusall.c and middleware/bufferid.c file
*/

#define	DEFAULT_MAX_BUFFERS_MESSAGES				"3"
#define	DEFAULT_SEND_BUFFER_TIMEOUT					"10000"
#define	DEFAULT_RECEIVE_BUFFER_TIMEOUT				"10000"

// default macros values
#define	DEFAULT_PRIORITY_LEVELS						"20"
//#define DEFAULT_POK_TEST				 			FALSE
//#define DEFAULT_POK_TEST_PART		 				FALSE
#define DEFAULT_POK_SKIP_LOADER 					FALSE
#define MAX_DEFAULT_PARTITIONS_SUPPORTED			3							// size of the following arrays 
#define DEFAULT_PARTITIONS_SIZE						{120000, 120000, 120000}
#define DEFAULT_PARTITIONS_BASE_VADDR				{0x1000, 0x4000, 0x7000}
#define DEFAULT_PARTITIONS_LOADADDR					{0x9f000, 0xc0000, 0xe1000}
#define	DEFAULT_POK_BUS_FREQ_MHZ					"74"
#define	DEFAULT_POK_FREQ_DIV						"1"
#define	DEFAULT_POK_FREQ_SHIFT						"0"
#define	DEFAULT_IDLE_STACK_SIZE						"4096"
#define	DEFAULT_POK_USER_STACK_SIZE					"8192"


// constant used in the program
#define NUMBER_OF_KERNEL_MACROS						57
#define NUMBER_OF_PARTIONS_MACROS					28
#define TRUE 										1
#define FALSE 										0
#define NB_SCHEDULER_TYPES 							3
#define MAX_PARTITIONS 								10
#define MAX_EVENTS_PER_PARTITION 					50
#define MAX_BLACKBOARDS_PER_PARTITION 				50
#define MAX_BUFFERS_PER_PARTITION 					50
#define MAX_EVENT_NAME_LENGTH 						30
#define MAX_BLACKBOARD_NAME_LENGTH 					30
#define MAX_BUFFER_NAME_LENGTH 						30
#define MAX_PRIORITY_LEVELS 						100

#define MAX_SLOTS	 								15
#define MAX_PORTS									100
#define MAX_THREADS_PER_PARTITION 				100
#define DEPLOYMENT_H_ "#ifndef __XML_GENERATED_DEPLOYMENT_H_\n#define __XML_GENERATED_DEPLOYMENT_H_\n\n"
#define ACTIVITY_H_   "#ifndef __XML_GENERATED_ACTIVITY_H_\n#define __XML_GENERATED_ACTIVITY_H_\n\n"
#define MESSAGE_IN_PROCESS							"\n\t\t/***************************************************************/\
\n\t\t/* Message processing code should be placed here               */\
\n\t\t/***************************************************************/\n\n"


typedef enum
{
	POK_TEST					 	= 0,	// TO BE REMOVED 
	POK_NEEDS_CONSOLE				= 1,	// Fixed. Value: 1
	POK_GENERATED_CODE				= 2,	// Fixed. Value: 1
	POK_NEEDS_THREADS				= 3,	// Fixed. Value: 1
	POK_NEEDS_PARTITIONS			= 4,	// Fixed. Value: 1
	POK_NEEDS_SCHED					= 5,	// Fixed. Value: 1
	POK_NEEDS_DEBUG					= 6,
	POK_CONFIG_NB_PARTITIONS		= 7,
	POK_CONFIG_NB_THREADS			= 8,
	POK_CONFIG_PARTITIONS_NTHREADS  = 9,
	POK_NEEDS_SCHED_O1 				= 10,
	POK_NEEDS_SCHED_FPPS 			= 11,
	POK_CONFIG_PRIORITY_LEVELS		= 12,	// Default value: DEFAULT_PRIORITY_LEVELS
	POK_CONFIG_PARTITIONS_SIZE		= 13,
	POK_CONFIG_DEFINE_BASE_VADDR	= 14,	// Fixed. Value: 1
	POK_CONFIG_PARTITIONS_BASE_VADDR	= 15, 	// Default: 	 {0x1000,0x4000}
	POK_SKIP_LOADER					= 16,	// Fixed. Not DEFINED
	POK_CONFIG_PARTITIONS_LOADADDR = 17,	// Fixed. Value: {0x9f000,0xc0000}
	POK_CONFIG_PARTITIONS_SCHEDULER = 18,
	POK_CONFIG_SCHEDULING_SLOTS		= 19,
	POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION = 20,
	POK_CONFIG_SCHEDULING_NBSLOTS	= 21,	
	POK_CONFIG_SCHEDULING_MAJOR_FRAME =	22,
	POK_USER_STACK_SIZE 			= 23,
	POK_BUS_FREQ_MHZ 				= 24,
	POK_FREQ_DIV					= 25, 
	POK_FREQ_SHIFT 					= 26,
	POK_LOADER_BLOCK_SIZE			= 27,	// Disabled
	POK_DISABLE_LOADER_DATA_INIT	= 28,	// Fixed. Value: 1
	POK_PREFILL_PTE					= 29,	// Fixed. Value: 1
	POK_NEEDS_ARINC653				= 30,	// Fixed. Value: 1
	POK_NEEDS_GETTICK				= 31,	// Fixed. Value: 1
	POK_NEEDS_PORTS_SAMPLING		= 32,	
	POK_NEEDS_PORTS_QUEUEING		= 33,
	POK_NEEDS_PORTS_SLOT			= 34,
	POK_CONFIG_NB_PORTS 			= 35,
	POK_CONFIG_NB_GLOBAL_PORTS 		= 36,					 
	POK_CONFIG_MAX_QUEUING_MESSAGES = 37,	
 	POK_CONFIG_PARTITIONS_PORTS 	= 38,
//	POK_NEEDS_MIDDLEWARE			= 39,	// Fixed. Value: 1	
	IDLE_STACK_SIZE 				= 40,
	POK_CONFIG_NB_ASYNCH_EVENTS		= 41,
	POK_CONFIG_PARTITIONS_NB_ASYNCH_EVENTS = 42,
	POK_CONFIG_PARTITIONS_POSTWRITE_TIME = 43,
	POK_NEEDS_LOCKOBJECTS 			= 44,
	POK_CONFIG_NB_LOCKOBJECTS		= 45,
	POK_CONFIG_PARTITIONS_NLOCKOBJECTS = 46,
//	POK_NEEDS_THREAD_SUSPEND		= 47,
	POK_CONFIG_PARTITIONS_PERIOD	= 48,
	POK_NEEDS_SCHED_O1_SPLIT			= 49,
	POK_CONFIG_NB_SPORADIC_THREADS	= 50,
	POK_CONFIG_SUCCESSOR_THREADS	= 51,
	POK_CONFIG_SUCCESSORS_ID		= 52,
	POK_CONFIG_PREDECESSOR_THREADS	= 53,
	POK_CONFIG_PREDECESSORS_EVENTS	= 54,
	POK_CONFIG_SPORADIC_TARDINESS	= 55,
	POK_CONFIG_SPORADIC_DEADLINE	= 56
}kernel_macros;



typedef enum
{
//	POK_TEST_PART					= 0,
	POK_GENERATED_CODE_PART 		= 1,
	POK_NEEDS_LIBC_STDIO 			= 2,
	POK_CONFIG_PART_NB_THREADS		= 3,
	POK_NEEDS_ARINC653_PROCESS		= 4,
	POK_NEEDS_ARINC653_PARTITION	= 5,
	POK_NEEDS_ARINC653_TIME			= 6,
	POK_NEEDS_ARINC653_PART	= 7,
	POK_NEEDS_ARINC653_QUEUEING 	= 8,
	POK_NEEDS_ARINC653_SAMPLING 	= 9,
	POK_CONFIG_NEEDS_FUNC_STRNCPY 	= 10,
	POK_CONFIG_MAX_MESSAGE_SIZE		= 11,
	POK_CONFIG_PART_NB_INPUT_QUEUING_PORTS 		= 12,
	POK_CONFIG_PART_NB_INPUT_SAMPLING_PORTS 	= 13,
	POK_CONFIG_PART_MAX_QUEUING_MESSAGES		= 14, // Max number of messages in a queuing port
	POK_CONFIG_NB_SYSTEM_PORTS					= 15,
	POK_NEEDS_EVENTS				= 16,
	POK_CONFIG_NB_EVENTS			= 17,
	POK_NEEDS_ARINC653_EVENT		= 18,
	POK_CONFIG_ARINC653_NB_EVENTS 	= 19,
	POK_NEEDS_BLACKBOARDS			= 20,
	POK_CONFIG_NB_BLACKBOARDS		= 21,
	POK_NEEDS_ARINC653_BLACKBOARD	= 22,
	POK_CONFIG_MAX_BLACKBOARD_MESSAGE_SIZE	= 23,
	POK_NEEDS_BUFFERS				= 24,
	POK_CONFIG_NB_BUFFERS			= 25,
	POK_NEEDS_ARINC653_BUFFER		= 26,
	POK_CONFIG_MAX_BUFFERS_MESSAGES = 27,
	POK_CONFIG_MAX_BUFFER_MESSAGE_SIZE  = 28
}partitions_macros;

typedef enum
{
	FPPS=0,		
	O1=1,
	O1_SPLIT=2
}scheduler_t;


typedef enum
{
	SOURCE = 0,
	DESTINATION = 1
}port_direction_t;

typedef enum
{
	QUEUING = 1,
	SAMPLING = 2
}port_kind_t;


typedef int boolean;

typedef  char port_id_t[20];
typedef  int port_numberic_id_t;

struct port_list_item_t
{
	int port_id;
	int user_port_id;
	struct port_list_item_t *next;
};


typedef enum
{
	NORMAL = 0,
	EVENT_RESUME = 1,
	EVENT_WAIT = 2,
	EVENT_SET = 3,
	EVENT_RESET = 4,
	BB_DISPLAY = 5,
	BB_SUSPEND = 6,
	BB_RESUME = 7,
	BB_READ = 8,
	BUFFER_SEND = 9,
	BUFFER_RECEIVE = 10,
	BUFFER_RESUME_SENDER = 11,
	BUFFER_RESUME_RECEIVER = 12,
	EVENT_SETTER = 13,
	EVENT_PREDECESSOR = 14,
	EVENT_SUCCESSOR = 15	
}thread_type;

typedef enum
{
	SET = 0,
	WAIT = 1
}event_act;

typedef struct{
//	char		event_name[MAX_EVENT_NAME_LENGTH];
	boolean 		set;
	boolean			wait;
}event_t;

typedef enum
{
	DISPLAY = 0,
	READ = 1
}blackboard_act;

typedef struct{
//	char		event_name[MAX_EVENT_NAME_LENGTH];
	boolean 		display;
	boolean			read;
}blackboard_t;

typedef enum
{
	SEND = 0,
	RECEIVE = 1
}buffer_act;

typedef struct{
//	char		event_name[MAX_EVENT_NAME_LENGTH];
	boolean 		send;
	boolean			receive;
}buffer_t;

typedef struct{
	int 		thread_nb;
	int 		priority; 
	struct 		port_list_item_t 	*ports;
	boolean 	hasInputSamplingPort;
	thread_type	type;
	int			event_nb;
	int			blackboard_nb;
	int			buffer_nb;
	int 		successor_id; // in case thread has type EVENT_PREDECESSOR
//	char		blackboard_name[MAX_BLACKBOARDS_PER_PARTITION];	
//	char		buffer_name[MAX_BUFFERS_PER_PARTITION];	
	char		entry_point[50];
}thread_t;


typedef struct
{
	int		user_partition_id;				// partition id specified in the xml file
	char 	part_name[40];					// partition name, e.g. part1
	scheduler_t 	scheduler_type;
	int 	nb_of_threads;					// included main
	thread_t 	threads[MAX_THREADS_PER_PARTITION];
	int 		priority_levels[MAX_PRIORITY_LEVELS];	// check if a given priority level has been already assigned
	char 	size[15];						
	char 	base_vaddr[10];
	char 	loadaddr[10];
	int 	period;
	int 	nb_slots;
	struct 	port_list_item_t 	*ports;		// list of all partition ports
	int 	nb_of_ports;
	int 	nb_of_sampling_port;
	int 	nb_of_queuing_port;
	int 	nb_of_input_sampling_port;
	int 	nb_of_input_queuing_port;
	int 	nb_asynch_events;
	int 	nb_events;
	int 	nb_blackboards;
	int		nb_buffers;
	char	events_names[MAX_EVENTS_PER_PARTITION][MAX_EVENT_NAME_LENGTH];
	char	blackboards_names[MAX_BLACKBOARDS_PER_PARTITION][MAX_BLACKBOARD_NAME_LENGTH];	
	char	buffers_names[MAX_BUFFERS_PER_PARTITION][MAX_BUFFER_NAME_LENGTH];	
	event_t			events[MAX_EVENTS_PER_PARTITION];
	blackboard_t	blackboards[MAX_BLACKBOARDS_PER_PARTITION];
	buffer_t		buffers[MAX_BUFFERS_PER_PARTITION];
	char	blackboards_max_message_size[4];
	char	max_buffers_msgs[3];
	char	buffers_max_message_size[4];
	int		nb_events_blackb_buffers;
	char	receiving_variables[3000];
	char	receiving_addresses[3000];
	char	sampling_input_msg_data[2000];
	char	input_buffers[2000];
	char	queuing_input_msg_data_buffers[10000];
	char	input_buffers_queuing[10000];
	char	events_ids_array[100];			// e.g. uint8_t pok_events_ids[1];
	char	events_names_array[1000];		// e.g. char* pok_arinc653_events_names[POK_CONFIG_ARINC653_NB_BLACKBOARDS]
	char	events_names_array_values[1000];// e.g.  = {"p1ev1"};
	char	blackboards_ids_array[100];		
	char	blackboards_names_array[1000];		
	char	blackboards_names_array_values[1000];
	char	buffers_ids_array[100];
	char	buffers_names_array[1000];
	char	buffers_names_array_values[1000];
	char	main_c[200000];
	char	activity_c[200000];
	char	activity_h[100000];
}partition_t;


typedef struct
{
	int					user_port_id;
	port_id_t			local_id;
	port_id_t			global_id;
	port_numberic_id_t 	local_numeric_id;
	port_numberic_id_t 	global_numeric_id;
	char				receiving_var_name[30];
	char				name[40];		
	port_kind_t 		kind;
	port_direction_t 	direction;
	int 				user_partition_id;
	int 				partition_id;
	struct port_list_item_t *destinations;
	int				nb_of_destinations;
	char			default_message[DEFAULT_MAX_MESSAGE_SIZE * 6];
	char			default_message_size[5];
}port_t;

// global variables
int nb_partitions;
int nb_ports;
int nb_slots;
int nb_asynch_events;
int nb_priority_levels;
int nb_events;
int nb_blackboards;
int nb_buffers;
int nb_events_blackb_buffers;
int nb_of_ports_sampling;
int nb_of_ports_queuing;
int nb_of_sporadic_threads;
int priority_for_aperiodic;

// array of ports
port_t ports[MAX_PORTS];

// array of partitions
partition_t partitions[MAX_PARTITIONS];

int max_message_size;
char max_message_size_string[5];

int max_queueing_messages;
char max_queueing_messages_string[5];

char pok_port_local_identifier_t[1000];
char pok_port_identifier_t[1000];

char var_for_pok_ports_by_partition[1000];	// e.g. uint8_t pr1_partport[4] = {pr1_SS, pr1_SD, pr1_SDa, pr1_SDb};
char pok_ports_by_partition[1000];// e.g. uint8_t* pok_ports_by_partition[POK_CONFIG_NB_PARTITIONS] = {pr1_partport,pr2_partport};
char pok_ports_nb_ports_by_partition[1000];

char pok_global_ports_to_local_ports[1000];
char pok_local_ports_to_global_ports[1000];
char pok_ports_names[1000];
char pok_ports_nb_destinations[1000];
char pok_ports_kind[1000];
//char test_block[200000];

char pok_ports_message_size[20000];
char pok_ports_messages[200000];

char input_sampling_ports_names_from_file[MAX_PORTS][100];
char input_sampling_msgs_from_file[MAX_PORTS][DEFAULT_MAX_MESSAGE_SIZE * 6];
char input_sampling_msgs_size_from_file[MAX_PORTS][10];
int input_sampling_ports_in_file;


char deployment_destinations[2000];
char pok_ports_destinations[1000];

char slot_pok_outputports_to_flush[1000];
char pok_outputports_to_flush[1000];
char pok_nb_outputports_to_flush[1000];

char slot_pok_inputports_to_preload[1000];
char pok_inputports_to_preload[1000];
char pok_nb_inputports_to_preload[1000];

int slots_duration[MAX_SLOTS];
int slots_partition[MAX_SLOTS];
int postwrite_times[MAX_SLOTS];

// kernel macro
char macro_values[NUMBER_OF_KERNEL_MACROS][400];
unsigned char macro_enabled[NUMBER_OF_KERNEL_MACROS];

// partition macro
char macro_partitions_values[MAX_PARTITIONS][NUMBER_OF_PARTIONS_MACROS][100];
unsigned char macro_partitions_enabled[MAX_PARTITIONS][NUMBER_OF_PARTIONS_MACROS];

// used to quicly convert number to string
char number_to_string[100][3];



