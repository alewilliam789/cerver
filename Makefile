base_flags = -Wall -Wextra -Werror -pedantic
headers = -isystem./include


test : 
	gcc $(headers) $(base_flags) -g -c test.c server.c request.c handler.c builder.c
	gcc -o a.out test.o server.o request.o handler.o builder.o

library :
	make clean
	- rm ../lib/libcerver.so ../lib/include/http.h ../lib/include/builder.h
	gcc $(headers) $(base_flags) -c -fPIC server.c request.c handler.c builder.c
	gcc -shared -o libcerver.so server.o request.o handler.o builder.o
	- rm	server.o request.o handler.o builder.o
	mv ./libcerver.so ../lib/
	cp ./include/http.h ../lib/include
	cp ./include/builder.h ../lib/include



json :
	- rm compile_commands.json
	bear -- make test
	- rm a.out

debug : a.out
	gdb a.out

clean : 
	-rm *.o
	- rm a.out
