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

  int fdin = open(PIPE_NAME_TO_STORAGE, O_WRONLY);
  int fdout = open(PIPE_NAME_FROM_STORAGE, O_RDONLY);

  fprintf(stderr, "Pipes open\n");

  if (fdin <= 0 || fdout <= 0) {
    fprintf(stderr, "Unable to open %s\n", name);
    return NULL;
  }
  s->fd_to_storage = fdin;
  s->fd_from_storage = fdout;

  //head_out = {INIT_CONNECTION, sizeof(name)/sizeof(char), -1, sizeof(name)};
  head_out.type = INIT_CONNECTION;
  head_out.len_message = sizeof(name)/sizeof(char);
  head_out.location = -1;
  head_out.len_buffer = sizeof(name);


  write(fdin, &head_out, sizeof(HEADER));
  write(fdin, name, sizeof(name));

  read(fdout, &header, sizeof(HEADER));
  if (header.type != ACKNOWLEDGE) {
    fprintf(stderr, "Unexpected header\n");
    return NULL;
  }

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

  write(storage->fd_to_storage, &header_out, sizeof(HEADER));

  read(storage->fd_from_storage, &header, sizeof(HEADER));
  if (header.type != ACKNOWLEDGE) {
    fprintf(stderr, "Unexpected header received");
    return -1;
  }

  close(storage->fd_to_storage);
  close(storage->fd_from_storage);

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
  header.len_message = len;
  header.location = location;
  header.len_buffer = sizeof(buf);

  write(storage->fd_to_storage, &header, sizeof(HEADER));

  read(storage->fd_from_storage, &head_in, sizeof(HEADER));
  //Wait for DATA message back
  if (head_in.type != DATA) {
    fprintf(stderr, "Unexpected header");
    return(-1);
  }

  // Success
  return(len);
};


/**
   Write bytes to the storage

   Send the write request message + the data
 */
int put_bytes(STORAGE *storage, unsigned char *buf, int location, int len)
{
  HEADER header;
  HEADER head = {WRITE_REQUEST, sizeof(buf)/sizeof(char), location, len};
  /*head.type = WRITE_REQUEST;
  head.len_message = sizeof(buf)/sizeof(char);
  head.location = location;
  head.len_buffer = len;*/

  write(storage->fd_to_storage, &head, sizeof(HEADER));
  write(storage->fd_to_storage, buf, sizeof(buf));

  read(storage->fd_from_storage, &header, sizeof(HEADER));
  while (header.type != ACKNOWLEDGE) {
    read(storage->fd_from_storage, &header, sizeof(HEADER));
  }
  // Success
  return(len);
};

