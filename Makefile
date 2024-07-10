CC  = g++
CXX = g++

INCLUDES = 

CFLAGS   = -std=c++11 -g -Wall $(INCLUDES)
CXXFLAGS = -std=c++11 -g -Wall $(INCLUDES)

LDFLAGS = 
LDLIBS  = 

.PHONY: default
default: hfc

hfc: hfc.o
	$(CXX) $(LDFLAGS) -o hfc hfc.o $(LDLIBS)

hfc.o: hfc.cpp
	$(CXX) $(CXXFLAGS) -c hfc.cpp -o hfc.o

.PHONY: clean
clean:
	rm -f *.o *~ a.out core hfc

.PHONY: run
run: hfc
	./hfc

.PHONY: all
all: clean default