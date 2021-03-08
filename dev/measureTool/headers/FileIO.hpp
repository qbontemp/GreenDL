/* but: permet de lire le fichier de benchmark et d'écrire les résultats dans les fichiers ad-oc.
 */


#ifndef __FILEIO_HPP
#define __FILEIO_HPP


#include<iostream>
#include<fstream>

#include<string>
#include<cstdlib>
#include<stdexcept>
#include <algorithm>

#include<queue>


struct BenchInfoWork /* Structure pour créer les fichiers du benchmark */
{
    // date début
    std::string date;

    // executed cmd
    std::string cmd;

    // epoch
    //int epoch;
    int benchNb;
    int benchTurn;

    // data du benchmark
    double consumptionRT;
    double consumptionCPU;
    double realTime;    
    double cpuTime;
};

struct BenchInfoFinal /* Strucutre pour créer le fichier résumé pour les benchmark*/
{   
    // date début
    std::string date;

    // executed cmd
    std::string cmd;

    // epoch
    //int epoch;
    int benchNb;
    int benchTurn;

    // average - moyenne
    double avgConsumptionRT;
    double avgConsumptionCPU;
    double avgRealTime;
    double avgCpuTime;

    // std deviation - écart types
    double sdConsumptionRT;
    double sdConsumptionCPU;
    double sdRealTime;
    double sdCpuTime;
};





class FileIO
{
    private:
        std::string _configFilePath;
        std::string _generalOutputPath;
        std::string _benchDirectoryPath;
        int _nbBench;
        int _nbBenchTurns;

        std::queue<std::string> _commands;
        std::ifstream _configFile;

        void initConfigFile();

    protected:

    public:
        FileIO(const std::string& configFilePath, const std::string& generalOutputPath, const std::string& benchDirectoryPath);
        ~FileIO() = default;

        int getNbBench();
        int getBenchTurns();

        std::string readNextCommand();

        void saveBenchInfo(struct BenchInfoWork* bInfo);
        /* 
            Sauvegarde un tour du benchmark.
            // (BenchInfoWork supprimée dans le méthode.)
        */  

        void wrapUpBench(struct BenchInfoFinal* bInfo);
        /*
            Crée un fichier récapitulatif du benchmark.
            // (BenchInfoFinal supprimée dans le méthode.)
        */
        

};

#endif
