#include "db_pool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

DBPool *pool;

void* worker(void *arg) {
    int *thread_id = (int*)arg;
    printf("Thread ID: %d\n", *thread_id);
    PGconn *conn = get_connection(pool);

    printf("Thread %ld acquired connection\n", pthread_self());

    PGresult *res = PQexec(conn, "SELECT now()");
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        printf("Thread %ld result: %s\n",
               pthread_self(), PQgetvalue(res, 0, 0));
    }
    PQclear(res);

    sleep(1);  // simulate DB work

    release_connection(pool, conn);
    printf("Thread %ld released connection\n", pthread_self());

    return NULL;
}

int main() {
    const char *conninfo =
        "host=localhost dbname=testdb user=testuser password=testpass";

    pool = init_pool(conninfo);

    pthread_t threads[10];
    for (int i = 0; i < 10; i++) {
            int *id = malloc(sizeof(int));
            *id = i;
        pthread_create(&threads[i], NULL, worker, id);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    destroy_pool(pool);
    return 0;
}
