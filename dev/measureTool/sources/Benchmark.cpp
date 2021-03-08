#include"Benchmark.hpp"    
#include"FileIO.hpp"
#include"Listener.hpp"    
#include<iostream>
#include<ctime>
#include<chrono>
#include<string>
#include<cmath>
#include<thread>
#include<vector>
#include<unistd.h> //pour le fork
#include<sys/wait.h> // pour waitpid
#include<sstream>
#include<string.h>
#include<cstdlib>
#include<iostream>
#include<errno.h>


Benchmark::Benchmark(FileIO& file, std::string progname, int number, int benchNb, int nbCores) : _file(file), _progName(progname), _nombreDeTour(number), _benchNb(benchNb), _nbCores(nbCores){}

struct traveler Benchmark::dryRun(int nbSeconds){
    struct traveler messanger;
    Listener listener(messanger);
    Listener::launch(listener, 0);
    std::this_thread::sleep_for(std::chrono::seconds(nbSeconds));
    Listener::stop(listener);
    return messanger;
}

void Benchmark::start(){    
    time_t now = time(0);
    std::string dateTime = ctime(&now);;
    double moyConsRT=0,ecTypeConsRT=0,moyRealTime=0,ecTypeRealTime=0,moyConsCpu=0,ecTypeConsCpu=0,moyCpuTime=0,ecTypeCpuTime=0;
    struct BenchInfoFinal finalPacket;

    for (int i = 0; i < _nombreDeTour; i++)
    {   

        struct traveler dryData = dryRun(5*60); // donnee lors d'un benchmark à vide de 5min (5*60)

        struct BenchInfoWork benchPacket;
        pid_t child;
        std::vector<std::string> argsV;
        char **args;
        std::string inter;

        //construction de la liste d'args pour le execv
        std::istringstream entre(_progName);
        while (std::getline(entre, inter, ' ')){
            argsV.push_back(inter);
        }
        args = new char *[argsV.size()+1];
        for(int j=0;j<argsV.size();++j){
            args[j] = new char[argsV[j].size()+1];
            strcpy(args[j], argsV[j].c_str());
        }
        args[argsV.size()] = nullptr;
        
        //suite
        struct traveler bench;
        Listener listener(bench);

        //std::system(_progName.c_str()); // appel system
        child = fork();
        if (child == -1){
            throw std::runtime_error("Benchmark::start() : couldn't fork the process");
        }
        
        if(child == 0){ // child process
            int execution = execv(args[0], args);
            std::cout << args[0] << " ERROR "<< execution << " ERNO " << strerror(errno) << std::endl;
            std::exit(EXIT_FAILURE);
        }
        Listener::launch(listener, child);
        
        waitpid(child, nullptr, 0);
        
        Listener::stop(listener);
        if(listener.isError()){
            throw std::runtime_error(listener.getErrorMessage());
        }
        double cpuLoadInter = bench.cpuLoad;
        if(cpuLoadInter > _nbCores){
            cpuLoadInter = _nbCores;
        }
        double consommationCPU = ((bench.energy)/(bench.elapsedRealTime.count()*cpuLoadInter))*bench.elapsedCpuTime;
        double tempsReel = bench.elapsedRealTime.count(); 
        double tempsCpu = bench.elapsedCpuTime; 

        double consommationRT = bench.energy - (dryData.energy/dryData.elapsedRealTime.count()) * tempsReel;
        
        moyConsRT+=(consommationRT/_nombreDeTour);
        moyConsCpu+=(consommationCPU/_nombreDeTour);
        moyRealTime+=(tempsReel/_nombreDeTour);
        moyCpuTime+=(tempsCpu/_nombreDeTour);

        ecTypeConsRT += consommationRT*consommationRT;
        ecTypeConsCpu += consommationCPU*consommationCPU;
        ecTypeRealTime += tempsReel*tempsReel;
        ecTypeCpuTime += tempsCpu*tempsCpu;

        // initialisation du packet de chaque bench
        benchPacket.benchNb = _benchNb;
        benchPacket.benchTurn = i;
        finalPacket.date = dateTime;
        benchPacket.date =  dateTime; //ctime(&debut);
        benchPacket.cmd = _progName;

        benchPacket.consumptionRT = consommationRT;
        benchPacket.realTime = tempsReel;

        benchPacket.consumptionCPU = consommationCPU; 
        benchPacket.cpuTime = tempsCpu; 

        _file.saveBenchInfo(&benchPacket);
        for(int j=0;j<argsV.size();++j){
            delete args[j];
        }
        delete args;
    }

    ecTypeConsRT = sqrt(ecTypeConsRT/_nombreDeTour - (moyConsRT*moyConsRT)); 
    ecTypeConsCpu = sqrt(ecTypeConsCpu/_nombreDeTour - (moyConsCpu*moyConsCpu)); 
    ecTypeRealTime = sqrt(ecTypeRealTime/_nombreDeTour - (moyRealTime*moyRealTime));
    ecTypeCpuTime = sqrt(ecTypeCpuTime/_nombreDeTour - (moyCpuTime*moyCpuTime));

    // initialisation du packet final 
    finalPacket.benchNb = _benchNb;
    finalPacket.date = dateTime;
    finalPacket.benchTurn = _nombreDeTour ;
    finalPacket.cmd = _progName;

    finalPacket.avgRealTime = moyRealTime;
    finalPacket.avgConsumptionRT = moyConsRT;

    finalPacket.avgCpuTime = moyCpuTime ;
    finalPacket.avgConsumptionCPU = moyConsCpu;

    finalPacket.sdConsumptionRT = ecTypeConsRT;
    finalPacket.sdRealTime = ecTypeRealTime;

    finalPacket.sdConsumptionCPU = ecTypeConsCpu;
    finalPacket.sdCpuTime = ecTypeCpuTime;
    
    _file.wrapUpBench(&finalPacket);
}


Benchmark::~Benchmark(){}
