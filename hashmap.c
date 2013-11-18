/*
 * =====================================================================================
 *       Filename:  hashmap.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年11月16日 18时59分48秒
 *         Author:  wuyue (wy), vvuyve@gmail.com
 *        Company:  UESTC
 * =====================================================================================
 */
#include "hashmap.h"

unsigned hash_default_func(char *s,unsigned size)
{
    unsigned h = 0;
    while(*s != '\0')
    {
        h += *s;
        s++;
    }
    return h%size;
}

HMAP* hmap_create(unsigned size, double factor,H_FUNC hfunc,FREE_FUNC ffunc)
{
    HMAP* hptr = malloc(sizeof(HMAP));
    if(hptr==NULL)
    {
        perror("Error: malloc in hashmap failed.\n");
        exit(-1);
    }

    if(size == 0)
        hptr->size = HASHMAP_DEFAULT_SIZE;
    else
        hptr->size = size;

    if(factor <= 0)
        hptr->factor = HASHMAP_DEFAULT_FACTOR;
    else
        hptr->factor = factor;

    hptr->num = 0;
    hptr->entry = malloc(sizeof(HNODE*) * hptr->size);
    if(hptr->entry == NULL)
    {
        perror("Error: malloc for hashmap's entries failed.\n");
        exit(-1);
    }
    int i;
    for(i=0;i < hptr->size;i++)
        (hptr->entry)[i] = NULL;
    if(hfunc == NULL)
        hptr->hash_f = hash_default_func;
    else
        hptr->hash_f = hfunc;
    
    hptr->free_f = ffunc;
    hptr->default_node = NULL;
    return hptr;
}

void hmap_set_default_node(HMAP* hptr,void *node)
{
    hptr->default_node = node;
    return;
}

int hmap_num(HMAP* hptr)
{
    if(hptr == NULL)
    {
        perror("Error: hashmap is NULL.\n");
        return 0;
    }
    return hptr->num;
}

int hmap_contain(HMAP* hptr,char *s)
{
    if(hptr == NULL)
    {
        perror("Error: hashmap is NULL.\n");
        return 0;
    }
    int index = (hptr->hash_f)(s,hptr->size);
    HNODE *head = (hptr->entry)[index];
    while(head!= NULL)
    {
        if(strcmp(head->key,s)!= 0)
            head = head->next;
        else
            break;
    }
    if(head == NULL)
        return 0;
    else 
        return 1;
}

void* hmap_get(HMAP* hptr,char *s)
{
    if(hptr == NULL)
    {
        perror("Error: hashmap is NULL.\n");
        return NULL;
    }
    int index = (hptr->hash_f)(s,hptr->size);
    HNODE *head = (hptr->entry)[index];
    while(head!= NULL)
    {
        if(strcmp(head->key,s)!= 0)
            head = head->next;
        else
            break;
    }
    if(head == NULL)
        return NULL;
    else 
        return head->val;
}

int hmap_put(HMAP* hptr,char *key,void *val)
{
    int index = (hptr->hash_f)(key,hptr->size);
    HNODE *head = (hptr->entry)[index];
    if(head == NULL)
    {
        (hptr->entry)[index] = malloc(sizeof(HNODE));
        HNODE *p = (hptr->entry)[index];
        p->next = NULL;
        p->key = malloc(sizeof(char) * (strlen(key)+1));
        strcpy(p->key,key);
        p->val = val;
        p->hval = index;
        hptr->num++;
        return 1;
    }

    while(head->next != NULL && strcmp(head->key,key)!=0)
        head = head->next;
    if(strcmp(head->key,key) == 0)
        head->val = val;//update
    else    //new
    {
        HNODE *p = malloc(sizeof(HNODE));
        p->next = NULL;
        p->key = malloc(sizeof(char) * (strlen(key)+1));
        strcpy(p->key,key);
        p->val = val;
        p->hval = index;
        head->next = p;
        hptr->num++;
    }
    return 0;
}

int hmap_remove(HMAP* hptr,char *key)
{
    int index = (hptr->hash_f)(key,hptr->size);
    HNODE *p,*prev;
    p = (hptr->entry)[index];
    if(p == NULL)
    {
        perror("Error: Not exists.\n");
        return 0;
    }
    prev = p;
    while(p->next != NULL && strcmp(p->key,key)!=0)
    {
        prev = p;
        p = p->next;
    }

    if(strcmp(p->key,key) == 0)
    {
        if(p == prev)
            (hptr->entry)[index] = p->next;
        else
            prev->next = p->next;
        if(hptr->free_f != NULL)
            (hptr->free_f)(p);
        free(p);
        hptr->num--;
    }
    else    
    {
        perror("Error: Not exists.\n");
        return 0;
    }
}

int hmap_free(HMAP* hptr)
{
    int i;
    HNODE *p,*pnext;
    for(i=0;i<hptr->size;i++)
    {
        p = (hptr->entry)[i];
        while(p!=NULL)
        {
            pnext = p->next;
            free(p->key);
            if(hptr->free_f != NULL)
                (hptr->free_f)(p->val);
            free(p);
            p = pnext;
        }
    }
    free(hptr->entry);
    free(hptr);
}

void hmap_display(HMAP *hptr)
{
    int i;
    HNODE *p;
	printf("size:%d\n",hptr->size);
    for(i=0;i<hptr->size;i++)
    {
        p = (hptr->entry)[i];
        if(p == NULL)
            continue;
        printf("%d:",i);
        while(p!=NULL)
        {
            printf("%s  ",p->key);
            p=p->next;
        }
        printf("\n");
    }
}

void hmap_traverse(HMAP *hptr,HNODE_FUNC func,int usage)
{
    int i;
    HNODE *p;
    for(i=0;i<hptr->size;i++)
    {
        p = (hptr->entry)[i];
        if(p == NULL)
            continue;
        //printf("%d:",i);
        while(p!=NULL)
        {
            if(usage == KEY)
                func(p);
            else if(usage == VAL)
                func(p->val);
            p=p->next;
        }
        //printf("\n");
    }
}

