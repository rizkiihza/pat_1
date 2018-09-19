.PHONY: clean fork libev

clean:
	@rm -f server_fork server_libev

fork :	server_fork.c
	@gcc -o server_fork server_fork.c

libev : server_libev.c
	@gcc -o server_libev server_libev.c -lev
