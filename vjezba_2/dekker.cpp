#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <unistd.h>

using namespace std;

int PRAVO, ZASTAVICA[2];

void ispisi(int i, int k, int m) {
    printf("Dretva: %d, K.O. br: %d (%d/5)\n", i, k, m);
}

void udji_u_ko(int i, int j) {
    ZASTAVICA[i] = 1;
    while(ZASTAVICA[j] != 0) {
        if(PRAVO == j) {
            ZASTAVICA[i] = 0;
            while(PRAVO == j);
                            
            ZASTAVICA[i] = 1;
        }
    }
}

void izadji_iz_ko(int i, int j) {
    PRAVO = j;
    ZASTAVICA[i] = 0;
}

void *dretva(void *x) {
    int i = *((int *)x);
    for(int k = 1; k <= 5; ++k) {
        udji_u_ko(i, 1 - i);
        for(int m = 1; m <= 5; ++m) {
            ispisi(i+1, k, m);
            sleep(1);
        }
        izadji_iz_ko(i, 1 - i);
    }
}

int main() {
    pthread_t thr_id[2];
    int thr_i[2];
   
    for(int i = 0; i <= 1; ++i) {
        thr_i[i] = i;
        if(pthread_create(&thr_id[i], NULL, dretva, &thr_i[i]) != 0) {
            printf("Greska pri stvaranju dretve\n");
            exit(1);
        }
    }

    for(int i = 0; i <= 1; ++i) {
        pthread_join(thr_id[i], NULL);
    }

    return 0;
}
