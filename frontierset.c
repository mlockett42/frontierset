#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "frontierset.h"

#define BRANCH_0 0
#define BRANCH_1 1
#define BRANCH_TERMINATOR 2

#define MIN(a, b) (a < b ? a : b)

struct FrontierNode {
  struct FrontierNode* branches[3];
};

struct FrontierSet {
  struct FrontierNode* rootnode;
};

struct FrontierSetMember {
  char* thiselement;
  struct FrontierSetMember* pnext;
};

struct FrontierNode* InitFrontierNode()
// Creates an empty frontier node structure
{
  struct FrontierNode* ret = malloc(sizeof(struct FrontierNode));
  ret->branches[BRANCH_0] = NULL;
  ret->branches[BRANCH_1] = NULL;
  ret->branches[BRANCH_TERMINATOR] = NULL;
}

static bool IsFrontierNodeLeaf(struct FrontierNode* fn)
{
  return fn->branches[BRANCH_0] == NULL && fn->branches[BRANCH_1] == NULL &&
          fn->branches[BRANCH_TERMINATOR] == NULL;
}

LP_FRONTIERSET AllocFrontierSet()
{
  struct FrontierSet* fs = malloc(sizeof(struct FrontierSet));
  fs->rootnode = InitFrontierNode();
  fs->rootnode->branches[BRANCH_0] = InitFrontierNode();
  fs->rootnode->branches[BRANCH_1] = InitFrontierNode();
  fs->rootnode->branches[BRANCH_TERMINATOR] = InitFrontierNode();
  return fs;
}

static void FreeFrontierNodes(struct FrontierNode* fn)
// Free the tree of nodes below and including fn
{
  if (fn == NULL)
    return;
  FreeFrontierNodes(fn->branches[BRANCH_0]);
  FreeFrontierNodes(fn->branches[BRANCH_1]);
  FreeFrontierNodes(fn->branches[BRANCH_TERMINATOR]);
}

void FreeFrontierSet(LP_FRONTIERSET lpfrontierset)
{
  FreeFrontierNodes(((struct FrontierSet*)lpfrontierset)->rootnode);
  free(lpfrontierset);
}

static void AddBitStringToFrontierNode(struct FrontierNode* fn,
    unsigned char* data, int length, int index)
{
  if (index > length)
  // If we have gone past the end of the array there is nothing left to do
  {
    return;
  }
  if (IsFrontierNodeLeaf(fn))
  {
    fn->branches[BRANCH_0] = InitFrontierNode();
    fn->branches[BRANCH_1] = InitFrontierNode();
    if ((index & 7) == 0)
    {
      // We can only have terminators on byte boundaries
      fn->branches[BRANCH_TERMINATOR] = InitFrontierNode();
    }
  }
  if (index == length)
  {
    FreeFrontierNodes(fn->branches[BRANCH_TERMINATOR]);
    fn->branches[BRANCH_TERMINATOR] = NULL;
  }
  if (index < length)
  {
    AddBitStringToFrontierNode(fn->branches[data[index]], data, length,
                               index+1);
  }
}

void AddToFrontierSet(LP_FRONTIERSET lpfrontierset, unsigned char* data, int length)
{
  unsigned char* bits = malloc(length * 8);
  // From: https://stackoverflow.com/a/41709317
  int i;
  for (i=0; i<length*8; i++) {
    bits[i] = ((0x80 >> (i % 8)) & (data[i/8])) >> (7 - i % 8);
  }
  AddBitStringToFrontierNode(((struct FrontierSet*)lpfrontierset)->rootnode, bits, length*8, 0);
  free(bits);
}

int CalcFrontierSetMemberSize(struct FrontierSetMember* pthis)
// Calculate the size required to store the frontier set member strings
{
  if (pthis == NULL)
    return 1; // One character required for the final 0
  else
    return strlen(pthis->thiselement) + 1 + CalcFrontierSetMemberSize(pthis->pnext);
}

void FreeFrontierSetMembers(struct FrontierSetMember* pthis)
// Cleanup internal structures used to generate the frontier set output
{
  if (pthis != NULL)
  {
    FreeFrontierSetMembers(pthis->pnext);
    free(pthis);
  }
}

void InsertFrontierSetInBuffer(struct FrontierSetMember* pthis, char* buffer,
                               int buffersize)
{
  if (buffersize == 0)
  // We have run out of buffer do nothing
  {
    return;
  }
  if (pthis == NULL)
  // We have reached the end of the frontierset just add the trailling null and
  // exit
  {
    buffer[0] = '\0';
    return;
  }
  strncpy(buffer, pthis->thiselement, buffersize);
  int iBufferUsed = MIN(strlen(pthis->thiselement) + 1, buffersize);
  // Recursive call for this next element in the set
  InsertFrontierSetInBuffer(pthis->pnext, buffer + iBufferUsed,
                            buffersize - iBufferUsed);
}

struct FrontierSetMember* GenerateFrontierSetMembers(const char* prefix,
  struct FrontierNode* fn, struct FrontierSetMember* pNext)
{
  struct FrontierSetMember* pThis;
  if (IsFrontierNodeLeaf(fn))
  {
    pThis = malloc(sizeof(struct FrontierSetMember));
    pThis->thiselement = malloc(strlen(prefix) + 1);
    strcpy(pThis->thiselement, prefix);
    pThis->pnext = pNext;
    return pThis;
  }
  if (fn->branches[BRANCH_TERMINATOR] != NULL)
  {
    pThis = malloc(sizeof(struct FrontierSetMember));
    pThis->thiselement = malloc(strlen(prefix) + 2);
    pThis->pnext = pNext;
    strcpy(pThis->thiselement, prefix);
    strcat(pThis->thiselement, "T");
    pNext = pThis;
  }
  char* nextprefix;
  int prefixlen;
  for (int i = BRANCH_1; i >= BRANCH_0 ; i--)
  {
    prefixlen = strlen(prefix);
    nextprefix = malloc(prefixlen + 2);
    strcpy(nextprefix, prefix);
    nextprefix[prefixlen] = i - BRANCH_0 + '0';
    nextprefix[prefixlen + 1] = '\0';
    pNext = GenerateFrontierSetMembers(nextprefix, fn->branches[i], pNext);
  }
  return pNext;
}

int GenerateFrontierSet(LP_FRONTIERSET lpfrontierset, char* buffer, int buffersize)
{
  struct FrontierSet* fs = lpfrontierset;
  struct FrontierSetMember* pfirst = NULL;
  pfirst = GenerateFrontierSetMembers("", fs->rootnode, NULL);

  int iBufferRequired = CalcFrontierSetMemberSize(pfirst);

  InsertFrontierSetInBuffer(pfirst, buffer, buffersize);

  FreeFrontierSetMembers(pfirst);

  return iBufferRequired;
}
