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
      tile-hist-x-zsplit.o

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
bin/make_plots: bin/tile-detector-plots.o bin/tile-hist-x.o
bin/make_plots: bin/tile-hist.o bin/tile-hist-z.o
bin/make_plots: bin/tile-hist-ring.o bin/tile-hist-x-zsplit.o
bin/make_plots: bin/tile-hist-radius.o bin/tile-hist-energy.o
bin/make_plots: bin/tile-detector-uniform.o bin/tile-detector-sectioned.o 

bin/tile-hist-x.o: bin/tile-hist.o
bin/tile-hist-x-zsplit.o: bin/tile-hist.o
bin/tile-hist-z.o: bin/tile-hist.o
bin/tile-hist-ring.o: bin/tile-hist.o
bin/tile-hist-radius.o: bin/tile-hist.o
bin/tile-hist-energy.o: bin/tile-hist.o
bin/tile-detector.o: bin/tile-hist.o
bin/tile-detector-uniform.o: bin/tile-detector.o
bin/tile-detector-sectioned.o: bin/tile-detector.o
