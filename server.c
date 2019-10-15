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
    fd_in = open("pipe_in", RDONLY);
    fd_out = open("pipe_out", WRONLY);

    read(fd_in, header, sizeof(header));
    while (header.type != INIT_CONNECTION) {
      read(fd_in, header, sizeof(header));
    }
    read(fd_in, buffer, header.len_buffer);
    storage = init_storage(buffer);

    //Wait for shutdown message
    while(header.type != SHUTDOWN) {
      //Read pipe in
      read(fd_in, header, sizeof(header));

      if (header.type == WRITE_REQUEST) {
        //Generate and send HEADER
        header_out.type = ACKNOWLEDGE;
        header_out.len_message = 0;
        header_out.location = -1;
        header_out.len_buffer = -1;
        write(fd_out, header_out, sizeof(header_out));

        //Write to file from HEADER parsed from pipe_in
        read(fd_in, buffer, sizeof(header.len_buffer));
        //write(storage->fd, buffer, header.len_buffer);
        put_bytes(storage, buffer, header.location, header.len_message);
      }
      else if (header.type == READ_REQUEST) {
        //Generate and send HEADER
        
        header_out.type = DATA;
        header_out.len_message = 0; //This needs to be length of ret
        header_out.location = -1;
        header_out.len_buffer = -1;
        write(fd_out, header_out, sizeof(header_out));

        int ret = get_bytes(storage, buffer, header.location, header.len_message);
        write(fd_out, ret, sizeof(ret));
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
