EXEC=project2
CFLAGS = -c -O3 -Wall
LIBS = storage.o
PIPES = pipe_in pipe_out
TEST_DIR_1 = tests_public
OTHERS = storage.h

all: $(EXEC)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

$(EXEC): $(EXEC).o $(LIBS)
	gcc $(EXEC).o $(LIBS) -o $(EXEC)

clean:
	rm -f *.o $(EXEC) $(LIBS) $(PIPES)

zip:
	zip project2.zip README.txt project2.c Makefile storage.c storage.h

tar_tests:
	tar -cvf project2_dist.tar $(TEST_DIR_1) $(OTHERS)

install:
	cp project2_dist.tar ~/notepad/classes/cs3113/export/projects

tar_release:
	tar -cvf project2_release.tar Makefile *.h *.c

install_release:
	cp project2_release.tar ~/notepad/classes/cs3113/export/projects

pipes:
	mkfifo $(PIPES)
