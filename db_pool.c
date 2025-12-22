#include "db_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

DBPool* init_pool(const char *conninfo) {
    DBPool *pool = malloc(sizeof(DBPool));

    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->cond, NULL);

    for (int i = 0; i < MAX_POOL_SIZE; i++) {
        pool->connections[i] = PQconnectdb(conninfo);

        if (PQstatus(pool->connections[i]) != CONNECTION_OK) {
            fprintf(stderr, "Connection %d failed: %s\n",
                    i, PQerrorMessage(pool->connections[i]));
            exit(1);
        }

        pool->available[i] = 1;
    }
    return pool;
}

PGconn* get_connection(DBPool *pool) 
{
    pthread_mutex_lock(&pool->lock);

    while (1) {
        for (int i = 0; i < MAX_POOL_SIZE; i++) {
            if (pool->available[i]) {
                pool->available[i] = 0;
                pthread_mutex_unlock(&pool->lock);
                return pool->connections[i];
            }
        }
        pthread_cond_wait(&pool->cond, &pool->lock);
    }
}

void release_connection(DBPool *pool, PGconn *conn) {
    pthread_mutex_lock(&pool->lock);

    for (int i = 0; i < MAX_POOL_SIZE; i++) {
        if (pool->connections[i] == conn) {
            pool->available[i] = 1;
            break;
        }
    }

    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->lock);
}

void destroy_pool(DBPool *pool) {
    for (int i = 0; i < MAX_POOL_SIZE; i++) {
        PQfinish(pool->connections[i]);
    }

    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cond);
    free(pool);
}
