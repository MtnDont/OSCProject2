all: project2 server

project2: project2.c
	gcc project2.c storage_remote.c -o project2

server: server.c
	gcc server.c storage.c -o server

clean:
	rm -f *.o project2
        rm -f *.bin server
        rm -f pipe_in pipe_out

zip:
	zip project2.zip README.txt *.c *.h Makefile

pipes:
	mkfifo pipe_in pipe_out
