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
#include <stdio.h>

#include "arinc653-xml-conf.h"

extern int read_xml();
extern int build_config();
extern int print_config();
extern int is_valid();

/*
This program:

- read the parameters of the application example from the xml file
- build the configuration of the application example
- print all files of the application example in the generated-code dir

*/

int main(int argc, char **argv) {

	char *filename = DEFAULT_FILENAME;
	char *schema_filename = DEFAULT_SCHEMA;
	
	
	if (argc == 2){
		filename = argv[1];
	}else if (argc == 3){
		filename = argv[1];
		schema_filename = argv[2];
	}else if ( argc > 3 ){
		printf("ERROR: wrong number of paramenter\n");
		return -1;
	}
	
	debug_printf ("Validating the xml file...");
	// validate the xml file
	if ( is_valid(filename,schema_filename) ) {
		return(-1);
    }
	debug_printf ("OK\n");


	// read the xml file
	if ( read_xml() ){
		return(-1);
    }
    
	// build the configuration 
	if ( build_configuration() ){
		return(-1);
    }
    
	// print the configuration
	if ( print_config() ){
		return(-1);
    }

	// Shutdown libxml
	xmlCleanupParser();
  
}
