#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

LP_FRONTIERSET AllocFrontierSet()
{
  struct FrontierSet* fs = malloc(sizeof(struct FrontierSet));
  fs->rootnode = NULL;
}

void FreeFrontierSet(LP_FRONTIERSET lpfrontierset)
{
  free(lpfrontierset);
}

void Add(LP_FRONTIERSET lpfrontierset, unsigned char* data, int length)
{

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
  if (fs->rootnode == NULL) {
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
