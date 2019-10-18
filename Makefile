all: project2 server pipes

.c.o:
	gcc -c -O3 -Wall $< -o $@

project2: project2.o storage_remote.o
	gcc project2.o storage_remote.o -o project2

server: server.o storage.o
	gcc server.o storage.o -o server

clean:
	rm -f *.o project2 server pipe_in pipe_out

zip:
	zip project2.zip README.txt *.c *.h Makefile

pipes:
	mkfifo pipe_in pipe_out
