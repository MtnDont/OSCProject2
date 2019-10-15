//Camron Bartlow Project 2

#include "storage.h"
#include "comm.h"
#include "storage_common.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 200

int main(int argc, char** argv) {
  unsigned char buffer[BUFSIZE];
  HEADER header;
  HEADER header_out;
  int ret;
  STORAGE *storage;

  int fd_out;
  int fd_in;

  // Loop forever (break out with SHUTDOWN)
  while(1) {
    fprintf(stderr, "Waiting for connection with client...\n");

    // Open to_storage pipe
    fd_in = open("pipe_in", O_RDONLY);
    fd_out = open("pipe_out", O_WRONLY);
    if (fd_in < 0 || fd_out < 0) {
      perror("Error opening pipes");
      exit(-1);
    }

    while (header.type != INIT_CONNECTION)
      read(fd_in, header, sizeof(header));
    while(header.type != SHUTDOWN) {
      read(fd_in, header, sizeof(header));

      if (header.type == WRITE_REQUEST) {
        write();
      }
      else if (header.type == READ_REQUEST) {
        header_out.type = DATA;
        write(fd_out, header_out, sizeof(header_out));
      }
    }

    // We broke out because of a disconnection: clean up
    fprintf(stderr, "Closing connection\n");
    close(fd_in);
    close(fd_out);
    close_storage(storage);
  }

  // Should never reach here
  return(0);
}
