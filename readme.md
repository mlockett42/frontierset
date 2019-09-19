# Frontier Set

A library implementing Frontier Set technology in the c language

## How to compile

How to create a library

https://randu.org/tutorials/c/libraries.php

How to use the make program

http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

## API description

Some notes on the API

LP_FRONTIERSET AllocFrontierSet(); // Create the generic frontier set data structure

void FreeFrontierSet(LP_FRONTIERSET); // Destroy the in memory representation of the frontier set

void Add(LP_FRONTIERSET, unsigned char* data, int length); // Add an element to the frontier set.
The element in length bytes long and contains data

int GenerateFrontierSet(LP_FRONTIERSET, char* buffer, int buffersize);

// Output the frontier to a buffer.
// The buffer is buffersize bytes big and pointed to by buffer
// The return value is the number of chars that would be copied to the buffer if the full
// output was written
// Ie the return value is how big the buffer should be
// To work out how big to make the buffer. set buffer to null and buffersize to zero
//
// The result is a buffer full of c strings. A csting is a array of characters followed by a NULL.
// The next char in the buffer is then the next string. The final string is just a NULL (ie a
// double NULL the first one ends the last string the next one is the empty string.
//
// The output string. This is a bitwise frontier set we evaluate the bytes in order and the bits
// from the most significant to the least significant.
//
// The string can contain te following values '0', '1' or 'T'
// '0' the next bit value should be 0
// '1' the next bit value should be 1
// 'T' the byte (unsigned char) array terminates here.

// Use the string returning syntax of this guy
// http://www.cplusplus.com/reference/cstdio/snprintf/
//
// for reference but don't use
// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-findfirstfilenamew
// and
// https://cryptsoft.com/pkcs11doc/v220/pkcs11__all_8h.html#aC_Encrypt

BOOL Contains(LP_FRONTIERSET, unsigned char* data, int length)

Add example function that returns iff the given data is in the frontier set.

// LP_FRONTIERSET the set to test
// data + length the element to test for
// WOrks by calleding GenerateFrontierSet then iterating the entire set looking for a match.
