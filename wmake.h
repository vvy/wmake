#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "hashmap.h"

#define MAXLINE 4096
#define MAXCOMMAND 20

#ifndef STRUCT_VERTEX_T
#define STRUCT_VERTEX_T
#define WHITE 0
#define GRAY  1
#define BLACK 2
struct vertex_t{
    char*             filename;
    char**            command;
    time_t            timestamp;
    int               isbase;
	int               color;
    struct adjlist_t *adj;
};

struct adjlist_t{
    struct vertex_t *v;
    struct adjlist_t* next;
};

typedef struct vertex_t vertex_s;
typedef struct adjlist_t adjlist_s;
#endif

int readfile(char *);
void lprint(void *v);
void display_st(void *v);
void color_clear(void *v);
void dfs(HMAP*);
void dfs_visit(void *node);
void has_cycle(HMAP*);
int type_process(void);
int fmake(vertex_s *v);
int fmake_visit(vertex_s *);
time_t GetModifiedTimestamp(char *path);
int wmake(char *);
int wmake_visit(vertex_s *);
int execute_command(char*);
