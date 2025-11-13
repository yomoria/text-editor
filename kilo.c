/* kilo.c -- A simple text editor in C
 *
 * This program sets the terminal to raw mode and reads user input
 * character by character, displaying the ASCII value of each key pressed.
 * Press 'q' to exit the program.
 *
 * Compile with: gcc -o kilo kilo.c
 * 
 * This code is based on the tutorial from viewsourcecode.org
 */

/*** Includes ***/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** Data ***/
struct termios orig_termios;

/*** Functions ***/

// Function to handle errors and exit
void die(const char *s) {
  perror(s);
  exit(1);
}

// Function to disable raw mode and restore terminal settings
void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) die("tcsetattr");
}

// Function to enable raw mode for terminal input
void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);
  
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/*** init ***/
int main() {
  enableRawMode();

  while (1) {
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1) die("read");
    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }
    if (c == 'q') break;
  }
  return 0;
}