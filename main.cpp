#include<iostream>
#include<fstream>
#include<cstring>
using namespace std;
char a[101][101][101];
int fina[101],nrf; // starile finale si nr de stari finale
int verif(int nr,int stare,char w[101])
    {   int j,aux[101]={0},ok=0,i=0,k,lung;
        char sir[101];
        strcpy(sir,w);
        if(strlen(sir)==0)
            for(k=0;k<nrf;k++)
            {
                if(stare==fina[k])
                return 1;
            }
        else{
            for(j=0;j<nr;j++)
            {
                lung=strlen(a[stare][j]);
                for(k=0;k<lung;k++)

                    if(a[stare][j][k]==w[0] )
                        {aux[i++]=j; // aux retine starile in care se poate ajunge din starea curenta, folosind prima litera din cuvant
                        ok=1;
                        }
            }

        if(ok==0 && strlen(sir)!=0)
            return 0; //cuv nu este acceptat

        strcpy (sir , sir+1);
        for(k=0;k<i;k++)
            if(verif(nr,aux[k],sir)==1)
                return 1;
        }


           return 0; //cuvantul nu este acceptat deoarece nu s-a ajuns intr-o stare finala


    }

int main()
{   ifstream fin("date.txt");
    int nr,init,i,j,nrtr,k;
    char x,w[101];
    fin>>nr; //nr de stari
    for(i=0;i<nr;i++)
        for(j=0;j<nr;j++)
            strcpy(a[i][j],"0");
    fin>>init; //starea initiala
    fin>>nrf; //nr de stari finale
    for(i=0;i<nrf;i++)
        fin>>fina[i]; //starea finala
    fin>>nrtr; //numar de tranzitii

    for(k=0;k<nrtr;k++)
       {
        fin>>i>>j>>x;
        strncat(a[i][j], &x, 1);
        if(a[i][j][0]=='0')
            strcpy(a[i][j],a[i][j]+1);
       }
    fin>>w;
    if(strcmp(w,"*")==0) // "*" - notatia pt lambda
        {if(fina[0]==init)
         cout<<"Limbajul este acceptat.";
        else
        cout<<"Limbajul nu este acceptat.";
        }
    else
    {
    if(verif(nr,init,w))
        cout<<"Limbajul este acceptat.";
    else
        cout<<"Limbajul nu este acceptat.";
    }
    fin.close();

    return 0;
}
