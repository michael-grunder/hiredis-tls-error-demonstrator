# hiredis-tls-error-demonstrator
Small repository to reproduce the issue

## Must be on macOS and using redis-server >= 7.0.0 (I am using 7.0.2)

## Steps to reproduce the error
```bash
redis-server redis/redis.conf
make 
./trigger-tls-error
```
