#include <cstdio>
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include "signali.h"

int pid = 0;

void prekidna_rutina(int sig) {
    if(pid != 0)
        kill(pid, SIGKILL);

    exit(0);
}

int main(int argc, char *argv[]){
    if(argc == 1) {
        printf("Unesi PID\n");
        return 0;
    }
    pid = atoi(argv[1]);
    if(pid == 0) {
        printf("Unesi PID\n");
        return 0;
    }
    sigset(SIGINT, prekidna_rutina);

    while(1){
        int t = rand() % 5 + 3;
        sleep(t);
        int i = rand() % K;
        kill(pid, sig[i]);
    }
}