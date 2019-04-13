/* 
 * Soubor: proj3.h
 * Datum: 9. 12. 2017
 * Autor: Katerina Fortova (xforto00)
 * Predmet: IZP
 * Projekt: Projekt 3 - Jednoducha shlukova analyza
 * Popis: Program implementuje jednoduchou shlukovou analyzu. Kazdy objekt je dvourozmerny, ma jedinecny identifikator a je ulozen v textovem souboru.
 * Program spoustejte s argumenty ./proj3 SOUBOR [N], N je volitelny argument definujici celkovy pocet shluku, vychozi hodnota je nastavena na 1.
 * proj3.h je vytvoren pomoci anotovanych komentaru pro dokumentacni nastroj Doxygen.
 * Created on 6. listopadu 2017, 10:17
 */

/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2017/18
 * a pro dokumentaci Javadoc.
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

/**
 * Zkontroluje vstupni soubor
 * @param file_name je nazev souboru
 * @return v pripade neuspechu otevreni souboru
 */
void open_clust_file(char *file_name);

/**
 * Inicializace shluku 'c'
 * @param c je shluk 'c'
 * @param cap je kapacita objektu
 * @pre spravne alokovana pamet pro pole shluku
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * Odstraneni vsech objektu shluku a inicializace na prazdny shluk
 * @param c je shluk 'c'
 */
void clear_cluster(struct cluster_t *c);

extern const int CLUSTER_CHUNK;

/**
 * Zmena kapacity shluku 'c' na kapacitu 'new_cap'
 * @param c je shluk 'c'
 * @param new_cap je nova kapacita shluku
 * @return NULL v pripade arr == NULL
 * @return c v pripade c->capacity = new_cap
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * Prida objekt 'obj' na konec shluku 'c'
 * @param c je shluk 'c'
 * @param obj je objekt, ktery je pridan na konec shluku 'c'
 * @return v pripade, ze c == NULL, tedy alokace pameti selhala
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * Do shluku 'c1' prida objekty 'c2'
 * @param c1 je shluk 'c1', v pripade nutnosti rozsiren
 * @param c2 je shluk 'c2', jehoz objekty se pridavaji do shluku 'c1'
 * @post nalezeni dvou nejblizsich shluku
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @defgroup Prace s polem shluku
 */

/**
 * Odstrani shluk z pole shluku 'carr'
 * @param carr je pole shluku
 * @param narr je pocet polozek pole shluku
 * @param idx je index, kde se nachazi shluk pro odstraneni
 * @return pom vraci novy pocet shluku v poli
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * Vypocet Euklidovskou vzdalenost mezi dvema objekty
 * @param o1 je prvni objekt
 * @param o2 je druhy objekt
 * @return result vraci jejich vzdalenost
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * Pocita vzdalenost dvou shluku
 * @param c1 je prvni shluk
 * @param c2 je druhy shluk
 * @return min_distance vraci vzdalenost dvou shluku
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Funkce najde dva nejblizsi shluky
 * @param carr je pole shluku
 * @param narr je velikost pole shluku
 * @param c1 je adresa pro ukladani nalezenych shluku
 * @param c2 je adresa pro ukladani nalezenych shluku
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * Razeni objektu ve shluku vzestupne podle jejich identifikatoru
 * @param c je shluk 'c'
 */
void sort_cluster(struct cluster_t *c);

/**
 * Tisk shluku 'c' na stdout
 * @param c je shluk 'c'
 */
void print_cluster(struct cluster_t *c);

/**
 * Nacita objektu ze souboru
 * @param filename je nazev souboru
 * @param arr je pole shluku
 * @return -1 v pripade nalezeni chyby
 * @return count v pripade uspechu, vraci pocet nactenych objektu (shluku)
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * Tiskne pole shluku
 * @param carr je ukazatel na prvni polozku (shluk)
 * @param narr je pocet tisknutych shluku
 */
void print_clusters(struct cluster_t *carr, int narr);
