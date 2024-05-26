#include "ArgumentHandler.h"
#include "globals.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

#define RECEIVE_FLAG "--receive"
#define PORT_FLAG "--port"
#define HELP_FLAG "--help"

/*
 * Structure used for keeping flag values
 * against their integer counterparts
 * for easy comparison & assignment.
 */
typedef struct {
  char *key;
  int val;
} t_flagstruct;

/*
 * Table containing flags matched up
 * against their respective integer
 * values.
 *
 * Used by keyfromstring() function
 */
static t_flagstruct lookuptable[] = {
    {HELP_FLAG, 1}, {RECEIVE_FLAG, 2}, {PORT_FLAG, 3}};

/*
 * Utility value for keeping track of
 * number of keys in the lookuptable.
 */
#define NKEYS (sizeof(lookuptable) / sizeof(t_flagstruct))

/*
 * Utility function for getting the integer values
 * associated with a particular flag as defined
 * in the lookuptable.
 */
int keyfromstring(char *key) {
  int i;
  for (i = 0; i < NKEYS; i++) {
    t_flagstruct *flag = &lookuptable[i];
    if (strcmp(flag->key, key) == 0) {
      return flag->val;
    }
  }

  return -1;
}

void printInvalidCommand() {
  printf("Invalid instruction.\nTo "
         "get help on how to use CServ, use --help flag\n");
}

void printHelpInstructions() {
  printf("To use the program, please pass the valid "
         "operation flags combination. For example: --receive --port 3490\nIf "
         "no details (such as port number) are provided, default values will "
         "be used.\n");
}

void setPort(char val[]) {
  if (strlen(val) < sizeof(port)) {
    strcpy(port, val);
    return;
  } else {
    printf("Invalid port value. Please provide a valid value and try again.\n");
    exit(1);
  }
}

enum Operation handleArgs(int num, char *flags[]) {
  if (num <= 1) {
    printHelpInstructions();
    exit(1);
  }

  enum Operation output = NONE;
  for (int ix = 1; ix < num; ix++) {
    switch (keyfromstring(flags[ix])) {
    case 1: {
      if (output != NONE) {
        printInvalidCommand();
        exit(1);
      }
      printHelpInstructions();
      exit(1);
    }
    case 2: {
      if (output != NONE) {
        printInvalidCommand();
        exit(1);
      }
      output = RECEIVE;
      break;
    }
    case 3: {
      if (output != RECEIVE || (ix + 1) > num) {
        printInvalidCommand();
        exit(1);
      }

      char *portValue = flags[ix + 1];
      setPort(portValue);
      ix++;
      break;
    }
    default:
      printInvalidCommand();
      exit(1);
    }
  }
  return output;
}
