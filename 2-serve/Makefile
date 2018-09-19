.PHONY: all clean run

all: server

clean:
	@rm -f server

server: server.c router.* util.*
	@gcc -o server server.c router.c util.c -lev
