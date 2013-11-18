/*
 * =====================================================================================
 *       Filename:  hashmap.h
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年11月16日 19时00分06秒
 *         Author:  wuyue (wy), vvuyve@gmail.com
 *        Company:  UESTC
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#ifndef HASHMAP_STRUCT_PROTO
#define HASHMAP_STRUCT_PROTO
#define HASHMAP_DEFAULT_SIZE 256
#define HASHMAP_DEFAULT_FACTOR 0.75
#define KEY 0
#define VAL 1
typedef unsigned (*H_FUNC)(char *,unsigned);
typedef struct hash_node
{
    struct hash_node *next;
    char *key;
    void *val;
    unsigned hval;
} HNODE;

typedef void (*FREE_FUNC)(HNODE *);
typedef void (*HNODE_FUNC)(void *);

typedef struct hashmap
{
    int size;
    int num;
    double factor;
    HNODE** entry;
    void* default_node;
    H_FUNC hash_f;
    FREE_FUNC free_f;
} HMAP;


extern HMAP* hmap_create(unsigned , double,H_FUNC,FREE_FUNC);
extern void  hmap_set_default_node(HMAP*,void *);
extern int   hmap_num(HMAP*);
extern int   hmap_put(HMAP*,char *,void *);
extern void* hmap_get(HMAP*,char *);
extern int   hmap_contain(HMAP*,char *);
extern int   hmap_remove(HMAP*,char *);
extern int   hmap_free(HMAP*);
extern void  hmap_display(HMAP*);
extern void  hmap_traverse(HMAP *,HNODE_FUNC ,int);
#endif
