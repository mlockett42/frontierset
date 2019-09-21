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

static void AddBitStringToFrontierSet(LP_FRONTIERSET lpfrontierset, unsigned char* data, int length)
{

}

void AddToFrontierSet(LP_FRONTIERSET lpfrontierset, unsigned char* data, int length)
{
  unsigned char* bits = malloc(length * 8);
  // From: https://stackoverflow.com/a/41709317
  int i;
  for (i=0; i<length*8; i++) {
      bits[i] = ((1 << (i % 8)) & (data[i/8])) >> (i % 8);
  }
  AddBitStringToFrontierSet(lpfrontierset, bits, length*8);
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

int GenerateFrontierSet(LP_FRONTIERSET lpfrontierset, char* buffer, int buffersize)
{
  struct FrontierSet* fs = lpfrontierset;
  struct FrontierSetMember* pfirst = NULL;
  if (IsFrontierNodeLeaf(fs->rootnode)) {
    // Return the empty set. Ie everything is on the other side of the frontier
    // That everything starting in zero, one or terminated (ie zero length string)
    pfirst = malloc(sizeof(struct FrontierSetMember));
    pfirst->thiselement = malloc(2);
    strcpy(pfirst->thiselement, "0");
    struct FrontierSetMember* pfs1 = malloc(sizeof(struct FrontierSetMember));
    pfirst->pnext = pfs1;

    pfs1->thiselement = malloc(2);
    strcpy(pfs1->thiselement, "1");
    struct FrontierSetMember* pfsTerm = malloc(sizeof(struct FrontierSetMember));
    pfs1->pnext = pfsTerm;

    pfsTerm->thiselement = malloc(2);
    strcpy(pfsTerm->thiselement, "T");
    pfsTerm->pnext = NULL;
  }

  int iBufferRequired = CalcFrontierSetMemberSize(pfirst);

  InsertFrontierSetInBuffer(pfirst, buffer, buffersize);

  FreeFrontierSetMembers(pfirst);

  return iBufferRequired;
}
