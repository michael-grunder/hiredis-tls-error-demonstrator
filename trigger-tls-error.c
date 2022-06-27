#include <hiredis/hiredis.h>
#include <hiredis/hiredis_ssl.h>
#include <assert.h>
#include <stdlib.h>

#define REDIS_HOST "127.0.0.1"
#define REDIS_PORT 56443

#define CA_CERT "redis/ca.crt"
#define SSL_CERT "redis/redis.crt"
#define SSL_KEY "redis/redis.key"

#define N 10000

int main(void) {
    redisSSLContext *sctx;
    redisContext *c;
    redisReply *r;

    sctx = redisCreateSSLContext(CA_CERT, NULL, SSL_CERT, SSL_KEY, NULL, NULL);
    assert(sctx != NULL);

    c = redisConnect(REDIS_HOST, REDIS_PORT);
    assert(c != NULL && c->err == 0);

    assert(redisInitiateSSLWithContext(c, sctx) == REDIS_OK);

    for (size_t i = 0; i < 500; i++) {
        assert((r = redisCommand(c, "RPUSH mylist element:%d", i)) != NULL);
        freeReplyObject(r);
    }

    for (size_t i = 0; i < N; i++) {
        assert(redisAppendCommand(c, "LRANGE mylist 0 499") == REDIS_OK);
    }

    for (size_t i = 0; i < N; i++) {
        /* Bug will happen here at some point in the loop */
        if (redisGetReply(c, (void**)&r) != REDIS_OK || c->err != 0 ||
            r->type != REDIS_REPLY_ARRAY || r->elements != 500)
        {
            fprintf(stderr, "[%zu] redisContext error: %s (%d)\n", i, c->errstr, c->err);
            exit(1);
        }

        freeReplyObject(r);
    }
}
