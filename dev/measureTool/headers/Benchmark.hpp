/*
    but : effectuer un benchmark , c'est à dire execute une commande _nombreDeTour fois

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
    struct traveler dryRun(int nbSeconds); // retourne les infos de temps et énergie pour nbMinutes minutes (sert pour le calibrage, 5 min par défaut) 
public:
    Benchmark(FileIO& file, std::string progname, int number, int benchNb, int nbCores);
    
    /**
     * Launch the benchmark.
     */
    void start();
    
    ~Benchmark();
};

#endif
