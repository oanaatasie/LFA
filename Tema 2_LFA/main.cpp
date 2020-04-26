#include <iostream>
#include<fstream>
#include<cstring>
#include<list>
using namespace std;

/// Transformare AFN-lambda in AFD

struct tranzitie{
    int nr_s1;                   /// nr_s1=nr stari din care porneste litera
    int *s1;                     /// *s1=starile din care porneste litera
    char litera;                 /// litera=litera tranzitiei
    int nr_s2;                   /// nr_s2= nr stari care primesc litera
    int *s2;                     /// *s2=starile care primesc litera
};

struct automat{
    int q;                        /// q=nr total stari
    int *init;                    /// *init=staria initiala, sub tip de vector
    int nr_sf;                    /// nr_sf=nr stari finale
    int *sf;                      /// *sf=vector stari finale
    int l;                        /// l=nr de litere din alfabet
    char *L;                      /// *L=alfabetul de intrare
    int t;                        /// t=nr de tranzitii
    tranzitie *T;                 /// *T=vector de tranzitii
};


struct ElementTabel {
    int *vect;
};

/// functie pentru a afla lambda-inchiderea starii curente
/// (adica acele stari in care se ajunge din starea curenta citind lambda la orice putere, inclusiv 0)
void  InchidereLambda(int stare_curenta, int q,int t,tranzitie *T, int *inchiderelambda)
{
    int ok=1;
    inchiderelambda[stare_curenta]=1;  /// se ajunge in ea cu lambda la puterea 0
    while(ok)                        /// cat timp se modifica inchiderelambda
    { ok=0;
      for (int i = 0; i < q; i++)
      {
        if(inchiderelambda[i]==1)
            for(int j=0;j<t;j++)
                if(T[j].s1[0]==i && T[j].litera=='&') /// &=lambda
                       for(int k=0;k<T[j].nr_s2;k++)
                            if(inchiderelambda[T[j].s2[k]]!=1)
                             {
                                 inchiderelambda[T[j].s2[k]]=1;
                                 ok=1;
                             }
    }
    }

}
/// functie care calculeaza inchiderea pentru o litera
void InchidereLitera(int stare_curenta,int q,int t, tranzitie *T, int *inchiderelitera, char c)
{
    int *inchiderelambda=new int[q];
    for (int i = 0; i < q; i++)
        inchiderelitera[i]=0;
    InchidereLambda(stare_curenta,q,t,T,inchiderelambda);
    for(int i = 0; i < q; i++)
       {
        if(inchiderelambda[i]==1)
            for(int j=0;j<t;j++)
                if(T[j].s1[0]==i && T[j].litera==c)
                    for(int k=0;k<T[j].nr_s2;k++)
                      inchiderelitera[T[j].s2[k]]=1;
        }
}
/// functie care calculeaza lambda* litera lambda*
int * InchidereLambdaLiteraLambda(int stare_curenta,int q,int t, tranzitie *T, char c) {

    int *inchidere=new int[q];
    int *inchiderelitera=new int[q];
    int *inchiderelambda=new int[q];
    *inchidere={0};
    for (int i = 0; i < q; i++)
        inchiderelambda[i]=0;
    InchidereLambda(stare_curenta,q,t,T,inchiderelambda);

    for(int i = 0; i < q; i++) {
        if(inchiderelambda[i] == 1) {
            InchidereLitera(stare_curenta,q,t,T,inchiderelitera,c);
        }
    }

    for(int i = 0; i < q; i++) {
        if(inchiderelitera[i] == 1){
            InchidereLambda(i,q,t,T,inchidere);
        }
    }
    return inchidere;


}
/// functie pt reuniunea a 2 vectori:
void reuniuneMultimi(int a[], int b[], int rezultat[], int marime) {

    for (int i = 0; i < marime; i++)
        if (a[i] == 1 || b[i] == 1)
            rezultat[i] = 1;

}
/// verificare daca 2 multimi sunt egale
int verifegal(int *a, int *b, int marime) {
    for (int i = 0; i < marime; i++) {
        if(a[i] != b[i])
            return -1;
    }
    return 1;
}

/// transformare AFNlambda in AFD:
void AfnLambdaInAfd(automat AFNl, automat &AFD)
{   /// cream tabelul AFNl-ului
    ElementTabel tabelAFNl[AFNl.q][AFNl.l];
    for (int i = 0; i < AFNl.q; i++) {
        for (int j = 0; j < AFNl.l; j++) {
            tabelAFNl[i][j].vect = InchidereLambdaLiteraLambda(i, AFNl.q, AFNl.t, AFNl.T, AFNl.L[j]);
        }
    }
    /* Pentru printarea tabelului AFN-lambda:
    for(int i = 0; i < AFNl.q; i++) {
        cout<<i<<" ";
        for(int j = 0; j < AFNl.l; j++) {
            for(int x = 0; x < AFNl.q; x++) {
                if(tabelAFNl[i][j].vect[x] == 1) {
                    cout<<x;
                }
            }
            cout<<"|";
        }
        cout<<endl;
    }
    */

    /// cream tabelul AFD-ului
    ElementTabel tabelAFD[AFNl.q][AFNl.l + 1];       /// tabel AFD reprezinta o matrice cu elemente vector
    ElementTabel elementInitial;                     /// elemetul initial din tabel e starea initiala a AFD-ului
    list<ElementTabel> elementeNoi;                  /// lista de vectori care retine vectorii ce urmeaza a fi introdusi in tabel ca stari ale AFD-ului
    int linietabel=0;
    AFD.sf = new int[AFNl.q];
    AFD.init=new int[AFNl.q];
    elementInitial.vect = new int[AFNl.q];
    InchidereLambda(AFNl.init[0], AFNl.q, AFNl.t, AFNl.T, elementInitial.vect);
    /// starea initiala a AFD-ului este inchiderea lamba a starii initiale a AFNl-ului
    AFD.init=elementInitial.vect;
    elementeNoi.push_back(elementInitial);

    /// cat timp pot fi adaugate elemente noi in tabel
    while(!elementeNoi.empty())
    {
        ElementTabel elementNou = elementeNoi.front(); /// retine vectorul de stari de la inceputul listei
        tabelAFD[linietabel][0] = elementNou; /// adaug elementNou ca stare a AFD-ului
        elementeNoi.pop_front();
        for(int i = 0; i < AFNl.l; i++)
        {
            ElementTabel elementLitera;       /// vectorul starii in care se ajunge prin litera corespunzatoare din starea elementNou
            elementLitera.vect = new int[AFNl.q];
            /// stim faptul ca in dreptul unei litere in tabelul AFD-ului se pune reuniunea a tuturor lambda* litera lamba*
            /// pentru starile care fac parte din starea AFD-ului
            for(int j = 0; j < AFNl.q; j++) {
                if(elementNou.vect[j] == 1){
                    reuniuneMultimi(elementLitera.vect, tabelAFNl[j][i].vect, elementLitera.vect, AFNl.q);
                }
            }

        int aparitie=0;
        /// verificam daca elementul corespunzator literei a fost deja adaugat ca stare in tabel AFD
        for(int j = 0; j <=linietabel; j++)
            if(verifegal(tabelAFD[j][0].vect, elementLitera.vect, AFNl.q) == 1)
                aparitie = 1;                 /// apare deja in tabel

        /// verificam daca elementul urmeaza sa fie adaugat in tabel prin lista elementeNoi
        for(ElementTabel x : elementeNoi)
            if(verifegal(x.vect, elementLitera.vect, AFNl.q) == 1)
                aparitie = 1;                          /// apare in elementeNoi

        if(aparitie == 0)
            elementeNoi.push_back(elementLitera);     /// trebuie adaugat in tabel

        tabelAFD[linietabel][i+1] = elementLitera;
        }

        linietabel++;

    }

    /* Pentru printare tabel AFD:
    for(int i = 0; i < linietabel; i++) {
        for(int j = 0; j < AFNl.l + 1; j++) {
            for(int x = 0; x < AFNl.q; x++) {
                if(tabelAFD[i][j].vect[x] == 1) {
                    cout<<x;
                }
            }
            cout<<"|";
        }
        cout<<endl;
    }
    */

    cout<<"Starile automatului AFD: "<<endl;
    for(int i = 0; i < linietabel; i++)
    {
        for(int x = 0; x < AFNl.q; x++)
            if(tabelAFD[i][0].vect[x] == 1)
                cout<<x;
        cout<<endl;
    }

    /// cream AFD-ul
    AFD.l = AFNl.l;             /// AFD-ul si AFNl-ul au acelasi alfabet
    AFD.L = AFNl.L;
    AFD.t=linietabel*AFD.l;
    AFD.q = linietabel;        /// AFD are ca numar de stari numarul de linii din tabel
    AFD.nr_sf = 0;

    /// calculam starile finale
    /// starile finale ale AFD-ului sunt starile care cuprind cel putin o stare finala a AFNl
    int StariFinale[AFNl.q] = {0};
    for(int i = 0; i < linietabel; i++)
        for(int j  = 0; j < AFNl.q; j++)
        {
            if(tabelAFD[i][0].vect[j] == 1)
                for(int k = 0; k < AFNl.nr_sf; k++)
                    if(AFNl.sf[k] == j)
                        StariFinale[i] = 1;
        }

    for(int i = 0; i < AFD.q; i++)
        if(StariFinale[i] == 1)
            AFD.nr_sf++;              /// calculam numarul de stari finale

    int index = 0;
    for(int i = 0; i < AFD.q; i++)
        if(StariFinale[i] == 1)
            AFD.sf[index++] = i;

     /// setam tranzitiile AFD-ului
     int tranz=0;
     AFD.T=new tranzitie[AFD.t];
     for(int i = 0; i < AFD.q; i++)
        for(int j = 0; j < AFD.l; j++)
            {   AFD.T[tranz].nr_s1=AFNl.q;
                AFD.T[tranz].s1=new int[AFD.T[tranz].nr_s1];
                AFD.T[tranz].s1=tabelAFD[i][0].vect;
                AFD.T[tranz].nr_s2=AFNl.q;
                AFD.T[tranz].s2=new int[AFD.T[tranz].nr_s2];
                AFD.T[tranz].s2=tabelAFD[i][j+1].vect;
                AFD.T[tranz].litera=AFNl.L[j];
                tranz++;
            }
}
int main()
{   ifstream f("date.txt");
    int i,j;
    automat AFD;
    automat AFNl;
    f>>AFNl.q;

    AFNl.init=new int[1];
    f>>AFNl.init[0];

    f>>AFNl.nr_sf;

    AFNl.sf=new int[AFNl.nr_sf];
    for(i=0;i<AFNl.nr_sf;i++)
        f>>AFNl.sf[i];
    f>>AFNl.l;
    AFNl.L=new char[AFNl.l];
    for(i=0;i<AFNl.l;i++)
        f>>AFNl.L[i];
    f>>AFNl.t;
    AFNl.T=new tranzitie[AFNl.t];
    for(i=0;i<AFNl.t;i++)
        {
        f>>AFNl.T[i].nr_s1;
        AFNl.T[i].s1=new int[AFNl.T[i].nr_s1];
        f>>AFNl.T[i].s1[0];    /// folosesc s1[0] pt ca AFN are doar o stare din care se pleaca
        f>>AFNl.T[i].litera>>AFNl.T[i].nr_s2;
        AFNl.T[i].s2=new int[AFNl.T[i].nr_s2];
        for(j=0;j<AFNl.T[i].nr_s2;j++)
            f>>AFNl.T[i].s2[j];
    }

    AfnLambdaInAfd(AFNl,AFD);

    cout<<"Numarul de stari: "<<AFD.q<<endl;
    cout<<"Starea initiala: ";
    for(int i=0;i<AFNl.q;i++)
        if(AFD.init[i]==1)
            cout<<i;
    cout<<endl;
    cout<<"Numarul de stari finale: "<<AFD.nr_sf<<endl;
    cout<<"Numerele corespunzatoare starilor finale: ";
    for(int i=0;i<AFD.nr_sf;i++)
            cout<<AFD.sf[i]<<" ";
         cout<<endl;
    AFD.l=AFNl.l;
    strcpy(AFD.L,AFNl.L);
    cout<<"Nr de litere din alfabet: "<<AFD.l<<endl;
    cout<<"Literele din alfabet: ";
    for(int i=0;i<AFD.l;i++)
        cout<<AFD.L[i]<<" ";
    cout<<endl;
    cout<<"Numarul total de tranzitii: "<<AFD.t<<endl;
    cout<<"Tranzitiile sunt: ";
    cout<<endl;
    for(int i=0;i<AFD.t;i++)
      {
        cout<<i<<")    ";
        for(int j=0;j<AFD.T[i].nr_s1;j++)
            if(AFD.T[i].s1[j]==1)
              cout<<j;
        cout<<"--->";
        for(int j=0;j<AFD.T[i].nr_s2;j++)
            if(AFD.T[i].s2[j]==1)
                cout<<j;
        cout<<" cu litera ";
            cout<<AFD.T[i].litera<<endl;

      }
    return 0;
}
