/*
    but : effectuer un benchmark , c'est a dire execute une commande _nombreDeTour fois

*/
#ifndef __BENCHMARK_HPP
#define __BENCHMARK_HPP

#include<FileIO.hpp>
#include<string>

class Benchmark
{
private:
    FileIO& _file;
    std::string _progName;
    int _nombreDeTour;
    int _benchNb;
    int _nbCores;
    struct traveler dryRun(int nbSeconds); // retourne les infos de temps et énèrgie pour nbMinutes minutes (sert pour le calibrague, 5 min par défaux) 
public:
    Benchmark(FileIO& file, std::string progname, int number, int benchNb, int nbCores);
    
    /**
     * Launch the benchmark.
     */
    void start();
    
    ~Benchmark();
};

#endif
