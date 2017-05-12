#
# stuff to make
#
ifndef ROOTSYS
all:
	@echo "ROOTSYS is not set. Please set ROOT environment properly"; echo
else

all: 	build
help:
	@echo "Available Targets:";\
	cat Makefile | perl -ne 'printf("\t%-15s %s\n",$$1,$$2) if(/^(\S+):[^#]+(#.*)$$/)'

ifndef VERBOSE
  QUIET := @
endif

#ROOFITINCLUDE = 
#ifdef CMSSW_VERSION
#	ROOFITINCLUDE = $(shell scramv1 tool info roofitcore | grep INCLUDE | sed 's/^INCLUDE=/-I/')
#endif

CC = g++
CMSROOT = ./
INCLUDE = $(shell root-config --cflags) -I$(CMSROOT) -I$(CMSROOT)/CORE
# CFLAGS = -Wall -Wno-unused-function -g -O2 -fPIC $(INCLUDE) $(EXTRACFLAGS)
CFLAGS = -Wall -Wno-unused-function -g -O0 -fPIC $(INCLUDE) $(EXTRACFLAGS)
# CFLAGS = -Wall -Wno-unused-function -g -O1 -fPIC $(INCLUDE) $(EXTRACFLAGS)
ROOTLIBS = $(shell root-config --ldflags --cflags --libs) -lTMVA #-lEG -lGenVector
COREDIR = CORE

DICTINCLUDE = $(ROOTSYS)/include/Math/QuantFuncMathCore.h $(ROOTSYS)/include/TLorentzVector.h $(ROOTSYS)/include/Math/Vector4D.h

LINKER = g++
LINKERFLAGS = $(shell root-config --ldflags --libs) -lEG -lGenVector -lTMVA

#DIR = /Users/cerati/SSAnalysis/SSAnalysis/
DIR = ./

CORESOURCES=$(DIR)/$(COREDIR)/CMS3.cc \
 $(DIR)/$(COREDIR)/Base.cc \
 $(DIR)/$(COREDIR)/ElectronSelections.cc \
 $(DIR)/$(COREDIR)/MuonSelections.cc \
 $(DIR)/$(COREDIR)/JetSelections.cc \
 $(DIR)/$(COREDIR)/MetSelections.cc \
 $(DIR)/$(COREDIR)/VertexSelections.cc \
 $(DIR)/$(COREDIR)/TriggerSelections.cc \
 $(DIR)/$(COREDIR)/MCSelections.cc \
 $(DIR)/$(COREDIR)/IsolationTools.cc \
 $(DIR)/$(COREDIR)/Tools/MT2/MT2Utility.cc \
 $(DIR)/$(COREDIR)/Tools/MT2/MT2.cc \
 $(DIR)/$(COREDIR)/Tools/utils.cc \
 $(DIR)/$(COREDIR)/Tools/JetCorrector.cc \
 $(DIR)/$(COREDIR)/Tools/goodrun.cc \
 $(DIR)/$(COREDIR)/Tools/datasetinfo/getDatasetInfo.cc \
 $(DIR)/$(COREDIR)/Tools/btagsf/BTagCalibrationStandalone.cc \
 $(DIR)/$(COREDIR)/SSSelections.cc
COREOBJECTS=$(CORESOURCES:.cc=.o)
CORELIB=libCMS3CORE.so

SOURCES = $(wildcard $(DIR)/*.cc)
OBJECTS = $(SOURCES:.cc=.o) 
LIB = liblooperBatch.so

DICT = LinkDef_out.o

LIBS = $(LIB)

EXE = main.exe

#
# how to make it
#

$(CORELIB): $(DICT) $(COREOBJECTS)
	$(QUIET) echo "Linking $@"; \
	echo "$(LINKER) -shared -o $@ $(COREOBJECTS) $(DICT) $(LINKERFLAGS)"; \
	$(LINKER) -shared -o $@ $(COREOBJECTS) $(DICT) $(LINKERFLAGS)

$(LIB):	$(DICT) $(OBJECTS) $(COREOBJECTS)
	$(QUIET) echo "Linking $@"; \
	echo "$(LINKER) -shared -o $@ $(OBJECTS) $(COREOBJECTS) $(DICT) $(LINKERFLAGS)"; \
	$(LINKER) -shared -o $@ $(OBJECTS) $(COREOBJECTS) $(DICT) $(LINKERFLAGS)

LinkDef_out.cxx: LinkDef.h
	$(QUIET) echo "Making CINT dictionaries"; \
	rootcint -f LinkDef_out.cc -c -p $(DICTINCLUDE)  LinkDef.h; \
	cat LinkDef_out.cc > LinkDef_out.cxx; rm LinkDef_out.cc

%.exe:  $(LIBS)
	$(QUIET) echo "Building $@"; \
	$(CC) -o $@ $(LIBS) $(ROOTLIBS) ${@:.exe=.cc} 

%.o: 	%.cc %.h
	$(QUIET) echo "Compiling $<"; \
	$(CC) $(CFLAGS) $< -c -o $@

%.o: 	%.cc
	$(QUIET) echo "Compiling $<"; \
	$(CC) $(CFLAGS) $< -c -o $@

%.o:    %.cxx 
	$(QUIET) echo "Compiling $<"; \
	$(CC) $(CFLAGS) $< -c -o $@

libs:	$(LIBS)

build:  $(EXE)

b: build

loopclean:
	rm -f \
	*_out.*	 \
	*.o \
	*.*~ \
	$(LIB) \
	main.exe

clean: loopclean
	rm -f \
	$(CORELIB) \
	./$(COREDIR)/*.o \
	./Tools/MT2/*.o \
	./Tools/*.o \

endif
