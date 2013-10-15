EXEC= ecal_gas_profile
OBJS= tile-detector.o \
      tile-hist.o \
      tile-hist-x.o \
      tile-hist-z.o

TARGETS= $(addprefix bin/,$(EXEC))
SOURCES=$(patsubst %,src/%.cc,$(EXEC))

OBJECTFILES=$(addprefix bin/,$(OBJS))

CFLAGS= -Wall `root-config --cflags` `geant4-config --cflags` -I../CaTS/include -Iinclude/

LDFLAGS = `root-config --glibs` `geant4-config --libs` -lCintex -L../CaTS-build -lClassesDict

all: $(TARGETS) $(OBJECTFILES)

$(OBJECTFILES): bin/%.o : src/%.cc include/%.h
	g++ -c -o $@ $(CFLAGS) $<

$(TARGETS): bin/% : src/%.cc
	g++ -o $@ $(CFLAGS) $(LDFLAGS) $< $(OBJECTFILES)

depend: 
	makedepend -Y -f Makefile $(addprefix src/,$(OBJS:.o=.cc))  $(SOURCES) -I../CaTS/include -Iinclude/ 2> /dev/null

clean:
	rm -f $(TARGETS) 

#Special depends
bin/ecal_gas_profile: bin/tile-detector.o bin/tile-hist-x.o
bin/ecal_gas_profile: bin/tile-hist.o bin/tile-hist-z.o
bin/tile-hist-x.o: bin/tile-hist.o
bin/tile-hist-z.o: bin/tile-hist.o
