/* but: cpp de Master.hpp
 */

#include<string>
#include<iostream>
#include<chrono>
#include<ctime>
#include<Master.hpp>
#include<Benchmark.hpp>

Master::Master(const std::string& pathConfigFile, const std::string& pathGeneralOutput, const std::string& pathDirectoryBenchFiles, int nbCpuCore): _nbCpuCore(nbCpuCore), _fileModule(pathConfigFile, pathGeneralOutput, pathDirectoryBenchFiles), _nbBench(0), _benchTurns(0){
    _nbBench = _fileModule.getNbBench();
    _benchTurns = _fileModule.getBenchTurns();
}

void Master::start(){
    std::string command;
    std::chrono::system_clock::time_point clockPoint;
    time_t time;
    char timeBuff[100];
    for(int i=0;i<_nbBench;++i){
        std::cout << "Reading next bench (" << i << ") from conf file." << std::endl;
        command = _fileModule.readNextCommand();
        clockPoint = std::chrono::system_clock::now();
        time = std::chrono::system_clock::to_time_t(clockPoint);
        std::strftime(timeBuff, sizeof(timeBuff), "%c", std::localtime(&time));
        std::cout << timeBuff << std::endl;
        std::cout << "Starting bench number: " << i << " for command: " << command << std::endl;
        Benchmark currentBench(_fileModule, command, _benchTurns, i, _nbCpuCore);
        currentBench.start();
    }
}
