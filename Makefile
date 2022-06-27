all:
	cc -Wall -O0 -o trigger-tls-error trigger-tls-error.c -lhiredis -lhiredis_ssl
