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

#define	GENERATEDCODE_MAKEFILE	"all:\n\t$(MAKE) -C cpu all\n\nclean:\n\t$(MAKE) -C cpu clean\n\nrun:\n\t$(MAKE) -C cpu run&"

#define CPU_MAKEFILE_FIRSTPART	"export ARCH=ppc\n\
export BSP=prep\n\
export POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE=1\n\
include $(POK_PATH)/misc/mk/config.mk\n\
include $(POK_PATH)/misc/mk/common-$(ARCH).mk\n\
TARGET=$(shell pwd)/pok.elf\n\
PARTITIONS= "

#define CPU_MAKEFILE_SECONDPART	"HEADERFILE=kernel/deployment.h\n\
$(if $(wildcard $(HEADERFILE)),,$(error \"kernel/deployment.h is missing, bye bye.\"))\n\
#export DEF_VADDR=`grep -i \"POK_CONFIG_DEFINE_BASE_VADDR\" $(HEADERFILE)`\n\
export DEF_VADDR=$(shell grep -i POK_CONFIG_DEFINE_BASE_VADDR $(HEADERFILE)|grep -v \"/\")\n\
export PARTITIONS_VADDR=`awk '{if ($$2 == \"POK_CONFIG_PARTITIONS_BASE_VADDR\") print $$3}' $(HEADERFILE)|sed 's/{//'|sed 's/}//'|sed 's/,/ /g'`\n\
\n\
export SKIP_LOADER=$(shell grep -i POK_SKIP_LOADER $(HEADERFILE)|grep -v \"/\")\n\
export PARTITIONS_ADDR=`awk '{if ($$2 == \"POK_CONFIG_PARTITIONS_LOADADDR\") print $$3}' $(HEADERFILE)|sed 's/{//'|sed 's/}//'|sed 's/,/ /g'`\n\
\n\
KERNEL=kernel/kernel.lo\n\
all: build-lds build-kernel partitions $(TARGET)\n\
\n\
build-lds: kernel-lds	partitions-lds\n\
\n\
kernel-lds:  \n\
# Handle kernel linker script\n\
ifeq ($(shell grep -i POK_NEEDS_QEMU_SETUP $(HEADERFILE)|grep -v \"/\"),)\n\
	$(ECHO) \"PRAORTIS SIM setup $(ARCH)\"\n\
	cp $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/PROARTIS_sim/kernel_template.lds $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_template.lds \n\
	cp $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/PROARTIS_sim/kernel_postscript.lds $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_postscript.lds \n\
	sed s/placeholder/0xFC000000/ $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_template.lds > $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel.lds \n\
else\n\
	$(ECHO) \"QEMU setup\" \n\
	cp $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/QEMU/kernel_template.lds $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_template.lds \n\
	cp $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/QEMU/kernel_postscript.lds $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_postscript.lds \n\
	sed s/placeholder/0xFFF00000/ $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_template.lds > $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel.lds \n\
endif\n\
\
ifeq ($(shell grep -i POK_SKIP_LOADER $(HEADERFILE)|grep -v \"/\"),)\n\
	$(ECHO) \"Using DEFAULT Base physical addresses\"\n\
#	cp $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_template.lds $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_t.lds \n\
#	sed s/placeholder/0xFC000000/ $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_t.lds > $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel.lds\n\
	cp $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel.lds ./kernel/kernel.lds ; \\\n\
#	rm -f $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_t.lds\n\
else\n\
	$(ECHO) \"Base physical addresses\"\n\
	cp $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_preamble.lds $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_t.lds\n\
	c=1;for v in $(PARTITIONS_ADDR); do \\\n\
$(ECHO) \"Base address\" $$v \"-\" $$c \": in kernel.lds\" ; \\\n\
		$(ECHO) \"   . = $$v; \" >> $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_t.lds; \\\n\
		$(ECHO) \"   .part$$c :  \" >> $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_t.lds; \\\n\
		$(ECHO) \"   { \" >> $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_t.lds; \\\n\
		$(ECHO) \"       part$$c.elf(.text) \" >> $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_t.lds; \\\n\
		$(ECHO) \"   } \" >> $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_t.lds; \\\n\
		$(ECHO) \" \" >> $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_t.lds; \\\n\
		c=`$(ECHO) $${c}+1 | bc` ; \\\n\
	done\n\
	cat $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_t.lds $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_postscript.lds > $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel.lds ; \\\n\
  cp $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel.lds ./kernel/kernel.lds ; \\\n\
	rm -f $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel_t.lds \n\
endif\n\
\
partitions-lds:\n\
# Handle partitions linker scripts\n\
ifeq ($(shell grep -i POK_SKIP_LOADER $(HEADERFILE)|grep -v \"/\"),)\n\
	$(eval PARTITION_LDS=partition_template.lds)\n\
else\n\
	$(eval PARTITION_LDS=partition_no_loader.lds)\n\
	$(ECHO) \"Skipping POK loader\"\n\
endif\n\
#ifeq ($(shell grep -r \"POK_CONFIG_DEFINE_BASE_VADDR\" $(HEADERFILE)),)\n\
ifeq ($(shell grep -i POK_CONFIG_DEFINE_BASE_VADDR $(HEADERFILE)|grep -v \"/\"),)\n\
	$(ECHO) \"Using DEFAULT Base virtual addresses\"\n\
	c=1;for v in $(PARTITIONS); do \\\n\
		$(ECHO) \"Base vaddress 0x1000 -\" $$c \": in ./part\"$$c\"/partition_\"$$c\".lds\" ; \\\n\
		cp $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/$(PARTITION_LDS) $(eval) ./part$$c/partition_$$c_t.lds ; \\\n\
		sed s/placeholder/0x1000/ ./part$$c/partition_$$c_t.lds > ./part$$c/partition_$$c.lds;\\\n\
		rm -f ./part$$c/partition_t.lds ; \\\n\
    c=`$(ECHO) $${c}+1 | bc` ; \\\n\
	done\n\
else\n\
	c=1;for v in $(PARTITIONS_VADDR); do \\\n\
		$(ECHO) \"Base vaddress\" $$v \"-\" $$c \": in ./part\"$$c\"/partition_\"$$c\".lds\" ; \\\n\
		cp $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/$(PARTITION_LDS) $(eval) ./part$$c/partition_$$c_t.lds ; \\\n\
		sed s/placeholder/$$v/ ./part$$c/partition_$$c_t.lds > ./part$$c/partition_$$c.lds;\\\n\
		rm -f ./part$$c/partition_.lds ; \\\n\
    c=`$(ECHO) $${c}+1 | bc` ; \\\n\
	done\n\
endif\n\
\n\
build-kernel:\n\
	$(CD) kernel && $(MAKE)\n\npartitions:\n"


#define CPU_MAKEFILE_THIRDPART	"include $(POK_PATH)/misc/mk/rules-common.mk\n\
include $(POK_PATH)/misc/mk/rules-main.mk\n\
include $(POK_PATH)/misc/mk/install-rules.mk\n\
\n\
INSTRUMENTATION_PATH=$(POK_PATH)/kernel/core\n\
#kernel/core\n\
#libpok/arinc653\n\
INSTRUMENTATION_FILE=sched\n\
#sampling\n\
CINS=/usr/local/bin/cins\n\
XSTUTILS=/usr/local/bin/xstutils\n\
ROOT_FUNCTION=pok_elect_partition\n\
#pok_elect_partition\n\
#READ_SAMPLING_MESSAGE\n\
#WRITE_SAMPLING_MESSAGE\n\
\n\
instrument:\n\
	$(ECHO) Preprocessing...\n\
	$(RM) -rf $(POK_PATH)/instrumentation/instr\n\
	$(RM) -rf $(POK_PATH)/instrumentation/xsc\n\
	$(RM) -rf $(POK_PATH)/instrumentation/*.exf\n\
	$(RM) -rf $(POK_PATH)/instrumentation/*.p\n\
	$(MKDIR) $(POK_PATH)/instrumentation/instr\n\
	$(MKDIR) $(POK_PATH)/instrumentation/xsc\n\
	#if [ -f $(INSTRUMENTATION_PATH)/$(INSTRUMENTATION_FILE)_ORIGINAL.c ]; then $(MV) -f $(INSTRUMENTATION_PATH)/$(INSTRUMENTATION_FILE)_ORIGINAL.c $(INSTRUMENTATION_PATH)/$(INSTRUMENTATION_FILE).c; fi\n\
	$(CC_ppc) -E -ansi $(INSTRUMENTATION_PATH)/$(INSTRUMENTATION_FILE).c -o $(POK_PATH)/instrumentation/$(INSTRUMENTATION_FILE).p -I$(POK_PATH)/kernel/include -I$(POK_PATH)/instrumentation -I$(POK_PATH)/libpok/include -include ./kernel/deployment.h\n\
	$(ECHO) Done\n\
	$(CINS) --cext GNU -v -c $(POK_PATH)/instrumentation/rvs_instrumentation.h $(POK_PATH)/instrumentation/$(INSTRUMENTATION_FILE).p -d $(POK_PATH)/instrumentation/instr/ --xsc-dir $(POK_PATH)/instrumentation/xsc/ --exf $(POK_PATH)/instrumentation/app.exf --nbits 12 -u -D --discover-fptr\n\
	$(XSTUTILS) $(POK_PATH)/instrumentation/xsc/*.xsc -r $(ROOT_FUNCTION) -o $(POK_PATH)/instrumentation/xsc/$(ROOT_FUNCTION).rvd -v --debug-report\n\
	$(RM) -rf ./rvs_dbgreport_files\n\
	$(RM) -rf ./rvs_dbgreport.txt\n\
	$(CD) $(POK_PATH)/instrumentation/instr && for f in *.p; do mv $$f `echo $$f | sed 's/\\(.*\\.\\)p/\\1c/'`; done\n\
	#$(MV) -f $(INSTRUMENTATION_PATH)/$(INSTRUMENTATION_FILE).c $(INSTRUMENTATION_PATH)/$(INSTRUMENTATION_FILE)_ORIGINAL.c\n\
	#$(CP) $(POK_PATH)/instrumentation/instr/$(INSTRUMENTATION_FILE).c $(INSTRUMENTATION_PATH)/$(INSTRUMENTATION_FILE).c\n\
	#$(CC_ppc) -c $(POK_PATH)/instrumentation/rvs.c -o $(POK_PATH)/instrumentation/rvs.o\n\
	#$(CC_ppc) -c -I/home/proartis/proartis/POK/instrumentation /home/proartis/proartis/POK/instrumentation/rapita.cpp -o /home/proartis/proartis/POK/instrumentation/rapita.o\n\
	#$(MAKE)"

#define KERNEL_MAKEFILE "export DEPLOYMENT_HEADER=$(shell pwd)/deployment.h\n\
include $(POK_PATH)/misc/mk/config.mk\n\
LO_TARGET = kernel.lo\n\
LO_OBJS = deployment.o\n\
LO_DEPS = pok.lo\n\
all: kernel copy-kernel $(LO_TARGET)\n\
clean: common-clean\n\
include $(POK_PATH)/misc/mk/common-$(ARCH).mk\n\
include $(POK_PATH)/misc/mk/rules-common.mk\n\
include $(POK_PATH)/misc/mk/rules-kernel.mk"

#define PART_MAKEFILE_FIRSTPART "export DEPLOYMENT_HEADER=$(shell pwd)/deployment.h\n\
include $(POK_PATH)/misc/mk/config.mk\n\
TARGET = "

#define PART_MAKEFILE_SECONDPART "all: libpok $(TARGET)\n\
clean: common-clean\n\
include $(POK_PATH)/misc/mk/common-$(ARCH).mk\n\
include $(POK_PATH)/misc/mk/rules-partition.mk\n\
include $(POK_PATH)/misc/mk/rules-common.mk"
