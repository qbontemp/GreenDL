/* but: prend les mesures pendant l'exécution du programme.
 * usage: le construire, appeller Listener::launch(instanceConstruite) et pour le stopper appeller Listener::stop(instanceConstruite)
 * */

#ifndef __LISTENER_HPP
#define __LISTENER_HPP

#include<thread>
#include<chrono>
#include<raplcap.h>
#include<string>
#include<unistd.h>

/**
 * structure storing measures informations
 */
struct traveler{
    double energy; //en joules
    double elapsedCpuTime; //temps en secondes
    double cpuLoad; //cpu load moyen durant l'exécution
    std::chrono::duration<double, std::ratio<1,1>> elapsedRealTime; // elapsedRealTime.count() pour convertir en secondes
};

class Listener{
        std::thread _currentThread; //ref vers le thread dans le quel le Listerner s'exécute
        bool _stop;                 //si true, stop le thread
        struct traveler *_messager; //la structure dans laquelle stocker les résultats
        pid_t _child;               //le pid du processus à surveiller (celui dans lequel tourne le code à mesurer)
        bool _error;                //si true, une erreur est survenue dans l'exécution
        std::string _errorMessage;  //le message d'erreur associé à _error
        unsigned long int *_dieCount; //un tableau qui compte le nombre de die pour chaque package cpu
        double **_firstE, **_secondE, **_maxE; //tableaux d'énergie, tableau[x][y], x==package ,y==die
        raplcap _capDivice;         //raplcap object
        int _packagesCount;         //nombre de cpu packages
        void start();               //lance l'exécution
        void cleanUp();             //détruit les tableaux dynamiques
    public:
        Listener(struct traveler& messager);
        
        /**
         * constructeur de transfer.
         */
        Listener(Listener&& toTrans);
        
        /**
         * opérateur d'assignation par transfer.
         */
        Listener& operator=(Listener&& toTrans);
        
        /**
         * retourne true si une erreur est survenue ==> résultats non utilisables ==> arrêt du programme
         */
        bool isError();
        
        /**
         * retourne le message d'erreur à afficher (si isError est true)
         */
        std::string getErrorMessage();
        
        /**
         * Launch le Listener dans un nouveau thread (il doit s'exécuter en même temps que le programme mesuré).
         * retourne quand le thread est lancé
         */
        static void launch(Listener &toLaunch, pid_t child);
        
        /**
         * Stop un Listener lancé dans un autre thread.
         * retourne quand le thread est stoppé
         */
        static void stop(Listener &toStop);
        ~Listener();
};

#endif
