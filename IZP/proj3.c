/* 
 * Soubor: proj3.c
 * Datum: 3. 12. 2017
 * Autor: Katerina Fortova (xforto00)
 * Predmet: IZP
 * Projekt: Projekt 3 - Jednoducha shlukova analyza
 * Popis: Program implementuje jednoduchou shlukovou analyzu. Kazdy objekt je dvourozmerny, ma jedinecny identifikator a je ulozen v textovem souboru.
 * Program spoustejte s argumenty ./proj3 SOUBOR [N], N je volitelny argument definujici celkovy pocet shluku, vychozi hodnota je nastavena na 1.
 * Created on 6. listopadu 2017, 10:17
 */

/**
 * Kostra programu pro 3. projekt IZP 2017/18
 *
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */


void open_clust_file(char *file_name)
{
  
    FILE *input_file;
    int clust_count;
    int a, b, c;
    int i;
    
    input_file = fopen(file_name, "r"); // otevru soubor pro cteni
    
    if(input_file != NULL) // pokud se soubor podarilo otevrit
    {
        printf("soubor se podarilo otevrit");
        
        fscanf(input_file,"count=%d", &clust_count);
        printf("pocet clusteru = %d\n", clust_count);
        
        for(i = 0; i < clust_count; i++)
        {
            fscanf(input_file, "%d %d %d", &a, &b, &c);
            printf("%d %d %d\n", a, b, c);
        }
        
        fclose(input_file); // zavru soubor
    }
    else // soubor se nepodarilo otevrit
    {
        printf("soubor se nepodarilo otevrit");
        return; // opustim funkci
    }  
}


/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO
    c->size = 0;
    c->capacity = cap;
    c->obj = malloc(cap * sizeof(struct obj_t));
    if(c->obj == NULL)
    {
        c->capacity = 0;
        fprintf(stderr,"Alokace pameti selhala! malloc\n");
    }
    
    
    
}



/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
    free(c->obj);        
    c->size = 0;
    c->capacity = 0;
    c->obj = NULL;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
    return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO
   if(c->size == c->capacity) 
   {
        c = resize_cluster(c, c->capacity + CLUSTER_CHUNK);
        
        if(c == NULL)
     {
         fprintf(stderr,"Alokace pameti selhala! resize_cluster\n");
         return;
     }   
   }
 
   
   c->obj[c->size++] = obj;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
   for (int i = 0; i < c2->size; i++) 
   {
        append_cluster(c1, c2->obj[i]);
   }
 
  sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO
    
    int pom = narr - 1;
    clear_cluster(&carr[idx]);
    
    for(int i = idx; i < narr - 1; i++)
    {
        carr[i] = carr[i + 1];
    }
    
    return pom;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO
 
    // souradnice jsou typu int
    
    int dx, dy, result; 
    
    dx = o2->x - o1->x;
    dy = o2->y - o1->y;
    
    result = sqrt(dx*dx + dy*dy);

    return result;
    
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO    
    // 0 <= X <= 1000, 0 <= Y <= 1000
    float min_distance = 0;
    float actual_distance = 0;
    int i, j;
 
    for(i = 0; i < c1->size; i++) 
    {
        for(j = 0; j < c2->size; j++) 
        {  
            actual_distance = obj_distance(&c1->obj[i], &c2->obj[j]);
            
            min_distance = min_distance + actual_distance;
            }      
        }
    min_distance = min_distance / (c1->size * c2->size);
  return min_distance;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO
    int pos_x = 0, pos_y = 0, i, j;
    float min_distance = 2000, cur_distance;
 
    for(i = 0; i < narr; i++) {
 
    for(j = i + 1; j < narr; j++) {
      cur_distance = cluster_distance(&carr[i], &carr[j]);
      if(cur_distance <= min_distance) {
        min_distance = cur_distance;
        pos_x = i;
        pos_y = j;
      }
    }
  }
  *c1 = pos_x;
  *c2 = pos_y;
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    //assert(arr != NULL);

    // TODO
    
    FILE *input_file;
    int count;
    struct obj_t obj;
    int pom;
    
    input_file = fopen("objekty.txt", "r"); // otevru soubor pro cteni
    
    if(input_file != NULL) // pokud se soubor podarilo otevrit
    {
        //printf("soubor se podarilo otevrit");        
        pom = fscanf(input_file,"count=%d", &count);
        //printf("pocet clusteru = %d\n", clust_count);
        
        if(pom != 1 || count <= 0) //pokud neplatny format prvniho radku souboru
        {
          fprintf(stderr, "Prvni radek souboru \"%s\" ma neplatny format nebo count mimo rozsah!\n", filename); 
          if(fclose(input_file) == EOF)
          {
            fprintf(stderr, "Soubor \"%s\" se nepodarilo zavrit!\n", filename);
          }
          return -1;
        }
        
        // alokace pameti pro nacteny pocet shluku
        *arr = malloc(sizeof(struct cluster_t) * count);
        if (*arr == NULL) 
        {
         fprintf(stderr,"Alokace pameti selhala! malloc\n");
         if (fclose(input_file) == EOF)
          {
           fprintf(stderr,"Soubor \"%s\" se nepodarilo zavrit\n", filename);
          }
         return -1;
        }
        
      
        
        for(int i = 0; i < count; i++)
        {
            
         if(fscanf(input_file, "%d %f %f", &obj.id, &obj.x, &obj.y) != 3)
         // pocet uspesne nactenych hodnot musi byt 3
         {
          fprintf(stderr, "Nespravny format dat nebo poskozeny soubor \"%s\" - chyba na pozici i=%d\n", filename,i);
          
          if(fclose(input_file) == EOF)
          {
            fprintf(stderr, "Soubor \"%s\" se nepodarilo zavrit!\n", filename);
          }
          for(int j = 0; j < i; j++)  //uvolneni dosud naalokovane pameti
          {
            clear_cluster(&(*arr)[j]);
          }
         return -1;
        }
         
         // kontrola rozsahu souradnic
         if(obj.x < 0 || obj.x > 1000 || obj.y < 0 || obj.y > 1000)
         {
          fprintf(stderr, "Souradnice na pozici i=%d mimo povoleny rozsah\n", i);
          if(fclose(input_file) == EOF)
           {
            fprintf(stderr, "Soubor \"%s\" se nepodarilo zavrit!\n", filename);
           }
          return -1;
         }
         
         init_cluster(&(*arr)[i], 1);
         append_cluster(&(*arr)[i], obj);
        }
    } 
    else // soubor se nepodarilo otevrit
    {
      fprintf(stderr, "Soubor \"%s\" se nepodarilo otevrit", filename);
      return -1;
    } 
    
    fclose(input_file);
    
    return count;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;   
    // TODO
    
     int c1 = 0;
  int c2 = 0;
  int count;
  char *ptr;
  int n;
 
  if(argc == 3) {
    n = strtol(argv[2], &ptr, 10);
    if(*ptr!= '\0' || !(n > 0)) {
      fprintf(stderr,"Error! Spuste program s argumenty: ./proj3 \"jmeno souboru\" \"pocet objektu\"!\n"
                      "Soubor musi na prvnim radku obsahovat \"count=n\", kde n > 0 a dale n radku ve tvaru [id] [x] [y]\n");
 
      return 1;
    }
  }
  else if(argc == 2) {
    n = 1;
  } else {
    fprintf(stderr,"Error! Spuste program s argumenty: ./proj3 \"jmeno souboru\" \"pocet objektu\"!\n"
                      "Soubor musi na prvnim radku obsahovat \"count=n\", kde n > 0 a dale n radku ve tvaru [id] [x] [y]\n");
 
    return 2;
  }
 
  if((count = load_clusters(argv[1], &clusters)) == -1) {
    fprintf(stderr,"Error! Spuste program s argumenty: ./proj3 \"jmeno souboru\" \"pocet objektu\"!\n"
                      "Soubor musi na prvnim radku obsahovat \"count=n\", kde n > 0 a dale n radku ve tvaru [id] [x] [y]\n");
 
    return 3;
  }
 
  while(count > n) {
    find_neighbours(clusters, count, &c1, &c2);
    merge_clusters(&clusters[c1], &clusters[c2]);
    count = remove_cluster(clusters, count, c2);
  }
 
  print_clusters(clusters, count);
 
  for(int i = 0; i < count; i++) {
    clear_cluster(&clusters[i]);
  }
 
  free(clusters);
  return 0;
}
