IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) `xml2-config --cflags` `xml2-config --libs`
GEN_DIR=../generated-code
ODIR=obj


_DEPS = Makefile.h arinc653-xml-conf.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = arinc653-xml-conf.o read_xml.o print_config.o build_config.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

code-gen: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -rf $(ODIR)/*.o $(GEN_DIR)

clean-gen-dir: 
	rm -rf $(GEN_DIR) 
	

