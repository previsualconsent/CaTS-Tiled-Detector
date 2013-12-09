EXEC= make_plots
OBJS= tile-detector-plots.o \
      tile-detector.o \
      tile-detector-uniform.o \
      tile-detector-sectioned.o \
      tile-hist.o \
      tile-hist-x.o \
      tile-hist-z.o \
      tile-hist-ring.o \
      tile-hist-radius.o \
      tile-hist-energy.o \
      tile-hist-x-zsplit.o \
      energy-graph.o \
      energy-graph-res.o \
      energy-graph-z-template.o

TARGETS= $(addprefix bin/,$(EXEC))
SOURCES=$(patsubst %,src/%.cc,$(EXEC))

OBJECTFILES=$(addprefix bin/,$(OBJS))

CFLAGS= -g -Wall `root-config --cflags` `geant4-config --cflags` -I../CaTS/include -Iinclude/

LDFLAGS = `root-config --glibs` `geant4-config --libs` -lCintex -L../CaTS-build -lClassesDict

all: $(TARGETS) 

$(OBJECTFILES): bin/%.o : src/%.cc include/%.h
	g++ -c -o $@ $(CFLAGS) $<

$(TARGETS): bin/% : src/%.cc $(OBJECTFILES)
	g++ -o $@ $(CFLAGS) $(LDFLAGS) $< $(OBJECTFILES)

depend: 
	makedepend -Y -f Makefile $(addprefix src/,$(OBJS:.o=.cc))  $(SOURCES) -I../CaTS/include -Iinclude/ 2> /dev/null

clean:
	rm -f $(TARGETS) 

#Special depends
bin/make_plots: include/tile-detector-plots.h include/tile-hist-x.h
bin/make_plots: include/tile-hist.h include/tile-hist-z.h
bin/make_plots: include/tile-hist-ring.h include/tile-hist-x-zsplit.h
bin/make_plots: include/tile-hist-radius.h include/tile-hist-energy.h
bin/make_plots: include/tile-detector-uniform.h include/tile-detector-sectioned.h 

bin/tile-hist-x.o: include/tile-hist.h
bin/tile-hist-x-zsplit.o: include/tile-hist.h
bin/tile-hist-z.o: include/tile-hist.h
bin/tile-hist-ring.o: include/tile-hist.h
bin/tile-hist-radius.o: include/tile-hist.h
bin/tile-hist-energy.o: include/tile-hist.h
bin/tile-detector.o: include/tile-hist.h
bin/tile-detector-plots.o: include/energy-graph-res.h
bin/tile-detector-plots.o: include/energy-graph-z-template.h
bin/tile-detector-plots.o: include/tile-hist-z.h include/tile-hist-radius.h
bin/tile-detector-plots.o: include/tile-hist-energy.h
bin/tile-detector-uniform.o: include/tile-detector.h
bin/tile-detector-sectioned.o: include/tile-detector.h
