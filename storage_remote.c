//Camron Bartlow Project 2

#include <string.h>
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
  header.type = SHUTDOWN;
  header.len_message = 0;
  header.location = -1;
  header.len_buffer = -1;

  write(storage.fd_to_storage, header, sizeof(header));

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
  HEADER header;
  header.type = READ_REQUEST;
  header.len_message = len;
  header.location = location;
  header.len_buffer = buf;

  write(storage.fd_to_storage, header, sizeof(header));
  //Wait for DATA message back
  while (header.type != DATA) {
    read();
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



  // Success
  return(len);
};

