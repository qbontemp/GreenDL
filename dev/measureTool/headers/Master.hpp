/* but: cette classe est la base du programme de mesure (elle lance les benchmarks).
 */

#ifndef __MASTER_HPP
#define __MASTER_HPP

#include<FileIO.hpp>

class Master{
        int _benchTurns, _nbBench, _nbCpuCore;
        FileIO _fileModule;
    public:
        Master(const std::string& pathConfigFile, const std::string& pathGeneralOutput, const std::string& pathDirectoryBenchFiles, int nbCpuCore);
        void start();
};

#endif
