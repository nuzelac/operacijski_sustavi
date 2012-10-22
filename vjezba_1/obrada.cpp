#include <cstdio>
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include "signali.h"

#define N 6 // broj razina prekida

int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET;

void dump(int mjesto, char znak) {
    for(int i = 0; i < K + 1; ++i) {
        if (i == mjesto) {
            printf(" %c", znak);
        } else {
            printf(" -");
        }
    }
    printf("\n");
}

void dump(int mjesto, int brojac) {
   for(int i = 0; i < K + 1; ++i) {
        if (i == mjesto) {
            printf("%d", brojac);
        } else {
            printf(" -");
        }
    }
    printf("\n");
}

void zabrani_prekidanje() {
    for(int i = 0; i < K; ++i)
        sighold(sig[i]);
}

void dozvoli_prekidanje() {
    for(int i = 0; i < K; ++i)
        sigrelse(sig[i]);
}

void obrada_prekida(int i) {
    dump(i, 'P');
    for(int j = 0; j < 5; ++j) {
        sleep(1);
        dump(i, (char)(j + '1')); 
    }
    dump(i, 'K');
}

void prekidna_rutina(int sig) {
    int n = 1;
    zabrani_prekidanje();
    switch (sig) {
    case SIGUSR1:
        n = 1;
        break;
    case SIGUSR2:
        n = 2;
        break;
    case SIGTERM:
        n = 3;
        break;
    case SIGPIPE:
        n = 4;
        break;
    case SIGINT:
        n = 5;
        break;
    }
    dump(n, 'X');
    OZNAKA_CEKANJA[n] = 1;
    int x = 0;
    do {
        x = 0;
        for(int j = TEKUCI_PRIORITET + 1; j < N; ++j) {
            if(OZNAKA_CEKANJA[j] != 0) {
                x = j;
            }
        }

        if(x > 0) {
            OZNAKA_CEKANJA[x] = 0;
            PRIORITET[x] = TEKUCI_PRIORITET;
            TEKUCI_PRIORITET = x;
            dozvoli_prekidanje();
            obrada_prekida(x);
            zabrani_prekidanje();
            TEKUCI_PRIORITET = PRIORITET[x];
        }
    } while (x > 0);    
    dozvoli_prekidanje();
}

int main() {
    for(int i = 0; i < K; ++i)
        sigset(sig[i], prekidna_rutina);

    printf("Proces obrade prekida, PID=%d\n", getpid());
    printf("GP 1 2 3 4 5\n");
    for(int i = 0; i < 99; ++i) {
        sleep(1);
        if(i < 9)
            dump(0, (char)(i + '1'));
        else
            dump(0, i+1);
    }

    printf("Zavrsio osnovni program\n");
    
    return 0;
}
