#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

using namespace std;

#define SEM_PRAZAN 0
#define SEM_PUN 1
#define SEM_PISI 2
#define SEM_KO 3
#define SEM_UNOS_GOTOV(id) 3 + id // semafori 4 i 5

int id_pod; // identifikacijski broj segmenta

struct zajednicki {
    int ULAZ, IZLAZ;
    char M[5];
};

zajednicki* podatci; 

int SemId;   /* identifikacijski broj skupa semafora */

void SemGet(int n) {  /* dobavi skup semafora sa ukupno n semafora */
   SemId = semget(IPC_PRIVATE, n, 0600);
   if (SemId == -1) {
      printf("Nema semafora!\n");
      exit(1);
   }
}

int SemSetVal(int SemNum, int SemVal)
{  
    /* postavi vrijednost semafora SemNum na SemVal */
   return semctl(SemId, SemNum, SETVAL, SemVal);
}

int SemOp(int SemNum, int SemOp)
{  
    /* obavi operaciju SemOp sa semaforom SemNum */
   struct sembuf SemBuf;
   SemBuf.sem_num = SemNum;
   SemBuf.sem_op  = SemOp;
   SemBuf.sem_flg = 0;
   return semop(SemId, &SemBuf, 1);
}

void SemRemove(void)
{  
    /* uništi skup semafora */
   (void) semctl(SemId, 0, IPC_RMID, 0);
}

void proizvodjac(int id) {
    char s[100];
    SemOp(SEM_KO, -1);
    printf("Unesi znakove za proizvodjaca %d: ", id);
    scanf("%s", s);
    SemOp(SEM_KO, 1);

    SemOp(SEM_UNOS_GOTOV(id), 1);
    SemOp(SEM_UNOS_GOTOV(3 - id), -1);

    int l = strlen(s);
    for(int i = 0; i <= l; ++i) {
        SemOp(SEM_PUN, -1);
        SemOp(SEM_PISI, -1);
        podatci->M[podatci->ULAZ] = s[i];
        printf("PROIZVODJAC%d -> %c\n", id, s[i]);
        podatci->ULAZ = (podatci->ULAZ + 1) % 5;
        SemOp(SEM_PISI, 1);
        SemOp(SEM_PRAZAN, 1);        
        sleep(1);
    }
}

void potrosac() {
    int br_kraja = 0;
    char s[200];
    int i = 0;
    while(br_kraja < 2) {
        SemOp(SEM_PRAZAN, -1);
        s[i] = podatci->M[podatci->IZLAZ];
        if(s[i] == '\0') ++br_kraja;
        printf("POTROSAC <- %c\n", s[i]);
        podatci->IZLAZ = (podatci->IZLAZ + 1) % 5;
        SemOp(SEM_PUN, 1);
        if(s[i] != '\0') ++i;
    }
    printf("\nPrimljeno je %s", s);
}

void izlaz() {
    shmctl(id_pod, IPC_RMID, NULL);
    SemRemove();
}

int main() {
    id_pod = shmget(IPC_PRIVATE, sizeof(struct zajednicki), 0600);
    if(id_pod == -1) {
        printf("Nisam uspio dobiti zajednicki spremnik\n");
        exit(1);
    }

    podatci = (struct zajednicki *) shmat(id_pod, NULL, 0);

    SemGet(6);
    SemSetVal(SEM_KO, 1);
    SemSetVal(SEM_PISI, 1);
    SemSetVal(SEM_UNOS_GOTOV(1), 0);
    SemSetVal(SEM_UNOS_GOTOV(2), 0);
    SemSetVal(SEM_PRAZAN, 0);
    SemSetVal(SEM_PUN, 5);
    podatci->ULAZ = 0;
    podatci->IZLAZ = 0;

    // stvori dva procesa proizvodjaca
    for(int i = 0; i < 2; ++i) {
        switch(fork()) {
        case 0:
            proizvodjac(i+1);
            exit(0);
        case -1:
            printf("Nisam uspio stvoriti proces!\n");
            break;
        }
    }

    // stvori proces potrosac
    switch(fork()) {
    case 0:
        potrosac();
        exit(0);
    case -1:
        printf("Nisam uspio stvoriti proces potrosac\n");
        break;
    }

    // pricekaj sve procese da zavrse
    for(int i = 0; i < 3; ++i)
        wait(NULL);

    //    sleep(3);
    izlaz();

    return 0;
}
