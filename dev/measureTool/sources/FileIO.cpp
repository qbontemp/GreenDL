/* but: cpp de FileIO.hpp
 */

#include<iostream>
#include<fstream>

#include<string>
#include<cstdlib>
#include<stdexcept>
#include <algorithm>

#include<queue>

#include<FileIO.hpp>

// TODO oublie pas les exceptions, on code propre ici. :)

void FileIO::initConfigFile()
{
    _configFile.open(_configFilePath);
    std::string line;
            
    if(_configFile.is_open())
    {
        // simple line count
        while(getline(_configFile, line))
        {
            if (! line.empty())
                _commands.push(line);
        }
        _configFile.close();

        _nbBenchTurns = std::stoi(_commands.front());
        _commands.pop();
        _nbBench = _commands.size();
    }
    else
    {
        throw std::runtime_error("FileIO::openConfigFile() : config file doesn't exist");
    }
}

FileIO::FileIO(const std::string& configFilePath, const std::string& generalOutputPath, const std::string& benchDirectoryPath)
{
    _configFilePath = configFilePath;
    _generalOutputPath = generalOutputPath;
    _benchDirectoryPath = benchDirectoryPath;
    initConfigFile();
}


void FileIO::saveBenchInfo(struct BenchInfoWork* bInfo)
{
    // -- STRING FORMATING -- //
    bInfo->date.erase(std::remove(bInfo->date.begin(), bInfo->date.end(), '\n'), bInfo->date.end()); // deleting useless '\n' from the date


    // --  INIT -- //
    std::fstream outputFile;
    //std::string outputBenchFileName = bInfo->cmd + "__" + bInfo->date + ".txt";
    std::string outputBenchFileName = bInfo->date + "_NB" + std::to_string(bInfo->benchNb) + ".csv";
    std::replace(outputBenchFileName.begin(), outputBenchFileName.end(), '/', '-'); // replacing '/' by '-'
    std::replace(outputBenchFileName.begin(), outputBenchFileName.end(), ' ', '_'); // replacing ' ' by '-'
    //std::string outputBenchPath = _benchDirectoryPath + "/" + outputBenchFileName;

    std::string outputBenchPath; // = _benchDirectoryPath + "/" + outputBenchFileName;
    if (_benchDirectoryPath.back() == '/')
        outputBenchPath = _benchDirectoryPath + outputBenchFileName;
    else
        outputBenchPath = _benchDirectoryPath + "/" + outputBenchFileName;


    // -- FILE EXISTANCE -- //
    std::ifstream fileCheck(outputBenchPath);
    bool exists = fileCheck.good();


    // -- BENCH FILE -- //
    outputFile.open(outputBenchPath, std::fstream::in | std::fstream::out | std::fstream::app);

    if (! exists && outputFile.is_open())
    {
        // DATE (DD_MM_YYYY) and TIME + executed CMD
        outputFile << bInfo->date + "," + bInfo->cmd << std::endl; 

        // EMPTY LINE
        outputFile << std::endl; 

        // OUTPUT FORMAT DESCRIPTION
        outputFile << std::string("benchTurn") + std::string(",") + std::string("consumptionRT") + std::string(",") + std::string("realTime") + std::string(",") + std::string("consumptioncPU") + std::string(",") + std::string("cpuTime") << std::endl;
    }

    if (outputFile.is_open())
    {
        // CONSUMPTION | REAL_TIME
        outputFile << std::to_string(bInfo->benchTurn) + std::string(",") + std::to_string(bInfo->consumptionRT) + std::string(",") + std::to_string(bInfo->realTime) + std::string(",") + std::to_string(bInfo->consumptionCPU) + std::string(",") + std::to_string(bInfo->cpuTime) << std::endl;
        //outputFile << std::string("epoch: ") + std::to_string(bInfo->epoch) + std::string(", consumptionRT: ") + std::to_string(bInfo->consumptionRT) + std::string(", consumptionCPU: ") + std::to_string(bInfo->consumptionCPU) + std::string(", realTime: ") +std::to_string(bInfo->realTime) + std::string(", cpuTime: ") +std::to_string(bInfo->cpuTime) << std::endl;
        outputFile.close();
    }  
    else
    {
        throw std::runtime_error("FileIO::saveBenchInfo() : couldn't open file 1");
    }
}


void FileIO::wrapUpBench(struct BenchInfoFinal* bInfo)
{
    // -- STRING FORMATING -- //
    bInfo->date.erase(std::remove(bInfo->date.begin(), bInfo->date.end(), '\n'), bInfo->date.end());


    // -- INIT -- //
    std::fstream outputFile;
    std::string outputSummaryPath = _generalOutputPath;
    //std::string outputBenchFileName = bInfo->cmd + "__" + bInfo->date + ".txt";
    std::string outputBenchFileName = bInfo->date + "_NB" + std::to_string(bInfo->benchNb) + ".csv";
    std::replace(outputBenchFileName.begin(), outputBenchFileName.end(), '/', '-'); // replacing '/' by '-'
    std::replace(outputBenchFileName.begin(), outputBenchFileName.end(), ' ', '_'); // replacing ' ' by '-'
    
    std::string outputBenchPath;
    if (outputBenchPath.back() == '/')
        outputBenchPath = _benchDirectoryPath + outputBenchFileName;
    else
        outputBenchPath = _benchDirectoryPath + "/" + outputBenchFileName;


    // -- SUMMARY FILE EXISTANCE -- //
    std::ifstream fileCheck(outputSummaryPath);
    bool exists = fileCheck.good();

    outputFile.open(outputSummaryPath, std::fstream::in | std::fstream::out | std::fstream::app);

    // -- SUMMARY FILE -- //
    if (! exists && outputFile.is_open())
    {
        // Si fichiers n'existait pas, on va imprimer un guide visuel des résultats à la 1ère ligne
        outputFile << std::string("benchTurns") + std::string(",") + std::string("avgConsumptionRT") + std::string(",") + std::string("sdConsumptionRT") + std::string(",") + std::string("avgRealTime") + std::string(",") + std::string("sdRealTime") + std::string(",") + std::string("avgConsumptionCPU") + std::string(",") + std::string("sdConsumptionCPU") + std::string(",") + std::string("avgCpuTime") + std::string(",") + std::string("sdCpuTime") << std::endl;
        outputFile << std::endl;
    }

    if (outputFile.is_open())
    {
        outputFile << bInfo->date << " " << bInfo->cmd << std::endl;
        outputFile << std::to_string(bInfo->benchTurn) + std::string(",") + std::to_string(bInfo->avgConsumptionRT) + std::string(",") + std::to_string(bInfo->sdConsumptionRT) + std::string(",") + std::to_string(bInfo->avgRealTime) + std::string(",") + std::to_string(bInfo->sdRealTime) + std::string(",") + std::to_string(bInfo->avgConsumptionCPU) + std::string(",") + std::to_string(bInfo->sdConsumptionCPU) + std::string(",") + std::to_string(bInfo->avgCpuTime) + std::string(",") + std::to_string(bInfo->sdCpuTime) << std::endl;
        outputFile.close();
    }  
    else
    {
        throw std::runtime_error("FileIO::wrapUpBench() : couldn't open summary file");
    }


    // -- BENCH FILE -- //
    outputFile.open(outputBenchPath, std::fstream::in | std::fstream::out | std::fstream::app);

    if (outputFile.is_open())
    {
        // CONSUMPTION | REAL_TIME
        outputFile << std::endl;
        outputFile << std::string("benchTurns") + std::string(",") + std::string("avgConsumptionRT") + std::string(",") + std::string("sdConsumptionRT") + std::string(",") + std::string("avgRealTime") + std::string(",") + std::string("sdRealTime") + std::string(",") + std::string("avgConsumptionCPU") + std::string(",") + std::string("sdConsumptionCPU") + std::string(",") + std::string("avgCpuTime") + std::string(",") + std::string("sdCpuTime") << std::endl;
        outputFile << std::to_string(bInfo->benchTurn) + std::string(",") + std::to_string(bInfo->avgConsumptionRT) + std::string(",") + std::to_string(bInfo->sdConsumptionRT) + std::string(",") + std::to_string(bInfo->avgRealTime) + std::string(",") + std::to_string(bInfo->sdRealTime) + std::string(",") + std::to_string(bInfo->avgConsumptionCPU) + std::string(",") + std::to_string(bInfo->sdConsumptionCPU) + std::string(",") + std::to_string(bInfo->avgCpuTime) + std::string(",") + std::to_string(bInfo->sdCpuTime) << std::endl;
        outputFile << std::endl;
        outputFile.close();
    } 
    else
    {
        throw std::runtime_error("FileIO::wrapUpBench() : couldn't open bench file");
    }

}



std::string FileIO::readNextCommand()
{
    std::string temp = _commands.front();
    _commands.pop();
    return temp;
}

int FileIO::getNbBench(){return _nbBench;}

int FileIO::getBenchTurns(){return _nbBenchTurns;}


