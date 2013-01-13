#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

using namespace std;

pthread_t thr_id[5];
int thr_i[5];
char stanje[5];
bool vilica[5];
pthread_mutex_t M;
pthread_cond_t red[5];

void ispisi_stanje(int n) {
    printf("%c %c %c %c %c (%d)\n", stanje[0], stanje[1], stanje[2], stanje[3], stanje[4], n + 1);
}

void misliti() {
    sleep(3);
}

void jesti(int n) {
    pthread_mutex_lock(&M);
    stanje[n] = 'o';
    while(vilica[n] == 0 || vilica[(n + 1) % 5] == 0)
        pthread_cond_wait(&red[n], &M);
    vilica[n] = vilica[(n + 1) % 5] = 0;
    stanje[n] = 'X';
    ispisi_stanje(n);
    pthread_mutex_unlock(&M);

    sleep(2);

    pthread_mutex_lock(&M);
    stanje[n] = 'O';
    vilica[n] = vilica[(n + 1) % 5] = 1;
    pthread_cond_signal(&red[(n - 1) % 5]);
    pthread_cond_signal(&red[(n + 1) % 5]);
    ispisi_stanje(n);
    pthread_mutex_unlock(&M);
}

void *dretva(void *x) {
    int i = *((int *)x);
    while(1) {
        misliti();
        jesti(i);
    }
}

int main() {

    pthread_mutex_init(&M, NULL);
    for(int i = 0; i < 5; ++i) {
        vilica[i] = 1;
        stanje[i] = 'O';
        pthread_cond_init(&red[i], NULL);
    }

    
    for(int i = 0; i < 5; ++i) {
        thr_i[i] = i;
        if(pthread_create(&thr_id[i], NULL, dretva, &thr_i[i]) != 0) {
            printf("Greska pri stvaranju dretve\n");
            exit(1);
        }
    }

    for(int i = 0; i < 5; ++i) {
        pthread_join(thr_id[i], NULL);
    }

    return 0;
}
