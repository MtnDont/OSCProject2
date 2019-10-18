Camron Bartlow Project 2

This program holds a 128 byte buffer and stores it within a file
that is communicated over a separate server program via files named
pipe_in and pipe_out.

program2
    Starts the program with a default filename of storage.bin.

program2 {filename}
    Starts the program and writes to a file with a given filename.

server
    Starts the server file that controls file access.

Commands:
    List the contents of the buffer.
    l
    Print each byte in hexadecimal format. There are 16 bytes per output line, for a total of 8 lines. Each byte has exactly two characters, and bytes are separated by a space.

    Zero out the contents of the buffer.
    z
    Set all of the byte values in the buffer to zero.

    Write a byte value to a buffer location.
    b LOC VAL
    LOC is the location in the buffer to write to (LOC is decimal). VAL is the byte value (in decimal) to be written

    Read a byte value from a buffer location.
    B LOC
    LOC is the location in the buffer to read from. Prints the value of the byte (in decimal)

    Write a hexadecimal byte value to a buffer location.
    h LOC VAL
    LOC is the location in the buffer to write to. VAL is the byte value (in hexadecimal) to be written

    Read a byte value from a buffer location.
    H LOC
    LOC is the location in the buffer to read from. Prints the value of the byte (in hexadecimal)

    Write a character to a buffer location.
    c LOC VAL
    LOC is the location in the buffer to write to. VAL is the character to be written

    Read a character value from a buffer location.
    C LOC
    LOC is the location in the buffer to read from. Prints the value of the character (i.e., the glyph)

    Write an integer value to a buffer location.
    i LOC VAL
    LOC is the location in the buffer to write to (remember that integers require multiple bytes to store). VAL is the integer value (in decimal) to be written

    Read an integer value from a buffer location.
    I LOC
    LOC is the location in the buffer to read from. Prints the value of the integer (in decimal)

    Write a floating point value to a buffer location.
    f LOC VAL
    LOC is the location in the buffer to write to (floats require multiple bytes). VAL is the floating point value to be written

    Read a floating point value from a buffer location.
    F LOC
    LOC is the location in the buffer to read from. Prints the value of the floating point value.

    Write a string to a buffer location (may be multiple characters).
    s LOC VAL
    LOC is the location in the buffer to write to. VAL is the string value to be written. Note that it will contain no spaces (due to the tokenization process). Make sure that the NULL terminator is also written to the buffer.

    Read a string from a buffer location.
    S LOC
    LOC is the location in the buffer to read from. Prints the value of the string.

    Write the contents of the buffer to the file.
    w OFFSET LEN
    OFFSET is the offset in the file to start writing to. LEN is the number of bytes in the buffer to write (bytes 0 ... LEN-1 in the buffer are written).

    Read part of the file into the buffer.
    r OFFSET LEN
    OFFSET is the offset in the file to start reading from. LEN is the number of bytes in the buffer to read. 
