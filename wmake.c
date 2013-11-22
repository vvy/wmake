#include "wmake.h"
#define HASHMAP_SIZE 256

HMAP* hptr = NULL;

void free_vertex_s(HNODE* node)
{
    free(((vertex_s*)(node->val))->filename);
    if(((vertex_s*)(node->val))->command != NULL)
    {
        int i;
        for(i=0;(((vertex_s*)(node->val))->command)[i]!= NULL && i<MAXCOMMAND;i++)
            free((((vertex_s*)(node->val))->command)[i]);
        free(((vertex_s*)(node->val))->command);
    }
    adjlist_s *p,*pnext;
    p = ((vertex_s*)(node->val))->adj;
    pnext = p;
    while(p!=NULL)
    {
        pnext = p->next;
        free(p);
        p=pnext;
    }
}

time_t GetModifiedTimestamp(char *path)
{
    struct stat attr;
    if(stat(path,&attr) == -1)
        return 0;
    return attr.st_mtime;
}

void lprint(void *v)
{
	printf("%s ",((vertex_s* )v)->filename);
	return;
}

void display_ts(void *v)
{
	printf("  %s %ld\n",((vertex_s* )v)->filename,((vertex_s* )v)->timestamp);
	return;
}

void set_ts(void *node)
{
    ((vertex_s* )node)->timestamp = GetModifiedTimestamp(((vertex_s* )node)->filename);
	return;
}

void color_clear(void *node)
{
    ((vertex_s* )node)->color = WHITE;
	return;
}

void dfs(HMAP* hmp)
{
    hmap_traverse(hmp,color_clear,VAL);
    hmap_traverse(hmp,dfs_visit,VAL);
    return;
}

void dfs_visit(void *node)
{
    if(((vertex_s*)node)->color != WHITE)
        return;
    ((vertex_s*)node)->color = GRAY;
    adjlist_s *p = ((vertex_s*)node)->adj;
    while(p!=NULL)
    {
        if(p->v->color == WHITE)
            dfs_visit(p->v);
        else if(p->v->color == GRAY)
        { //CLRS  p545,p550
            printf("  [ERROR]input: a cycle exists.\n");
            exit(0);
        }
        p=p->next;
    }
    ((vertex_s*)node)->color = BLACK;
    //printf("found:%s\n",((vertex_s*)node)->filename);
    return;
}

void has_cycle(HMAP *h)
{
    dfs(h);
}

void show_conncetion(void *v)
{
    adjlist_s* p = ((vertex_s*)v)->adj;
    printf("%s(isbase:%d):",((vertex_s*)v)->filename,((vertex_s*)v)->isbase);
    while(p!=NULL)
    {
        printf(" %s %d[%ld]",p->v->filename,p->v->isbase,p->v->timestamp);
        p = p->next;
    }
    printf("\n");
    if(((vertex_s*)v)->isbase)
  	    return;
    int i;
    for(i=0; (((vertex_s*)v)->command[i]) !=NULL;i++)
        printf("%s",((vertex_s*)v)->command[i]);
}


int readfile(char *file)
{
    FILE *fp;
    char line[MAXLINE],nextline[MAXLINE];
    int pre_read = 0;
    fp = fopen(file,"r");
    if(fp == NULL)
    {
        perror(file);
        return 0;
    }

    hptr = hmap_create(HASHMAP_DEFAULT_SIZE,HASHMAP_DEFAULT_FACTOR,NULL,free_vertex_s);
    //line process
    do {
        if(pre_read == 0)
            fgets(line,MAXLINE,fp);
        else
            strncpy(line,nextline,MAXLINE);
        if(strcmp(line,"\n")!=0 && !feof(fp) && !(line[0] == ' '))
        {
            char *p = strtok(line,":");
            if(p[strlen(p)-1] == ' ') //dependency line
            {
                char *tfilename = malloc(sizeof(char) * (strlen(p)+1));
                strcpy(tfilename,p);
                tfilename[strlen(tfilename) -1] = '\0';
				//printf("%s\n",tfilename);
                
                //process next line(s)
                char **tcommands;
                tcommands = malloc(sizeof(char*) *MAXCOMMAND);
                int i;
                for(i=0;i<MAXCOMMAND;i++)
                {
                    fgets(nextline,MAXLINE,fp);
                    if(nextline[0] != '\t' || feof(fp))
                    {
                        pre_read = 1;
                        break;
                    }

                    tcommands[i] = malloc(sizeof(char) * (strlen(nextline)+1));
                    strcpy(tcommands[i],nextline);
                }
                tcommands[i] = NULL;

                if(hmap_contain(hptr,tfilename)) 
                {
                    vertex_s *vtemp = hmap_get(hptr,tfilename);
                    if(vtemp->isbase == 1)
                    {
                        vtemp->isbase = 0;
                        vtemp->command = tcommands;
                    }
                    else
                    {
                        printf("[error]the same filename:%s\n",tfilename);
                        exit(0);
                    }
                }
                else
                {
                    vertex_s *vtemp = malloc(sizeof(vertex_s));
                    vtemp->filename = tfilename;
                    vtemp->isbase = 0;
                    vtemp->command = tcommands;
                    vtemp->adj = NULL;
                    hmap_put(hptr,tfilename,vtemp);
                }

                //process dependency

                vertex_s *vptr = hmap_get(hptr,tfilename);
                if(hptr->default_node == NULL)
                    hmap_set_default_node(hptr,(void*)vptr);
                while( (p=strtok(NULL," ")) != NULL && *p!= '\n')
                {
                    vertex_s* vtemp;
                    adjlist_s *node;
                    char *strtemp = NULL;
                    char *stemp;
                    if(p[strlen(p)-1] == '\n')
                    {
                        char *strtemp = malloc(sizeof(char) * (strlen(p)+1));
                        strcpy(strtemp,p);
                        strtemp[strlen(strtemp)-1] = '\0';
                        p = strtemp;
                    }

                    if(!hmap_contain(hptr,p))
                    {
                        stemp = malloc(sizeof(char) *(strlen(p) +1 ));
                        strcpy(stemp,p);
                        vtemp = malloc(sizeof(vertex_s));
                        vtemp->filename = stemp;
                        vtemp->isbase = 1;
                        vtemp->command = NULL;
                        vtemp->adj = NULL;
                        hmap_put(hptr,stemp,vtemp);
                    }
                    vtemp = hmap_get(hptr,p);
                    node = malloc(sizeof(adjlist_s));
                    node->v = vtemp;
                    node->next = NULL;
                    if(vptr->adj == NULL)
                    {
                        vptr->adj = node;
                    }
                    else
                    {
                        adjlist_s* t_vptr = vptr->adj;
                        while(t_vptr->next != NULL)
                            t_vptr = t_vptr->next;
                        t_vptr->next = node;

                    }
                    free(strtemp);
                }
            }
        }
        else 
            pre_read = 0;
    } while(!feof(fp));

    hmap_traverse(hptr,set_ts,VAL);
#ifdef DEBUG
    hmap_traverse(hptr,show_conncetion,VAL);
#endif
    fclose(fp);
    return 0;
}

int wmake(char *s)
{
    if(s == NULL)
    {
        s = ((vertex_s*)(hptr->default_node))->filename;
    }
    else if(!hmap_contain(hptr,s))
    {
        printf("wmake: ***no rules to make target '%s'\n.",s);
        return -1;
    }
    //printf("\nwmake:%s\n",s);
    vertex_s* v = hmap_get(hptr,s);
    hmap_traverse(hptr,color_clear,VAL);
    time_t ttemp = v->timestamp;
    wmake_visit(v);
    if(ttemp == v->timestamp && ttemp!=0) //not 'clean',etc
        printf("wmake: \"%s\" is the newest.\n",v->filename);
    return 0;
}

int wmake_visit(vertex_s *node) //dfs
{
    if(node->color != WHITE) //NO CYCLE,BLACK ONLY.
        return node->timestamp;
    time_t tempstamp,maxsucstamp = 0;
    node->color = GRAY;
    adjlist_s *p = node->adj;
    while(p!=NULL)
    {
        tempstamp = wmake_visit(p->v);
        if(tempstamp == 0 && p->v->isbase == 1)
        {
            printf("make: *** no rules to make target “%s” of “%s”.\n",node->filename,p->v->filename);
            exit(-1);
        }
        if(tempstamp > maxsucstamp)
            maxsucstamp = tempstamp;
        p=p->next;
    }
    if(maxsucstamp > node->timestamp)
    {
        int i;
        for(i=0;(node->command)[i]!=NULL;i++)
        {
            execute_command((node->command)[i]);
        }
        node->timestamp = GetModifiedTimestamp(node->filename);
    }
    else if(maxsucstamp == node->timestamp && node->timestamp == 0)
    //no dependency line : clean,etc
    {
        int i;
        for(i=0;(node->command)[i]!=NULL;i++)
        {
            execute_command((node->command)[i]);
        }
    }
    //else:base file
    node->color = BLACK;
    //printf("found:%s\n",((vertex_s*)node)->filename);
    return node->timestamp;
}

int execute_command(char *s)
{
    char *p = strtok(s,"\t");
    int status;
    printf("%s",p);
    if(fork()!=0)
        waitpid(-1,&status,0);
    else
    {
        system(p);
        exit(0);
    }
}

int main(int argc, char **argv)
{
    readfile("wmakefile");
    has_cycle(hptr);
    if(argc == 1)
        wmake(NULL);
    else if(argc == 2)
        wmake(argv[1]);
    return 0;
}
