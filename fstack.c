// Tue Aug  7 01:17:10 UTC 2018
// Forth stack - primitives
// based on Charley Shattuck's Forth-Like Interpreter

/* Tiny interpreter,
   similar to myforth's Standalone Interpreter
   This example code is in the public domain */

// Sun 17 Jun 03:03:26 UTC 2018
// 4737-a3a-0ca-

/* Data stack for parameter passing
   This "stack" is circular,
   like a Green Arrays F18A data stack,
   so overflow and underflow are not possible
   Number of items must be a power of 2 */
// const int STKSIZE = 8;
#define STKSIZE 8
// const int STKSIZE = 8;
const int STKMASK = 7;
// int stack[STKSIZE]; // the "stack" itself
int stack[STKSIZE]; // the "stack" itself
int p = 0; // top of stack is stack[0]

/* TOS is Top Of Stack */
#define TOS stack[p]
/* NAMED creates a string in flash */

/* push n to top of data stack */
void push(int n) {
  p = (p + 1)& STKMASK;
  TOS = n;
}

/* return top of stack */
int pop(void) { // void added to inside-parens
  int n = TOS;
  p = (p - 1)& STKMASK;
  return n;
}


// references
//     https://github.com/wa1tnr/Metro-M4-Express-interpreter.git
//     Metro-M4-Express-interpreter
//     Charley Shattuck's Feather-M0-interpreter, now for the M4 (with minor changes by me).
//
//     15 June 2018 - timestamped.
