#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
using namespace std;

int id; // identifikacijski broj segmenta
int br_procesa;

struct proc {
    int TRAZIM, BROJ;
};

proc *procesi;

void ispisi(int i, int k, int m) {
    printf("Proces: %d, K.O. br: %d (%d/5)\n", i, k, m);
}

int max_br() {
    int sol = 0;
    for(int i = 0; i < br_procesa; ++i)
        if(procesi[i].BROJ > sol) sol = procesi[i].BROJ;
    return sol;
}

void udji_u_ko(int i) {
    procesi[i].TRAZIM = 1;
    procesi[i].BROJ = max_br() + 1;
    procesi[i].TRAZIM = 0;

    for(int j = 0; j < br_procesa; ++j) {
        while(procesi[j].TRAZIM != 0);
        while(procesi[j].BROJ != 0 && (procesi[j].BROJ < procesi[i].BROJ || (procesi[j].BROJ == procesi[i].BROJ && j < i)));
    }
}

void izadji_iz_ko(int i) {
    procesi[i].BROJ = 0;
}


void proces(int i) {
    for(int k = 1; k <= 5; ++k) {
        udji_u_ko(i);
        for(int m = 1; m <= 5; ++m) {
            ispisi(i+1, k, m);
            sleep(1);
        }
        izadji_iz_ko(i);
    }
    }

void izlaz(int sig) {
    shmctl(id, IPC_RMID, NULL);
}

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Nedostaje broj procesa u argumentu programa\n");
        exit(1);
    }
    sigset(SIGINT, izlaz);
    br_procesa = atoi(argv[1]);

    id = shmget(IPC_PRIVATE, sizeof(struct proc) * br_procesa, 0600);
    if(id == -1)
        exit(1);

    procesi = (struct proc *) shmat(id, NULL, 0);

    for(int i = 0; i < br_procesa; ++i) {
        //procesi[i] = (struct proc *)malloc(sizeof(struct proc));
        procesi[i].TRAZIM = 0;
        procesi[i].BROJ = 0;
    }

    for(int i = 0; i < br_procesa; ++i) {
        if(fork() == 0) {
            proces(i);
            exit(0);
        }
    }
    
    for(int i = 0; i < br_procesa; ++i) {
        wait();
        }
   
    izlaz(0);

    return 0;
}
