CFLAGS=-std=c99 -Wall -Wextra -Werror  -lm

debug: src/cluster.c
	cc $(CFLAGS) -g -o cluster src/cluster.c 

release: src/cluster.c
	cc $(CFLAGS) -DNDEBUG -o cluster src/cluster.c 
