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

int get_number_of_threads_elem(char *base_expression, int partition_id);

int get_scheduler_elem(char *base_expression, int partition_id, xmlXPathObjectPtr xpathObj);

void init_config();

int getNode(const xmlChar * xpathExpr, xmlXPathObjectPtr *xpathObj_param);

int get_threads_elem(char *base_expression, int partition_id);

int get_size_elem(char *base_expression, int partition_id);

int get_loadaddr_elem(char *base_expression, int partition_id);

int get_slots_elem(char *base_expression, int partition_id);

int get_asynch_events_elem(char *base_expression, int partition_id);

int get_events_elem(char *base_expression, int partition_id);

int get_blackboards_elem(char *base_expression, int partition_id);

int get_buffers_elem(char *base_expression, int partition_id);

int check_port_name(char *name);
