// const int STKSIZE = 8;
// const int STKMASK = 7;
// extern int stack[]; // the "stack" itself
// int p = 0; // top of stack is stack[0]

/* TOS is Top Of Stack */
// #define TOS stack[p]

/* push n to top of data stack */
extern void push(int n); // {
  // p = (p + 1)& STKMASK;
  // TOS = n;
// }

/* return top of stack */
extern int pop(void); // {
  // int n = TOS;
  // p = (p - 1)& STKMASK;
  // return n;
// }


// references
//     https://github.com/wa1tnr/Metro-M4-Express-interpreter.git
//     Metro-M4-Express-interpreter
//     Charley Shattuck's Feather-M0-interpreter, now for the M4 (with minor changes by me).
//
//     15 June 2018 - timestamped.
