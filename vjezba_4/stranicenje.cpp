#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

int zahtjevi[100], br_okvira, br_zahtjeva, cnt = 0;

struct okvir {
    int n, t;

    void ispis() {
        if(n == 0) printf("-"); else printf("%d", n);
    }
};

okvir okviri[10];

void ispis_pogodak(int okvir) {
    for(int i = 0; i < br_okvira; ++i) {
        printf(" ");
        if(i == okvir) {
            printf("(");
            okviri[i].ispis();
            printf(")");
        } else {
            printf(" ");
            okviri[i].ispis();
            printf(" ");
        }
    }    
    printf("  #pogodak\n");
}

void ispis_ubacaj(int okvir) {
    for(int i = 0; i < br_okvira; ++i) {
        printf(" ");
        if(i == okvir) {
            printf("[");
            okviri[i].ispis();
            printf("]");
        } else {
            printf(" ");
            okviri[i].ispis();
            printf(" ");
        }
    }
    printf("\n");
}

void ubaci(int stranica) {
    int ubaci_u = 0;
    printf("%d ", stranica);
    for(int i = 0; i < br_okvira; ++i) {
        if(okviri[i].n == stranica) {
            okviri[i].t = ++cnt;
            ispis_pogodak(i);
            return;
        }
        if(okviri[i].t < okviri[ubaci_u].t) ubaci_u = i;
    }

    okviri[ubaci_u].t = ++cnt;
    okviri[ubaci_u].n = stranica;
    ispis_ubacaj(ubaci_u);
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        printf("Argumenti!");
        exit(-1);
    }
    br_okvira = atoi(argv[1]);
    br_zahtjeva = atoi(argv[2]);

    srand(2);

    printf("Zahtjevi: ");
    for(int i = 0; i < br_zahtjeva; ++i) {
        zahtjevi[i] = rand() % 8 + 1;
        printf("%d", zahtjevi[i]);
        if(i < br_zahtjeva-1) printf(",");
    }
    printf("\n");

    printf("#N");
    for(int i = 0; i < br_okvira; ++i) {
        printf("  %d ", i+1);
    }
    printf("\n");

    printf("--");
    for(int i = 0; i < br_okvira; ++i) {
        printf("----");
    }
    printf("\n");

    for(int i = 0; i < br_zahtjeva; ++i) {
        ubaci(zahtjevi[i]);
    }

        
}
