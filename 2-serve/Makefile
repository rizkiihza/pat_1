.PHONY: all clean run

all: run

clean:
	@rm -f server

run: server
	@./server

server: server.c router.* util.*
	@gcc -o server server.c router.c util.c -lev
