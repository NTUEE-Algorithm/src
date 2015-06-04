AR       := ar
ARFLAGS  := cr
CXX      := g++
CPPFLAGS := -O2 -I../lib

all : ../bin/DPT_balance_color
	@echo -n ""

clean :
	rm *.o ../bin/DPT_balance_color

../bin/DPT_balance_color : main.o graph.o myString.o map.o
	$(CXX) $^ -o $@

main.o : main.cpp graph.h map.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

graph.o : graph.cpp graph.h map.h
	$(CXX) $(CPPFLAGS) -c $< -o $@
	
myString.o : myString.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

map.o : map.cpp map.h graph.h
	$(CXX) $(CPPFLAGS) -c $< -o $@   
