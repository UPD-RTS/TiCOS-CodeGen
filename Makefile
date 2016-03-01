SDIR=src
IDIR=$(SDIR)/include
CC=gcc
CFLAGS=-I$(IDIR) `xml2-config --cflags` `xml2-config --libs`
GEN_DIR=generated-code
ODIR=obj
MKDIR=mkdir -p


_DEPS = Makefile.h arinc653-xml-conf.h read_utils.h build_utils.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJS = arinc653-xml-conf.o read_xml.o print_config.o build_config.o  read_utils.o build_utils.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS) | $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

code-gen: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

$(ODIR):
	$(MKDIR) $(ODIR)

.PHONY: clean

clean:
	rm -rf $(ODIR)/*

clean-gen-dir:
	rm -rf $(GEN_DIR) 
