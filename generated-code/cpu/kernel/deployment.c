#include "deployment.h"
#include <types.h>
#include <middleware/port.h>


uint8_t pr1_partport[2] = {QD_1,SS_0};
uint8_t pr2_partport[2] = {QS_2,SD_3};

uint8_t*	pok_ports_by_partition[POK_CONFIG_NB_PARTITIONS] = {pr1_partport, pr2_partport};

uint8_t		pok_ports_nb_ports_by_partition[POK_CONFIG_NB_PARTITIONS] = {2, 2};

uint8_t pok_global_ports_to_local_ports[POK_CONFIG_NB_GLOBAL_PORTS] = {
			SS_0,
			QD_1,
			QS_2,
			SD_3};

uint8_t pok_local_ports_to_global_ports[POK_CONFIG_NB_PORTS] = {
			SS_0_global,
			QD_1_global,
			QS_2_global,
			SD_3_global};

char* pok_ports_names[POK_CONFIG_NB_PORTS] = {
			"port_01",
			"port_02",
			"port_03",
			"port_04"};

// Number of DESTINATION ports for each partition
uint8_t pok_ports_nb_destinations[POK_CONFIG_NB_PORTS] = {1, 0, 1, 0};

// Configuration-time KIND attribute for each port
uint8_t		pok_ports_kind[POK_CONFIG_NB_PORTS]  = {
			POK_PORT_KIND_SAMPLING,
			POK_PORT_KIND_QUEUEING,
			POK_PORT_KIND_QUEUEING,
			POK_PORT_KIND_SAMPLING};

uint8_t SS_0_deployment_destinations [1] = {SD_3_global};
uint8_t QS_2_deployment_destinations [1] = {QD_1_global};
// Array of destination ports for each port (destination ports must be NULL)
uint8_t* 	pok_ports_destinations[POK_CONFIG_NB_PORTS] = {
			SS_0_deployment_destinations,
			NULL,
			QS_2_deployment_destinations,
			NULL};

uint8_t slot1_outputports_to_flush[1] = {SS_0};
uint8_t slot2_outputports_to_flush[1] = {QS_2};
uint8_t* pok_outputports_to_flush[POK_CONFIG_SCHEDULING_NBSLOTS] = {slot1_outputports_to_flush,slot2_outputports_to_flush};
uint8_t pok_nb_outputports_to_flush[POK_CONFIG_SCHEDULING_NBSLOTS] = {1,1};

uint8_t slot1_inputports_to_preload[1] = {QD_1};
uint8_t slot2_inputports_to_preload[1] = {SD_3};
uint8_t* pok_inputports_to_preload[POK_CONFIG_SCHEDULING_NBSLOTS] = {slot1_inputports_to_preload,slot2_inputports_to_preload};
uint8_t pok_nb_inputports_to_preload[POK_CONFIG_SCHEDULING_NBSLOTS] = {1,1};

