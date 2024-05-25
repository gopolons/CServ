#ifndef ArgumentHandler_h
#define ArgumentHandler_h

/*
 * Operations that can be performed by CServ.
 */
enum Operation { RECEIVE, NONE };

/*
 * Function that prints out an error if the invalid
 * command has been passed as an argument.
 */
void printInvalidCommand();

/*
 * Function that prints out the help instructions.
 */
void printHelpInstructions();

/*
 * Function for setting the port number to be used for
 * receiving incoming communications.
 */
void setPort(char val[]);

/*
 * Function used for handling incoming flags
 * that are passed as arguments to the binary
 */
enum Operation handleArgs(int num, char *flags[]);

#endif // !ArgumentHandler_h
