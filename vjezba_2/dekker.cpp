#include <cstdio>
#include <cpthread>

#define BROJ_DRETVI 5

void *dretva() {
    printf("bok ja sam dretva");
    sleep(1);
    printf("dretva je zavrsila");
}

int main() {
    pthread_t thr_id[BROJ_DRETVI];
   
    for(int i = 0; i < BROJ_DRETVI; ++i) { 
        if(pthread_create(&thr_id[i], NULL, dretva, NULL) != 0) {
            printf("Greska pri stvaranju dretve\n");
            exit(1);
        }
    }

    for(int i = 0; i < BROJ_DRETVI; ++i) {
        pthread_join(thr_id[i], NULL);
    }

    return 0;
}
