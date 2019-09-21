#define LP_FRONTIERSET void*

extern LP_FRONTIERSET AllocFrontierSet();
// Create the generic frontier set data structure

extern void FreeFrontierSet(LP_FRONTIERSET);
// Destroy the in memory representation of the frontier set

extern void AddToFrontierSet(LP_FRONTIERSET, unsigned char* data, int length);
// Add an element to the frontier set. The element in length bytes long and contains data

extern int GenerateFrontierSet(LP_FRONTIERSET, char* buffer, int buffersize);
// Output the frontier to a buffer.
// The buffer is buffersize bytes big and pointed to by buffer
// The return value is the number of chars that would be copied to the buffer if the full
// output was written
// Ie the return value is how big the buffer should be
// To work out how big to make the buffer. set buffer to null and buffersize to zero
//
// The result is a buffer full of c strings. A c string is a array of characters followed by a NULL.
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

extern void CreateBitString(char* sDest, const unsigned char* sSrc, int length);
