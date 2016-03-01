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


void write_sampling_message(int partition_id, struct port_list_item_t *tmp_output_ports);

void send_queuing_message(int partition_id, struct port_list_item_t *tmp_output_ports);

void read_sampling_message(int partition_id, struct port_list_item_t *tmp_output_ports);

void receive_queuing_message(int partition_id, struct port_list_item_t *tmp_output_ports);

void init_normal_thread(int partition_id, int thread_nb);

void while_in_thread(int partition_id, int thread_nb);

int build_activity_periodic_thread(int partition_id, int thread_number);

void init_kernel_macros();

void init_source_ports_variables (int partition_id, struct port_list_item_t *tmp_output_ports);

int build_partition_related_macros_in_kernel_deployment_h();

int build_slots_config();

int build_activity_thread_normal(int partition_id, int thread_number);

int build_activity_thread_event_set(int partition_id,  thread_t thread);

int build_activity_thread_event_wait(int partition_id, int thread_number);

int build_activity_thread_event_resume(int partition_id, int thread_number);

int build_activity_thread_event_reset(int partition_id, thread_t thread);

int build_activity_thread_blackboard_display(int partition_id, int thread_number);

int build_activity_thread_blackboard_read(int partition_id, int thread_number);

int build_activity_thread_buffer_send(int partition_id, int thread_number);

int build_activity_thread_buffer_receive(int partition_id, int thread_number);

