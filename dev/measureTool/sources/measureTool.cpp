/* but: mesure l'énergie consommée par l'exécution de commandes passées dans son fichier de configuration
 */


#include<iostream>
#include<string>
#include<cstdlib>
#include<stdexcept>
#include<Master.hpp>


int main(int argc, char **argv, char **envp){
    int nbCpuCore = 0;
    std::string pathConfigFile, pathGeneralOutput, pathDirectoryBenchFiles;

    if(argc != 5){
        std::cout << "programName path_conf_file path_generalOutput_file path_directory_benchFiles nb_cpu_core" << std::endl;
        std::cerr << "To few arguments." << std::endl;
        return EXIT_FAILURE;
    }

    nbCpuCore = std::stoi(std::string(argv[4]));
    pathConfigFile = std::string(argv[1]);
    pathGeneralOutput = std::string(argv[2]);
    pathDirectoryBenchFiles = std::string(argv[3]);

    Master master(pathConfigFile, pathGeneralOutput, pathDirectoryBenchFiles, nbCpuCore);
    master.start();
    return EXIT_SUCCESS;
}
