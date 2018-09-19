.PHONY: clean fork libev

clean:
	@rm -f server_fork server_libev

fork: server_fork.c util.*
	@gcc -o server_fork server_fork.c util.c

libev: server_libev.c router.* util.*
	@gcc -o server_libev server_libev.c router.c util.c -lev
