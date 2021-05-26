CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -Werror -std=c++2a -O0 -g
LDFLAGS=$(CXXFLAGS)
OBJ=$(SRC:.cc=.o)
all:  tsp

tsp: tsp.o deme.o tournament_deme.o chromosome.o climb_chromosome.o cities.o
#tsp: tsp.o  tournament_deme.o climb_chromosome.o cities.o
	$(CXX) $(LDFLAGS) -o $@ $^

tournament_deme: tournament_deme.o deme.o chromosome.o climb_chromosome.o cities.o
	$(CXX) $(LDFLAGS) -o $@ $^

deme: deme.o chromosome.o climb_chromosome.o cities.o
	$(CXX) $(LDFLAGS) -o $@ $^

climb_chromosome: climb_chromosome.o chromosome.o cities.o
	$(CXX) $(LDFLAGS) -o $@ $^

chromosome: chromosome.o cities.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf *.o tsp
