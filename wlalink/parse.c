
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"



int get_next_token(char *in, char *out, int *pos) {

  int index, t;


  /* skip white space */
  for (index = 0; in[index] == ' ' || in[index] == 0x09; index++);
  if (in[index] == 0)
    return FAILED;

  /* something between ""? */
  if (in[index] == '"') {
    index++;
    for (t = 0; in[index] != '"' && in[index] != 0x09 && in[index] != 0; index++, t++)
      out[t] = in[index];
    if (in[index] == '"')
      index++;
  }
  else {
    for (t = 0; in[index] != ' ' && in[index] != 0x09 && in[index] != 0; index++, t++)
      out[t] = in[index];
  }
  
  out[t] = 0;

  if (pos != NULL)
    *pos += index;

  return SUCCEEDED;
}


int get_next_number(char *in, int *out, int *pos) {

  int index, o, n;


  /* skip white space */
  for (index = 0; in[index] == ' ' || in[index] == 0x09; index++);
  if (in[index] == 0)
    return FAILED;

  if (in[index] >= '0' && in[index] <= '9') {
    /* is it hex after all? */
    n = 0;
    for (o = 0; 1; o++) {
      if (in[index+o] >= '0' && in[index+o] <= '9')
        continue;
      if (in[index+o] >= 'a' && in[index+o] <= 'f') {
        n = 1;
        break;
      }
      if (in[index+o] >= 'A' && in[index+o] <= 'F') {
        n = 1;
        break;
      }
      if (in[index+o] == 'h' || in[index+o] == 'H') {
        n = 1;
        break;
      }
      break;
    }

    if (n == 1) {
      /* hex */
      o = 0;
      while (TRUE) {
        if (in[index] >= '0' && in[index] <= '9')
          o = (o * 16) + in[index] - '0';
        else if (in[index] >= 'a' && in[index] <= 'f')
          o = (o * 16) + in[index] - 'a' + 0xA;
        else if (in[index] >= 'A' && in[index] <= 'F')
          o = (o * 16) + in[index] - 'A' + 0xA;
        else if (in[index] == 'h' || in[index] == 'H')
          break;
        else if (in[index] == ' ' || in[index] == 0x09 || in[index] == 0)
          break;
        else
          return FAILED;
        index++;
      }
    }
    else {
      /* decimal */
      for (o = 0; in[index] >= '0' && in[index] <= '9'; index++)
        o = (o * 10) + in[index] - '0';
      if (!(in[index] == ' ' || in[index] == 0x09 || in[index] == 0))
        return FAILED;
    }
  }
  else if (in[index] == '$') {
    index++;
    o = 0;
    while (TRUE) {
      if (in[index] >= '0' && in[index] <= '9')
        o = (o * 16) + in[index] - '0';
      else if (in[index] >= 'a' && in[index] <= 'f')
        o = (o * 16) + in[index] - 'a' + 0xA;
      else if (in[index] >= 'A' && in[index] <= 'F')
        o = (o * 16) + in[index] - 'A' + 0xA;
      else if (in[index] == ' ' || in[index] == 0x09 || in[index] == 0)
        break;
      else
        return FAILED;
      index++;
    }
  }
  else
    return FAILED;

  *out = o;

  if (pos != NULL)
    *pos += index;

  return SUCCEEDED;
}
