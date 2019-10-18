//Camron Bartlow Project 2

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "storage_remote.h"
#include "storage_common.h"
#include "comm.h"

/**
   initialize the storage

   Open the two fds (in and out), wait for an init message, initialize the local storage
 */
STORAGE * init_storage(char * name)
{
  // Create space for the STORAGE object
  STORAGE *s = malloc(sizeof(STORAGE));
  HEADER header;
  HEADER head_out;

  s->fd_to_storage = open(PIPE_NAME_TO_STORAGE, O_WRONLY);
  s->fd_from_storage = open(PIPE_NAME_FROM_STORAGE, O_RDONLY);

  head_out.type = INIT_CONNECTION;
  head_out.len_message = strlen(name)+1;
  head_out.location = -1;
  head_out.len_buffer = 128;

  int headfoob = write(s->fd_to_storage, &head_out, sizeof(HEADER));
  int foob = write(s->fd_to_storage, name, head_out.len_message);

  read(s->fd_from_storage, &header, sizeof(HEADER));
  /*if (header.type != ACKNOWLEDGE) {
    fprintf(stderr, "Unexpected header\n");
    return NULL;
  }*/

  // All okay
  return s;
};

/**
   Shut down the connection

   Tell the server to shut down
 */
int close_storage(STORAGE *storage)
{
  // Create the shutdown message
  HEADER header;
  HEADER header_out;
  header_out.type = SHUTDOWN;
  header_out.len_message = 0;
  header_out.location = -1;
  header_out.len_buffer = -1;

  int foob = write(storage->fd_to_storage, &header_out, sizeof(HEADER));

  read(storage->fd_from_storage, &header, sizeof(HEADER));
  if (header.type != ACKNOWLEDGE) {
    fprintf(stderr, "Unexpected header received");
    return -1;
  }

  // Free the storage struction
  free(storage);

  // Done
  return(0);
}

/**
   read bytes from the storage
 */
int get_bytes(STORAGE *storage, unsigned char *buf, int location, int len)
{
  HEADER head_in;
  HEADER header;
  header.type = READ_REQUEST;
  header.len_message = 0;
  header.location = location;
  header.len_buffer = len;

  int foob = write(storage->fd_to_storage, &header, sizeof(HEADER));

  read(storage->fd_from_storage, &head_in, sizeof(HEADER));
  //Wait for DATA message back
  if (head_in.type != DATA) {
    fprintf(stderr, "Unexpected header");
    return(-1);
  }
  read(storage->fd_from_storage, buf+location, head_in.len_message);

  // Success
  return(head_in.len_message);
};


/**
   Write bytes to the storage

   Send the write request message + the data
 */
int put_bytes(STORAGE *storage, unsigned char *buf, int location, int len)
{
  HEADER header;
  HEADER head;
  head.type = WRITE_REQUEST;
  head.len_message = len;
  head.location = location;
  head.len_buffer = len;

  int foob = write(storage->fd_to_storage, &head, sizeof(HEADER));
  foob = write(storage->fd_to_storage, buf+location, len);

  read(storage->fd_from_storage, &header, sizeof(HEADER));
  if (header.type != ACKNOWLEDGE) {
    fprintf(stderr, "Unexpected header\n");
    return(-1);
  }
  // Success
  return(len);
};

