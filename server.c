//Camron Bartlow Project 2

#include "storage.h"
#include "comm.h"
#include "storage_common.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 200

void sendAcknowledge(HEADER* head) {
  head->type = ACKNOWLEDGE;
  head->len_message = 0;
  head->location = -1;
  head->len_buffer = -1;
}

int main(int argc, char** argv) {
  unsigned char buffer[BUFSIZE] = { 0 };
  HEADER header;
  HEADER header_out;
  HEADER head_init;
  int ret;
  STORAGE *storage;

  for (int i = 0; i < BUFSIZE; i++) {
    buffer[i] = 0;
  }

  int fd_out;
  int fd_in;

  // Loop forever (break out with SHUTDOWN)
  while(1) {
    fprintf(stderr, "Waiting for connection with client...\n");

    // Open to_storage pipe
    fd_in = open(PIPE_NAME_TO_STORAGE, O_RDONLY);
    fd_out = open(PIPE_NAME_FROM_STORAGE, O_WRONLY);

    read(fd_in, &head_init, sizeof(HEADER));
    if (head_init.type != INIT_CONNECTION) {
      fprintf(stderr, "Unexpected header\n");
    }

    read(fd_in, buffer, head_init.len_buffer);

    sendAcknowledge(&header_out);
    write(fd_out, &header_out, sizeof(HEADER));

    storage = init_storage(buffer);

    //Wait for shutdown message
    while(1) {
      //Read pipe in
      read(fd_in, &header, sizeof(HEADER));

      if (header.type == WRITE_REQUEST) {
        //Write to file from HEADER parsed from pipe_in
        header_out.type = ACKNOWLEDGE;
        read(fd_in, buffer, header.len_buffer);

        put_bytes(storage, buffer, header.location, header.len_buffer);

        int foob = write(fd_out, &header_out, sizeof(HEADER));
      }
      else if (header.type == READ_REQUEST) {
        int ret = get_bytes(storage, buffer, header.location, header.len_buffer);

        //Generate and send HEADER
        header_out.type = DATA;
        header_out.len_message = ret;
        header_out.len_buffer = ret;
        int foob = write(fd_out, &header_out, sizeof(HEADER));

        int ar = write(fd_out, buffer, header_out.len_buffer);
      }
      else if (header.type == SHUTDOWN) {
        sendAcknowledge(&header_out);
        int foob = write(fd_out, &header_out, sizeof(HEADER));
        sleep(1);
        break;
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
