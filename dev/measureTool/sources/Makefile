CPPFLAG = -std=c++17 #-std=c++17 -masm=intel -fconcepts -mlong-double-128 -ggdb3 -Wpedantic -Wall -Wextra -Wconversion -Wsign-conversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wsign-promo -Wzero-as-null-pointer-constant -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -lquadmath -Werror
LIBS =  -L ../libs/sharedObj -l pthread -l raplcap-powercap -l powercap
CPPFLAGS = $(CPPFLAG) $(LIBS)
HEADERS =  -I ../headers/ -I ../libs/headers/raplcap -I ../libs/headers/powercap

all: measureTool

measureTool: measureTool.o Benchmark.o Listener.o Master.o FileIO.o
	g++ -o $@ $^ $(CPPFLAGS)

measureTool.o: measureTool.cpp
	g++ -o $@ $(HEADERS) -c $< $(CPPFLAGS)

Benchmark.o: Benchmark.cpp
	g++ -o $@ $(HEADERS) -c $< $(CPPFLAGS)

FileIO.o: FileIO.cpp
	g++ -o $@ $(HEADERS) -c $< $(CPPFLAGS)

Listener.o: Listener.cpp
	g++ -o $@ $(HEADERS) -c $< $(CPPFLAGS)

Master.o: Master.cpp
	g++ -o $@ $(HEADERS) -c $< $(CPPFLAGS)
