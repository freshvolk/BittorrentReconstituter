CPP=g++
CFLAGS=-Wall -g
OBJFLAGS=-c
OSNAME := $(shell uname -s)

ifeq ($(OSNAME),Darwin)
	LIBS=-L/opt/local/lib -lboost_program_options-mt -lboost_serialization-mt -lpcap
else
	LIBS=-lboost_program_options-mt -lboost_serialization-mt -lpcap
endif

PCAPOBJECTS=./pcap_parser/PacketHandler.o ./pcap_parser/SessionFinder.o ./pcap_parser/Session.o
FILERECONSTOBJECTS=
SUBDIRS = pcap_parser file_reconstituter
.PHONY: subdirs $(SUBDIRS) clean

all: subdirs btfinder

btfinder: driver.o $(PCAPOBJECTS) $(FILERECONSTOBJECTS)
ifeq ($(OSNAME),Darwin)
	$(CPP) $(CFLAGS) -o btfinder -I/opt/local/include $(LIBS) $(PCAPOBJECTS) $(FILERECONSTOBJECTS)
else
	$(CPP) $(CFLAGS) -o btfinder $(LIBS) $(PCAPOBJECTS) $(FILERECONSTOBJECTS)
endif

driver.o: driver.cpp

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

# The rest of these targets are for convenience

tags:
	ctags `find . -iname '*.[c,h]pp'`

goodtags:
	etags $(find . -name "*.c" -o -name "*.cpp" -o -name "*.h")

apidocs:
	doxygen

clean:
	find . -iname '*.o' -print0 | xargs -0 rm -f
	rm -f btfinder