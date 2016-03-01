#include <arinc653/types.h>
#include "deployment.h"


// variables used by the RECEIVE calls
extern MESSAGE_ADDR_TYPE SD_3_msg;

// mapping between input ports and addresses of the variables used by the RECEIVE calls.
MESSAGE_ADDR_TYPE *receiving_addresses[POK_CONFIG_NB_SYSTEM_PORTS] = 
{NULL,NULL,NULL,&SD_3_msg};

#include <arinc653/event.h>
#include <arinc653/buffer.h>

// input buffers used by the message pre load phase (sampling ports)
unsigned char input_sampling_buffer_1[POK_CONFIG_MAX_MESSAGE_SIZE];

unsigned char *input_buffers[POK_CONFIG_PART_NB_INPUT_SAMPLING_PORTS ]= {
{input_sampling_buffer_1}};

EVENT_ID_TYPE pok_arinc653_events_ids[1];

char* pok_arinc653_events_names[POK_CONFIG_ARINC653_NB_EVENTS] = {"evt1"};
BUFFER_ID_TYPE pok_buffers_ids[1];

char* pok_buffers_names[POK_CONFIG_NB_BUFFERS] = {"buf"};
