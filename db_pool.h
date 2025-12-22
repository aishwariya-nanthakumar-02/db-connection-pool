#ifndef DB_POOL_H
#define DB_POOL_H

#include <libpq-fe.h>
#include <pthread.h>

#define MAX_POOL_SIZE 5

typedef struct {
    PGconn *connections[MAX_POOL_SIZE];
    int available[MAX_POOL_SIZE];
    pthread_mutex_t lock;
    pthread_cond_t cond;
} DBPool;

DBPool* init_pool(const char *conninfo);
PGconn* get_connection(DBPool *pool);
void release_connection(DBPool *pool, PGconn *conn);
void destroy_pool(DBPool *pool);

#endif
