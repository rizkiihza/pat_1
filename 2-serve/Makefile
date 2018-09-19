.PHONY: clean fork libev

clean:
	@rm -f server_fork server_libev

fork: server_fork.c util.*
	@gcc -o server_fork server_fork.c util.*

libev: server_libev.c util.*
	@gcc -o server_libev server_libev.c util.* -lev
