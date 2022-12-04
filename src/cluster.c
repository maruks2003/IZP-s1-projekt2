/**
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <stdbool.h>

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

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO - hopefully done
    c->size = 0;
    c->obj = NULL;
    c->capacity = cap;

    if(cap != 0){
        c->obj = malloc(sizeof(struct obj_t)*cap);
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO - hopefully done
    free(c->obj);
    c->obj = NULL;
    c->capacity = 0;
    c->size = 0;
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


    void *arr = realloc(c->obj, sizeof(struct obj_t) * new_cap);

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO - hopefully done
    if (c->size >= c->capacity){
        resize_cluster(c, c->capacity+CLUSTER_CHUNK);
    }
    c->obj[c->size] = obj;
    c->size += 1;
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

    // TODO - hopefully done
    for (int i = 0; i < c2->size; ++i){
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

    // TODO - hopefully done

    clear_cluster(&carr[idx]);
    for(int i = idx; i < narr-1; i++){
        carr[i] = carr[i+1];
    }

    return narr-1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO - hopefully done
    float dist = sqrtf(powf(o1->x - o2->x, 2) + powf(o1->y - o2->y, 2));
    return dist;
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

    // TODO - hopefully done
    // We set the initial distance
    float min = obj_distance(&c1->obj[0], &c2->obj[0]);

    // The value which we will be comparing with min
    float cmp;

    for(int i = 0; i < c1->size; ++i){
        for(int j = 0; j < c2->size; ++j){
            cmp = obj_distance(&c1->obj[i], &c2->obj[j]);
            if(cmp < min){
                // cmp is our new smallest distance
                min = cmp;
            }
        }
    }
    return min;
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

    // TODO - hopefully done 

    // We can't find neighobours if we don't have
    // enough clusters to compare
    if (narr < 2){
        *c1 = -1;
        *c2 = -1;
        return;
    }

    // If we have only 2 clusters then they must be neighbours
    if (narr == 2){
        *c1 = 0;
        *c2 = 1;
        return;
    }

    // We set the initial smallest distance
    float min = cluster_distance(&carr[0], &carr[1]);
    float cmp;

    for(int i = 0; i < narr; ++i){
        for(int j = i+1; j < narr; ++j){
            // Compare clusters and 
            cmp = cluster_distance(&carr[i], &carr[j]); 
            if(cmp < min){
                min = cmp;
                *c1 = i;
                *c2 = j;
            }
        }
    }

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
    assert(arr != NULL);

    // TODO - hopefully done
    FILE *file = fopen(filename, "r");

    if(file == NULL){
        *arr = NULL;
        fprintf(stderr, "Error while opening file\n");
        return 0;
    }

    int count;

    // Error in fscanf
    if(fscanf(file, "count=%d", &count) != 1){
        fclose(file);
        *arr = NULL;
        fprintf(stderr, "Error while reading file\n");
        return 0;
    }
    if(count < 1){
        fclose(file);
        *arr = NULL;
        fprintf(stderr, "Error, count cant be lower than 0\n");
        return 0;
    }

    // obj_t properties
    int id, x, y;

    // Malloc enough space for all the clusters
    // and check for error
    *arr = malloc(sizeof(struct cluster_t)*count);
    if (*arr == NULL){
        *arr = NULL;
        fclose(file);
        return 0;
    }

    // Iterate through as many lines as count says

    // bool
    bool failed = false;
    for(int i = 0; i < count; ++i){
        if(fscanf(file, "%d %d %d", &id, &x, &y) != 3){
            failed = true;
        }

        init_cluster(&(*arr)[i], CLUSTER_CHUNK);

        // Unsuccesfull initialization
        if((*arr)[i].capacity != 0 && (*arr)[i].obj == NULL){
            failed = true;
        }
        if(failed){
            fprintf(stderr, "Wrong input format\n");

            for(int j = 0; j <= i; j++){
                clear_cluster(&(*arr)[j]);
            }

            free(*arr);
            *arr = NULL;
            fclose(file);
            return 0;
        }

        struct obj_t object={id, x, y};
        append_cluster(&(*arr)[i], object);

    }

    fclose(file);
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
    int final_count = 1;

    // Parse the arguments
    //
    // Invalid number of arguments
    // return and print an error
    if(argc < 2 || argc > 3){
        fprintf(stderr, "Error, invalid number of arguments: %d\n", argc-1);
        return 1;
    }
    // Filename is the first argument
    char *filename = argv[1];

    // The cluster count is provided
    if(argc == 3){
        int res = 0;
        res = sscanf(argv[2], "%d", &final_count); 

        if(res == 0){
            // The argument wasn't parsed succesfully
            fprintf(stderr, "The 2nd argument must be a number\n");
            return 1;
        }else if(res < 0){
            fprintf(stderr, "Error while parsing arguments\n");
            return 1;
        }
        if(final_count <= 0){
            // final_count is too small
            fprintf(stderr, "The 2nd argument must be larger than 0\n");
            return 1;
        }
    }
    // End of argument parsing

    int narr = load_clusters(filename, &clusters);
    if(clusters == NULL){
        return 1;
    }

    int c1_idx, c2_idx;
    while(narr > final_count){
        find_neighbours(clusters, narr, &c1_idx, &c2_idx);
        merge_clusters(&clusters[c1_idx], &clusters[c2_idx]);
        narr = remove_cluster(clusters, narr, c2_idx);
    }
    print_clusters(clusters, narr);
    for(int i = 0; i < narr; i++){
        clear_cluster(&clusters[i]);
    }
    free(clusters);

    return 0;
}

