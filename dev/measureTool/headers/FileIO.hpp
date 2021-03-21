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

        /*
            Renvoit le nombre de benchmarks à exécuter.
        */
        int getNbBench();

        /*
            Renvoit le nombre de tours d'un benchmark.
        */
        int getBenchTurns();

        /*
            Renvoit de la prochaine commande à exécuter.
        */
        std::string readNextCommand();

        /* 
            Sauvegarde un tour du benchmark.
        */  
        void saveBenchInfo(struct BenchInfoWork* bInfo);
        
        /*
            Crée un fichier récapitulatif du benchmark.
        */
        void wrapUpBench(struct BenchInfoFinal* bInfo);
};

#endif
