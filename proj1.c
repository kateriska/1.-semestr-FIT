// * Soubor: proj1.c
// * Datum: 5. 11. 2017
// * Autor: Katerina Fortova (xforto00)
// * Predmet: IZP
// * Projekt: Projekt 1 - Prace s textem
// * Popis: Program analyzuje vstup zadany uzivatelem z klavesnice a porovnava ho s textovym souborem adresy.txt.
//          V pripade shody vypise Found, v pripade neshody pismen Not found, jinak vypisuje dostupna pismena na dalsi pozici.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ZAPORNY_ARGUMENT (-1) // argument nemuze byt zaporny, ale tato hodnota funguje pro ukoncovani cyklu

// void bubbleSort(int * pismena, int size)
// {
//    for (int i = 0; i < 43 - 1; i++)
//    {
//	for (int j = 0; j < 43 - i - 1; j++)
//	    {
//		if (pismena[j+1] < pismena[j])
//		{
//		    int tmp = pismena[j+1];
//		    pismena[j+1] = pismena[j];
//		    pismena[j] = tmp;
//		}
//	    }
//	}
// }


int main (int argc, char *argv[])
{
    (void) argc;

    int i = 0; // promenna urcujici pole mesto a vstup
    int x = 0; // promenna urcujici pole pismena
    char mesto[101] = ""; // adresy.txt
    char pismena[43] = ""; // pole pro ukladani pismen, co jsou vysledkem hledani
    char vstup[101] = "";  // pole pro zadavani argumentu z klavesnice
    int duplicita = 0; // pri duplicite se vypisuje pouze jedna hodnota pro Found

    strcpy(vstup, argv[1]); // kopiruje argument z klavesnice do pole vstup

    int code = scanf("%100s", mesto); // precte prvni radek ze souboru adresy.txt a ulozi ho do pole mesto

    for (i = 0; vstup[i] != '\0'; i++)
    {
        if (vstup[i] < 32 || vstup[i] > 126) // osetreni chyby - jsou povolene pouze tisknutelne znaky z ASCII tabulky
        {
            printf("Vase vyhledavani obsahuje nepovolene znaky\n");
            return (EXIT_FAILURE);
        }
    }

    for (i = 0; vstup[i] != '\0'; i++) // prevod naseho argumentu z klavesnice na velka pismena
    {
        vstup[i] = toupper(vstup[i]);
    }

    i = 0; // navrat i do vychozi hodnoty

    while (code == 1)  // nacitani slova ze stdin
    {
        for (i = 0; mesto[i] != '\0'; i++)
        {
            mesto[i] = toupper(mesto[i]);
        }

        for (i = 0; mesto[i] != '\0'; i++)  // osetreni chyby u adresy.txt  - jsou povolene pouze tisknutelne znaky
        {
            if (mesto[i] < 32 || mesto[i] > 126)
            {
                printf("Soubor adresy.txt obsahuje nepovolene znaky\n");
                return (EXIT_FAILURE);
            }
        }

        i = 0;

        while (i != ZAPORNY_ARGUMENT)
        { // dokud je i kladne tak se i zvysuje, neni zaporne pole, takze cyklus konci

            if (vstup[i] != mesto[i]) // kdyz se nase hledani a mesto neshoduji
            {
                if (vstup[i] == '\0')
                {
                    pismena[x] = mesto[i];
                    x = x + 1; // ulozeni posledniho znaku, co se neshodoval do pismena[x]
                }
                i = ZAPORNY_ARGUMENT;
                code = scanf("%100s", mesto);  // nacita dalsi radek souboru adresy.txt
            }
            else if (vstup[i] == mesto[i])
            {

                if (vstup[i] == '\0')
                {
                    if (duplicita == 0) // vypsani pri nalezeni hledaneho mesta
                    {
                        printf("Found: %s\n", vstup);
                        i = ZAPORNY_ARGUMENT; // ukonci se cyklus
                        code = scanf("%100s", mesto);
                        duplicita = 1;
                    }
                    else
                    {
                        i = ZAPORNY_ARGUMENT;
                        code = scanf("%100s", mesto);
                    }
                }
                else
                {
                    i++;
                }
            }
        }
    }

//    bool obsahuje = false; // osetreni pismen, ktere se v Enable objevuji dvakrat
//
//	for (x = 0; x < 43; x++)
//	{
//	    if (pismena[x] == pismena[x + 1])
//	    {
//		obsahuje = true;
//		break;
//	    }
//	}

    // zjisteni a vypis Found a Enable, Enable nebo Not found
    if (duplicita == 0)
    {
        if (isalpha(pismena[0]) == 0)
        {
            printf("Not found.\n");
        }
        else
        {
            printf("Enable: %s\n", pismena);
        }
    }
    else if (isalpha(pismena[0]) != 0)
    {
        printf("Enable: %s\n", pismena);
    }
    return (EXIT_SUCCESS);
}