/* 
 * Soubor: proj2.c
 * Datum: 14. 11. 2017
 * Autor: Katerina Fortova (xforto00)
 * Predmet: IZP
 * Projekt: Projekt 2 - Iteracni vypocty
 * Popis: Program porovnava presnosti vypoctu funkce tangens s vyuzitim Taylorova polynomu, zretezeneho zlomku a hodnoty tangens z matematicke knihovny.
 * Dale vypisuje absolutni chyby. Pri zadani argumentu -m a -c provadi mereni vysky pozorovaneho objektu a vzdalenosti od nej.
 * Created on 6. listopadu 2017, 10:17
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#define PI 3.141592653589793238462643

/*
 * 
 */

void printhelp() // vypise napovedu
{
    printf(
		         "\nNapoveda:\n"
		         "Program: proj2\n"
		         "Autor: Katerina Fortova (xforto00)\n"
		         "Pouziti:\n" 
		         "	proj2 --help\n"
		         "	proj2 --tan\n"
		         "	proj2 -m\n"
			 "	proj2 -c\n"
                         "Omezeni:\n"
                         "      uhly alfa a beta  zadavajte v intervalu 0 < uhel <= 1.4 < PI/2\n"
                         "      pocet iteraci zadavejte v intervalu 0 < prvne zadavana iterace <= druha zadavana iterace < 14\n"
                         "      vysku -c zadavajte v intervalu 0 < c <= 100, implicitni hodnota vysky je 1.5 m\n"
			 "Program spocita a vypise presnost tangens nebo vypocita a zmeri vzdalenosti\n"
           );
}

double taylor_tan(double x, unsigned int n) // funkce pro tayloruv polynom pro vypocet tangens
{
   double citatel[] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604};
   double jmenovatel[] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
 
   unsigned int i;
   double tan = 0;
   double y = x * x;
   
   for (i = 0; i < n; i++)
   {
       tan += x * citatel[i] / jmenovatel[i];
       x *= y;
   }
   return tan;
}

double cfrac_tan(double x, unsigned int n) // funkce pro zretezeny zlomek pro vypocet tangens, podle druheho vzorce
{
    double cf = 0.;
    
    double a;
    double b = 1; // pomuzu si dalsima dvema promennyma
    unsigned int i;
    
    for (i = n; i > 0; i--) 
    {
        a = (i*2 - 1) / x;
        
        cf = b / (a - cf);
        
    }
    
    return cf;
          
}

double chyba_taylor(double x, unsigned int n) // absolutni chyba mezi vypoctem pomoci Taylora a vypoctu pomoci tangens z matematicke knihovny
{
    double TE;
    
    TE = tan(x) - taylor_tan(x,n);
    
    return TE;
}

double chyba_zlomek(double x, unsigned int n) // absolutni chyba mezi vypoctem pomoci zretezenych zlomku a tangens z matematicke knihovny
{
    double CE;
    
    CE = tan(x) - cfrac_tan(x,n);
    
    return CE;
}

// MAIN
int main(int argc, char *argv[]) 
{
    int j = 1; // do promenne j se ukladaji argumenty z prikazove radky
    double alfa; // uhel alfa
    double beta; // uhel beta
    unsigned int pocatecni_iterace; // prvni iterace, kterou chceme zjistit
    unsigned int koncova_iterace; // posledni iterace, kterou chceme zjistit
    unsigned int n; // promenna n, do ktere se pote uklada pocatecni_iterace a pouziva se pri pruchodu cyklem
    float d; // delka - vzdalenost od objektu
    float v; // vyska objektu
    float c; // " vyska pozorovatele ", implicini vyska je 1.5
    
    (void) argc;
    
    if (strcmp(argv[j], "--help") == 0)
    {
            printhelp(); // vypsani napovedy pri zadani help
    }
        
    else if (strcmp(argv[j], "--tan") == 0) // vypsani porovnani pri zadani --tan
    { 
        alfa = atof(argv[j+1]); 
        pocatecni_iterace = atoi(argv[j+2]);
        koncova_iterace = atoi(argv[j+3]);
        n = pocatecni_iterace; // pocatecni iterace kterou chceme se ulozi do promenne n
        
        if (0 > alfa || (alfa > 1.4 && alfa > PI/2))
        { 
            printf("Spatne zadany definicni obor u uhlu alfa. Vice informaci po zavolani --help.\n");
            return (EXIT_FAILURE);
        }
        
        if ((pocatecni_iterace > koncova_iterace) || (pocatecni_iterace > 14 || koncova_iterace > 14))
        {
            printf("Iterace neodpovidajici zvolenemu intervalu. Vice informaci po zavolani --help.\n");
            return (EXIT_FAILURE);
        }
        
        for (n = pocatecni_iterace; n <= koncova_iterace; n++)
        {
            printf("%d %e %e %e %e %e\n", n, tan(alfa), taylor_tan(alfa,n), chyba_taylor(alfa,n), cfrac_tan(alfa,n), chyba_zlomek(alfa,n)); // cyklus pro vypsani
            
        }
        
    }
    
    // mereni
    
    
    else if ((strcmp(argv[j], "-c") == 0) && (strcmp(argv[j+2], "-m") == 0)) // mereni pri uprave parametru c
    {
        c = atof(argv[j+1]);
        alfa = atof(argv[j+3]);
        beta = atof(argv[j+4]);
        
        if (0 > alfa || (alfa > 1.4 && alfa > PI/2))
        { 
            printf("Spatne zadany definicni obor u uhlu alfa. Vice informaci po zavolani --help.\n");
            return (EXIT_FAILURE);
        }
        
        if (0 > beta || (beta > 1.4 && beta > PI/2))
        { 
            printf("Spatne zadany definicni obor u uhlu beta. Vice informaci po zavolani --help.\n");
            return (EXIT_FAILURE);
        }
        
        if (c < 0 || c > 100)
        {
            printf("Spatne zadana vyska c. Vice informaci po zavolani --help.\n");
            return (EXIT_FAILURE);
        }
      
        d = c / cfrac_tan(alfa, 11);
        v = (d * (cfrac_tan(beta, 11))) + c;
        printf("%.10e\n%.10e\n", d, v);
      
    } 
    
    else if (strcmp(argv[j], "-m") == 0) // mereni pri implicitnim zadani parametru c
    {
        alfa = atof(argv[j+1]);
        beta = atof(argv[j+2]);
        
        if (0 > alfa || (alfa > 1.4 && alfa > PI/2))
        { 
            printf("Spatne zadany definicni obor u uhlu alfa. Vice informaci po zavolani --help.\n");
            return (EXIT_FAILURE);
        }
        
        if (0 > beta || (beta > 1.4 && beta > PI/2))
        { 
            printf("Spatne zadany definicni obor u uhlu beta. Vice informaci po zavolani --help.\n");
            return (EXIT_FAILURE);
        }
        
        d = 1.5 / cfrac_tan(alfa,12);
        v = (d * (cfrac_tan(beta, 12))) + 1.5;
        printf("%.10e\n%.10e\n", d, v);
        
    }
        
    return (EXIT_SUCCESS);
}
