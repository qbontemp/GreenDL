/* but: cpp de Listener.hpp
 *
 */

#include<string>
#include<raplcap.h>
#include<thread>
#include<chrono>
#include<stdexcept>
#include<Listener.hpp>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>


Listener::Listener(struct traveler& messager): _currentThread(), _stop(false), _messager(&messager), _child(0), _error(false){
    if((_packagesCount = raplcap_get_num_packages(NULL)) == 0){
        throw std::runtime_error("No CPU packages found, are you using a camputer or a toaster dude ?");
    }
    _dieCount = new long unsigned int[_packagesCount];
    for(int i=0;i<_packagesCount;++i){
        if((_dieCount[i] = raplcap_get_num_die(NULL, i)) == 0){
            throw std::runtime_error("No CPU die found, is that even possible sinds you have at least one cpu package ?");
        }
    }
    if(raplcap_init(&_capDivice) != 0){
        throw std::runtime_error("RAPLCAP init failure.");
    }
    _firstE = new double*[_packagesCount];
    _secondE = new double*[_packagesCount];
    _maxE = new double*[_packagesCount];
    for(int i=0;i<_packagesCount;++i){
        _firstE[i] = new double[_dieCount[i]];
        _secondE[i] = new double[_dieCount[i]];
        _maxE[i] = new double[_dieCount[i]];
    }
}

Listener::Listener(Listener&& toTrans): _packagesCount(toTrans._packagesCount),_dieCount(toTrans._dieCount),_firstE(toTrans._firstE),_secondE(toTrans._secondE),_maxE(toTrans._maxE),_stop(toTrans._stop),_messager(toTrans._messager){
    toTrans._dieCount = nullptr;
    toTrans._firstE = nullptr;
    toTrans._secondE = nullptr;
    toTrans._maxE = nullptr;
    toTrans._messager = nullptr;
}

Listener& Listener::operator=(Listener&& toTrans){
    cleanUp();
    _packagesCount = toTrans._packagesCount;
    _dieCount = toTrans._dieCount;
    toTrans._dieCount = nullptr;
    _firstE = toTrans._firstE;
    toTrans._firstE = nullptr;
    _secondE = toTrans._secondE;
    toTrans._secondE = nullptr;
    _maxE = toTrans._maxE;
    toTrans._maxE = nullptr;
    _stop = toTrans._stop;
    _messager = toTrans._messager;
    toTrans._messager = nullptr;
    return *this;
}

void Listener::launch(Listener &toLaunch, pid_t child){
    toLaunch._child = child;
    toLaunch._currentThread = std::thread(&Listener::start, &toLaunch);
}

void Listener::stop(Listener &toStop){
    toStop._stop = true;
    toStop._currentThread.join();
}

bool Listener::isError(){
    return _error;
}

std::string Listener::getErrorMessage(){
    return _errorMessage;
}

void Listener::start(){
    clockid_t horlogue;
    int isError;
    unsigned long long int samples=0, tic=0;
    double cpuTime=0, loadAverage=0;
    double avgSample[1];
    struct timespec timeSample;
    std::chrono::steady_clock::time_point first, second;
    double sum=0, interSum=0;
    if(_child){
        isError = clock_getcpuclockid(_child, &horlogue);
        if(isError != 0){
            _error = true;
            _errorMessage = "Could not retreive process system clock.";
        }
    }else{
        isError = clock_getcpuclockid(getpid(), &horlogue);
        if(isError != 0){
            _error = true;
            _errorMessage = "Could not retreive process system clock.";
        }
    }
    first = std::chrono::steady_clock::now();
    for(int i=0;i<_packagesCount;++i){
        for(int j=0;j<_dieCount[i];++j){
            _maxE[i][j] = raplcap_pd_get_energy_counter_max(&_capDivice, i, j, RAPLCAP_ZONE_PACKAGE);
            _firstE[i][j] = raplcap_pd_get_energy_counter(&_capDivice, i, j, RAPLCAP_ZONE_PACKAGE);
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    while(!_stop && !_error){
        for(int i=0;i<_packagesCount;++i){
            for(int j=0;j<_dieCount[i];++j){
                _secondE[i][j] = raplcap_pd_get_energy_counter(&_capDivice, i, j, RAPLCAP_ZONE_PACKAGE);
                interSum = _secondE[i][j]-_firstE[i][j];
                if(interSum < 0){
                    sum += _maxE[i][j]-_firstE[i][j];
                    sum += _secondE[i][j];
                }else if(interSum == 0){
                    sum += _maxE[i][j];
                }else{
                    sum += interSum;
                }
                _firstE[i][j] = _secondE[i][j];
            }
        }
        isError = clock_gettime(horlogue, &timeSample);
        if(isError == 0){
            cpuTime = timeSample.tv_sec + (timeSample.tv_nsec*(0.000000001));
        }
        if((samples < 50 && (tic%30)==0) || (tic%900)==0){
            ++samples;
            isError = getloadavg(avgSample,1);
            if(isError == -1){
                _error = true;
                _errorMessage = "Load average retreiving failure";
            }
            loadAverage += avgSample[0];
        }
        ++tic;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    second = std::chrono::steady_clock::now();
    _messager->energy = sum;
    _messager->elapsedRealTime = second-first;
    if(_child != 0){
        _messager->elapsedCpuTime = cpuTime;
        _messager->cpuLoad = loadAverage/samples;
    }else{
        _messager->elapsedCpuTime = 0;
        _messager->cpuLoad = 0;
    }
}

void Listener::cleanUp(){
    raplcap_destroy(&_capDivice);
    for(int i=0;i<_packagesCount;++i){
        delete[] _firstE[i];
        delete[] _secondE[i];
        delete[] _maxE[i];
    }
    delete[] _firstE;
    delete[] _secondE;
    delete[] _maxE;
    delete[] _dieCount;
}

Listener::~Listener(){
    cleanUp();
}
