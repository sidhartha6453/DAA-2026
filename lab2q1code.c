/*
 * Design and Analysis of Algorithms - Lab 02, Q1
 * Dictionary ADT implemented 6 ways, timed to validate asymptotic claims.
 *
 * Structures:
 *   1. Unsorted array
 *   2. Sorted array
 *   3. Singly linked unsorted list
 *   4. Singly linked sorted list
 *   5. Doubly linked unsorted list
 *   6. Doubly linked sorted list
 *
 * Operations timed: Search, Insert, Delete(by pointer), Max, Min,
 *                    Predecessor(by pointer), Successor(by pointer)
 *
 * Output: CSV rows "structure,operation,n,avg_time_seconds" printed to
 * stdout AND written to results.csv, ready to plot (see plot_results.py).
 *
 * Compile:  gcc -O2 -o dictionary_ops dictionary_ops.c
 * Run:      ./dictionary_ops > results.csv
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TRIALS 5
#define NUM_SIZES 7
static const int SIZES[NUM_SIZES] = {1000, 2000, 4000, 8000, 16000, 32000, 64000};

/* ---------- utility: generate n unique random keys ---------- */
static int *gen_unique_keys(int n) {
    int pool = n * 4 + 10;
    int *all = malloc(sizeof(int) * pool);
    for (int i = 0; i < pool; i++) all[i] = i;
    /* Fisher-Yates partial shuffle, take first n */
    for (int i = 0; i < n; i++) {
        int j = i + rand() % (pool - i);
        int t = all[i]; all[i] = all[j]; all[j] = t;
    }
    int *keys = malloc(sizeof(int) * n);
    memcpy(keys, all, sizeof(int) * n);
    free(all);
    return keys;
}

static double now_sec(void) {
    return (double)clock() / CLOCKS_PER_SEC;
}

/* Sink to force the compiler to actually execute (and not optimize away)
 * calls whose return value would otherwise be discarded. */
volatile long sink = 0;

/* =====================================================================
 * 1. UNSORTED ARRAY
 * ===================================================================== */
typedef struct { int *a; int n; } AU;

static AU au_build(int *keys, int n) {
    AU s; s.a = malloc(sizeof(int) * (n + 1000)); s.n = n;
    memcpy(s.a, keys, sizeof(int) * n);
    return s;
}
static void au_free(AU *s) { free(s->a); }
static int au_search(AU *s, int key) {           /* O(n) */
    for (int i = 0; i < s->n; i++) if (s->a[i] == key) return i;
    return -1;
}
static void au_insert(AU *s, int key) {          /* O(1) amortized */
    s->a[s->n++] = key;
}
static void au_delete_at(AU *s, int idx) {        /* O(1): swap with last */
    s->a[idx] = s->a[s->n - 1];
    s->n--;
}
static int au_max(AU *s) {                        /* O(n) */
    int m = s->a[0];
    for (int i = 1; i < s->n; i++) if (s->a[i] > m) m = s->a[i];
    return m;
}
static int au_min(AU *s) {                        /* O(n) */
    int m = s->a[0];
    for (int i = 1; i < s->n; i++) if (s->a[i] < m) m = s->a[i];
    return m;
}
/* predecessor/successor of a.[idx] by KEY ORDER: must scan all (unsorted) O(n) */
static int au_pred(AU *s, int idx) {
    int key = s->a[idx], best = -1, found = 0;
    for (int i = 0; i < s->n; i++)
        if (s->a[i] < key && (!found || s->a[i] > best)) { best = s->a[i]; found = 1; }
    return best;
}
static int au_succ(AU *s, int idx) {
    int key = s->a[idx], best = -1, found = 0;
    for (int i = 0; i < s->n; i++)
        if (s->a[i] > key && (!found || s->a[i] < best)) { best = s->a[i]; found = 1; }
    return best;
}

/* =====================================================================
 * 2. SORTED ARRAY
 * ===================================================================== */
typedef struct { int *a; int n; } AS;

static int cmp_int(const void *a, const void *b) { return (*(int*)a - *(int*)b); }

static AS as_build(int *keys, int n) {
    AS s; s.a = malloc(sizeof(int) * (n + 1000)); s.n = n;
    memcpy(s.a, keys, sizeof(int) * n);
    qsort(s.a, n, sizeof(int), cmp_int);
    return s;
}
static void as_free(AS *s) { free(s->a); }
static int as_search(AS *s, int key) {            /* O(log n) */
    int lo = 0, hi = s->n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (s->a[mid] == key) return mid;
        else if (s->a[mid] < key) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}
static void as_insert(AS *s, int key) {           /* O(n): find pos + shift */
    int pos = 0;
    while (pos < s->n && s->a[pos] < key) pos++;
    for (int i = s->n; i > pos; i--) s->a[i] = s->a[i - 1];
    s->a[pos] = key;
    s->n++;
}
static void as_delete_at(AS *s, int idx) {         /* O(n): shift left */
    for (int i = idx; i < s->n - 1; i++) s->a[i] = s->a[i + 1];
    s->n--;
}
static int as_max(AS *s) { return s->a[s->n - 1]; }  /* O(1) */
static int as_min(AS *s) { return s->a[0]; }         /* O(1) */
static int as_pred(AS *s, int idx) { return idx > 0 ? s->a[idx - 1] : -1; } /* O(1) */
static int as_succ(AS *s, int idx) { return idx < s->n - 1 ? s->a[idx + 1] : -1; } /* O(1) */

/* =====================================================================
 * 3. SINGLY LINKED UNSORTED LIST
 * ===================================================================== */
typedef struct SNode { int key; struct SNode *next; } SNode;
typedef struct { SNode *head; } SLU;

static SLU slu_build(int *keys, int n) {
    SLU s; s.head = NULL;
    for (int i = 0; i < n; i++) {                  /* insert at head, O(1) each */
        SNode *node = malloc(sizeof(SNode));
        node->key = keys[i]; node->next = s.head; s.head = node;
    }
    return s;
}
static void slu_free(SLU *s) {
    SNode *c = s->head;
    while (c) { SNode *t = c; c = c->next; free(t); }
}
static SNode *slu_search(SLU *s, int key) {        /* O(n) */
    SNode *c = s->head;
    while (c) { if (c->key == key) return c; c = c->next; }
    return NULL;
}
static void slu_insert(SLU *s, int key) {           /* O(1): insert at head */
    SNode *node = malloc(sizeof(SNode));
    node->key = key; node->next = s->head; s->head = node;
}
static void slu_delete(SLU *s, SNode *target) {     /* O(n): need prev pointer */
    if (s->head == target) { s->head = target->next; free(target); return; }
    SNode *c = s->head;
    while (c->next != target) c = c->next;
    c->next = target->next;
    free(target);
}
static int slu_max(SLU *s) {                        /* O(n) */
    int m = s->head->key;
    for (SNode *c = s->head; c; c = c->next) if (c->key > m) m = c->key;
    return m;
}
static int slu_min(SLU *s) {                         /* O(n) */
    int m = s->head->key;
    for (SNode *c = s->head; c; c = c->next) if (c->key < m) m = c->key;
    return m;
}
static int slu_pred(SLU *s, SNode *target) {          /* O(n): unsorted scan */
    int key = target->key, best = -1, found = 0;
    for (SNode *c = s->head; c; c = c->next)
        if (c->key < key && (!found || c->key > best)) { best = c->key; found = 1; }
    return best;
}
static int slu_succ(SLU *s, SNode *target) {          /* O(n) */
    int key = target->key, best = -1, found = 0;
    for (SNode *c = s->head; c; c = c->next)
        if (c->key > key && (!found || c->key < best)) { best = c->key; found = 1; }
    return best;
}

/* =====================================================================
 * 4. SINGLY LINKED SORTED LIST
 * ===================================================================== */
typedef struct { SNode *head; } SLS;

static SLS sls_build(int *keys, int n) {
    /* build via insertion sort into the list (not timed) */
    SLS s; s.head = NULL;
    for (int i = 0; i < n; i++) {
        int key = keys[i];
        SNode *node = malloc(sizeof(SNode)); node->key = key;
        if (!s.head || s.head->key >= key) { node->next = s.head; s.head = node; }
        else {
            SNode *c = s.head;
            while (c->next && c->next->key < key) c = c->next;
            node->next = c->next; c->next = node;
        }
    }
    return s;
}
static void sls_freeS(SLS *s) {
    SNode *c = s->head;
    while (c) { SNode *t = c; c = c->next; free(t); }
}
static SNode *sls_search(SLS *s, int key) {          /* O(n) worst case */
    SNode *c = s->head;
    while (c) { if (c->key == key) return c; c = c->next; }
    return NULL;
}
static void sls_insert(SLS *s, int key) {             /* O(n): find sorted pos */
    SNode *node = malloc(sizeof(SNode)); node->key = key;
    if (!s->head || s->head->key >= key) { node->next = s->head; s->head = node; return; }
    SNode *c = s->head;
    while (c->next && c->next->key < key) c = c->next;
    node->next = c->next; c->next = node;
}
static void sls_delete(SLS *s, SNode *target) {        /* O(n): need prev */
    if (s->head == target) { s->head = target->next; free(target); return; }
    SNode *c = s->head;
    while (c->next != target) c = c->next;
    c->next = target->next;
    free(target);
}
static int sls_max(SLS *s) {                            /* O(n): walk to tail */
    SNode *c = s->head;
    while (c->next) c = c->next;
    return c->key;
}
static int sls_min(SLS *s) { return s->head->key; }      /* O(1) */
static int sls_succ(SNode *target) {                     /* O(1): next pointer */
    return target->next ? target->next->key : -1;
}
static int sls_pred(SLS *s, SNode *target) {              /* O(n): walk from head */
    if (s->head == target) return -1;
    SNode *c = s->head;
    while (c->next != target) c = c->next;
    return c->key;
}

/* =====================================================================
 * 5. DOUBLY LINKED UNSORTED LIST
 * ===================================================================== */
typedef struct DNode { int key; struct DNode *prev, *next; } DNode;
typedef struct { DNode *head, *tail; } DLU;

static DLU dlu_build(int *keys, int n) {
    DLU s; s.head = s.tail = NULL;
    for (int i = 0; i < n; i++) {
        DNode *node = malloc(sizeof(DNode));
        node->key = keys[i]; node->prev = NULL; node->next = s.head;
        if (s.head) s.head->prev = node;
        s.head = node;
        if (!s.tail) s.tail = node;
    }
    return s;
}
static void dlu_free(DLU *s) {
    DNode *c = s->head;
    while (c) { DNode *t = c; c = c->next; free(t); }
}
static DNode *dlu_search(DLU *s, int key) {          /* O(n) */
    DNode *c = s->head;
    while (c) { if (c->key == key) return c; c = c->next; }
    return NULL;
}
static void dlu_insert(DLU *s, int key) {             /* O(1): insert at head */
    DNode *node = malloc(sizeof(DNode));
    node->key = key; node->prev = NULL; node->next = s->head;
    if (s->head) s->head->prev = node;
    s->head = node;
    if (!s->tail) s->tail = node;
}
static void dlu_delete(DLU *s, DNode *target) {        /* O(1): direct unlink */
    if (target->prev) target->prev->next = target->next; else s->head = target->next;
    if (target->next) target->next->prev = target->prev; else s->tail = target->prev;
    free(target);
}
static int dlu_max(DLU *s) {                            /* O(n) */
    int m = s->head->key;
    for (DNode *c = s->head; c; c = c->next) if (c->key > m) m = c->key;
    return m;
}
static int dlu_min(DLU *s) {                             /* O(n) */
    int m = s->head->key;
    for (DNode *c = s->head; c; c = c->next) if (c->key < m) m = c->key;
    return m;
}
static int dlu_pred(DLU *s, DNode *target) {              /* O(n): unsorted scan */
    int key = target->key, best = -1, found = 0;
    for (DNode *c = s->head; c; c = c->next)
        if (c->key < key && (!found || c->key > best)) { best = c->key; found = 1; }
    return best;
}
static int dlu_succ(DLU *s, DNode *target) {               /* O(n) */
    int key = target->key, best = -1, found = 0;
    for (DNode *c = s->head; c; c = c->next)
        if (c->key > key && (!found || c->key < best)) { best = c->key; found = 1; }
    return best;
}

/* =====================================================================
 * 6. DOUBLY LINKED SORTED LIST
 * ===================================================================== */
typedef struct { DNode *head, *tail; } DLS;

static DLS dls_build(int *keys, int n) {
    DLS s; s.head = s.tail = NULL;
    for (int i = 0; i < n; i++) {
        int key = keys[i];
        DNode *node = malloc(sizeof(DNode)); node->key = key;
        if (!s.head || s.head->key >= key) {
            node->prev = NULL; node->next = s.head;
            if (s.head) s.head->prev = node;
            s.head = node;
            if (!s.tail) s.tail = node;
        } else {
            DNode *c = s.head;
            while (c->next && c->next->key < key) c = c->next;
            node->next = c->next; node->prev = c;
            if (c->next) c->next->prev = node; else s.tail = node;
            c->next = node;
        }
    }
    return s;
}
static void dls_free(DLS *s) {
    DNode *c = s->head;
    while (c) { DNode *t = c; c = c->next; free(t); }
}
static DNode *dls_search(DLS *s, int key) {           /* O(n) worst case */
    DNode *c = s->head;
    while (c) { if (c->key == key) return c; c = c->next; }
    return NULL;
}
static void dls_insert(DLS *s, int key) {              /* O(n): find sorted pos */
    DNode *node = malloc(sizeof(DNode)); node->key = key;
    if (!s->head || s->head->key >= key) {
        node->prev = NULL; node->next = s->head;
        if (s->head) s->head->prev = node;
        s->head = node;
        if (!s->tail) s->tail = node;
        return;
    }
    DNode *c = s->head;
    while (c->next && c->next->key < key) c = c->next;
    node->next = c->next; node->prev = c;
    if (c->next) c->next->prev = node; else s->tail = node;
    c->next = node;
}
static void dls_delete(DLS *s, DNode *target) {          /* O(1): direct unlink */
    if (target->prev) target->prev->next = target->next; else s->head = target->next;
    if (target->next) target->next->prev = target->prev; else s->tail = target->prev;
    free(target);
}
static int dls_max(DLS *s) { return s->tail->key; }       /* O(1): tail pointer */
static int dls_min(DLS *s) { return s->head->key; }       /* O(1) */
static int dls_succ(DNode *target) { return target->next ? target->next->key : -1; } /* O(1) */
static int dls_pred(DNode *target) { return target->prev ? target->prev->key : -1; } /* O(1) */

/* =====================================================================
 * TIMING HARNESS
 * ===================================================================== */
static void report(FILE *f, const char *structure, const char *op, int n, double t) {
    printf("%s,%s,%d,%.9f\n", structure, op, n, t);
    fprintf(f, "%s,%s,%d,%.9f\n", structure, op, n, t);
}

int main(void) {
    srand(42);
    FILE *f = fopen("results.csv", "w");
    fprintf(f, "structure,operation,n,time_seconds\n");
    printf("structure,operation,n,time_seconds\n");

    /* REPS: how many times a cheap, non-destructive op is repeated in a
     * tight loop so the total elapsed time rises above clock() noise.
     * Insert/Delete are measured as insert-then-delete-back pairs so the
     * structure size n stays constant across repetitions. */
    const int REPS = 300;

    for (int si = 0; si < NUM_SIZES; si++) {
        int n = SIZES[si];
        int missing_key = n * 4 + 100; /* guaranteed absent, forces worst case */

        double t_search[6] = {0}, t_insert[6] = {0}, t_delete[6] = {0},
               t_max[6] = {0}, t_min[6] = {0}, t_pred[6] = {0}, t_succ[6] = {0};

        for (int trial = 0; trial < TRIALS; trial++) {
            int *keys = gen_unique_keys(n);
            int probe_idx = rand() % n;      /* index into `keys` used to pick an existing key */
            int probe_key = keys[probe_idx];

            double t0, t1;

            /* ---------- 1. Unsorted array ---------- */
            { AU s = au_build(keys, n);
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += au_search(&s, missing_key);
              t1 = now_sec(); t_search[0] += (t1 - t0) / REPS;

              t0 = now_sec();
              for (int r = 0; r < REPS; r++) au_insert(&s, n * 4 + 900 + r);
              t1 = now_sec(); t_insert[0] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) au_delete_at(&s, s.n - 1);
              t1 = now_sec(); t_delete[0] += (t1 - t0) / REPS;

              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += au_max(&s);
              t1 = now_sec(); t_max[0] += (t1 - t0) / REPS;

              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += au_min(&s);
              t1 = now_sec(); t_min[0] += (t1 - t0) / REPS;

              int idx = au_search(&s, probe_key); if (idx < 0) idx = 0;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += au_pred(&s, idx);
              t1 = now_sec(); t_pred[0] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += au_succ(&s, idx);
              t1 = now_sec(); t_succ[0] += (t1 - t0) / REPS;
              au_free(&s);
            }

            /* ---------- 2. Sorted array ---------- */
            { AS s = as_build(keys, n);
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += as_search(&s, missing_key);
              t1 = now_sec(); t_search[1] += (t1 - t0) / REPS;

              t0 = now_sec();
              for (int r = 0; r < REPS; r++) as_insert(&s, n * 4 + 900 + r);
              t1 = now_sec(); t_insert[1] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) as_delete_at(&s, s.n - 1);
              t1 = now_sec(); t_delete[1] += (t1 - t0) / REPS;

              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += as_max(&s);
              t1 = now_sec(); t_max[1] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += as_min(&s);
              t1 = now_sec(); t_min[1] += (t1 - t0) / REPS;

              int idx = as_search(&s, probe_key); if (idx < 0) idx = 0;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += as_pred(&s, idx);
              t1 = now_sec(); t_pred[1] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += as_succ(&s, idx);
              t1 = now_sec(); t_succ[1] += (t1 - t0) / REPS;
              as_free(&s);
            }

            /* ---------- 3. Singly linked unsorted ---------- */
            { SLU s = slu_build(keys, n);
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += (long)slu_search(&s, missing_key);
              t1 = now_sec(); t_search[2] += (t1 - t0) / REPS;

              t0 = now_sec();
              for (int r = 0; r < REPS; r++) slu_insert(&s, n * 4 + 900 + r);
              t1 = now_sec(); t_insert[2] += (t1 - t0) / REPS;

              /* Delete worst case: target is the LAST node, forcing a full
               * traversal from head to find its predecessor (no prev ptr). */
              { double total = 0;
                for (int r = 0; r < REPS; r++) {
                    SNode *last = s.head;             /* untimed: locate target */
                    while (last->next) last = last->next;
                    double a = now_sec();
                    slu_delete(&s, last);              /* TIMED: O(n) unlink */
                    double b = now_sec();
                    total += b - a;
                    slu_insert(&s, n * 4 + 900 + r);   /* untimed: restore size */
                }
                t_delete[2] += total / REPS;
              }

              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += slu_max(&s);
              t1 = now_sec(); t_max[2] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += slu_min(&s);
              t1 = now_sec(); t_min[2] += (t1 - t0) / REPS;

              SNode *node = s.head;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += slu_pred(&s, node);
              t1 = now_sec(); t_pred[2] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += slu_succ(&s, node);
              t1 = now_sec(); t_succ[2] += (t1 - t0) / REPS;
              slu_free(&s);
            }

            /* ---------- 4. Singly linked sorted ---------- */
            { SLS s = sls_build(keys, n);
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += (long)sls_search(&s, missing_key);
              t1 = now_sec(); t_search[3] += (t1 - t0) / REPS;

              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sls_insert(&s, n * 4 + 900 + r);
              t1 = now_sec(); t_insert[3] += (t1 - t0) / REPS;

              /* Delete worst case: target is the LAST (largest-key) node,
               * forcing a full traversal from head to find its predecessor. */
              { double total = 0;
                for (int r = 0; r < REPS; r++) {
                    SNode *last = s.head;             /* untimed: locate target */
                    while (last->next) last = last->next;
                    double a = now_sec();
                    sls_delete(&s, last);              /* TIMED: O(n) unlink */
                    double b = now_sec();
                    total += b - a;
                    sls_insert(&s, n * 4 + 900 + r);   /* untimed: restore size */
                }
                t_delete[3] += total / REPS;
              }

              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += sls_max(&s);
              t1 = now_sec(); t_max[3] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += sls_min(&s);
              t1 = now_sec(); t_min[3] += (t1 - t0) / REPS;

              SNode *node = s.head;
              SNode *last_node = s.head;
              while (last_node->next) last_node = last_node->next; /* worst case for pred */
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += sls_pred(&s, last_node);
              t1 = now_sec(); t_pred[3] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += sls_succ(node);
              t1 = now_sec(); t_succ[3] += (t1 - t0) / REPS;
              sls_freeS(&s);
            }

            /* ---------- 5. Doubly linked unsorted ---------- */
            { DLU s = dlu_build(keys, n);
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += (long)dlu_search(&s, missing_key);
              t1 = now_sec(); t_search[4] += (t1 - t0) / REPS;

              t0 = now_sec();
              for (int r = 0; r < REPS; r++) dlu_insert(&s, n * 4 + 900 + r);
              t1 = now_sec(); t_insert[4] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) dlu_delete(&s, s.head);
              t1 = now_sec(); t_delete[4] += (t1 - t0) / REPS;

              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += dlu_max(&s);
              t1 = now_sec(); t_max[4] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += dlu_min(&s);
              t1 = now_sec(); t_min[4] += (t1 - t0) / REPS;

              DNode *node = s.head;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += dlu_pred(&s, node);
              t1 = now_sec(); t_pred[4] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += dlu_succ(&s, node);
              t1 = now_sec(); t_succ[4] += (t1 - t0) / REPS;
              dlu_free(&s);
            }

            /* ---------- 6. Doubly linked sorted ---------- */
            { DLS s = dls_build(keys, n);
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += (long)dls_search(&s, missing_key);
              t1 = now_sec(); t_search[5] += (t1 - t0) / REPS;

              t0 = now_sec();
              for (int r = 0; r < REPS; r++) dls_insert(&s, n * 4 + 900 + r);
              t1 = now_sec(); t_insert[5] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) dls_delete(&s, s.head);
              t1 = now_sec(); t_delete[5] += (t1 - t0) / REPS;

              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += dls_max(&s);
              t1 = now_sec(); t_max[5] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += dls_min(&s);
              t1 = now_sec(); t_min[5] += (t1 - t0) / REPS;

              DNode *node = s.head;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += dls_pred(node);
              t1 = now_sec(); t_pred[5] += (t1 - t0) / REPS;
              t0 = now_sec();
              for (int r = 0; r < REPS; r++) sink += dls_succ(node);
              t1 = now_sec(); t_succ[5] += (t1 - t0) / REPS;
              dls_free(&s);
            }

            free(keys);
        }

        const char *names[6] = {
            "UnsortedArray", "SortedArray",
            "SinglyLinkedUnsorted", "SinglyLinkedSorted",
            "DoublyLinkedUnsorted", "DoublyLinkedSorted"
        };
        for (int k = 0; k < 6; k++) {
            report(f, names[k], "Search", n, t_search[k] / TRIALS);
            report(f, names[k], "Insert", n, t_insert[k] / TRIALS);
            report(f, names[k], "Delete", n, t_delete[k] / TRIALS);
            report(f, names[k], "Max",    n, t_max[k] / TRIALS);
            report(f, names[k], "Min",    n, t_min[k] / TRIALS);
            report(f, names[k], "Predecessor", n, t_pred[k] / TRIALS);
            report(f, names[k], "Successor",   n, t_succ[k] / TRIALS);
        }
    }

    fclose(f);
    return 0;
}
