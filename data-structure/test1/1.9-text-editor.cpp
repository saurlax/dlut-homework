#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>

using namespace std;

void enable_raw_mode(termios &orig_termios) {
  termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode(termios &orig_termios) {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void move_cursor_to_end() { cout << "\033[999B\033[0G"; }

void move_cursor_to_start() { cout << "\033[H"; }

void clear_screen() { cout << "\033[2J"; }

int main() {
  termios orig_termios;
  tcgetattr(STDIN_FILENO, &orig_termios);

  clear_screen();
  move_cursor_to_end();
  enable_raw_mode(orig_termios);

  bool is_edit_mode = false;
  string text;
  char ch;

  while (true) {
    ch = getchar();

    if (is_edit_mode) {
      if (ch == 27) { // ESC key
        is_edit_mode = false;
        move_cursor_to_end();
      } else {
        text.push_back(ch);
        cout << ch;
      }
    } else {
      if (ch == 'i') {
        is_edit_mode = true;
        move_cursor_to_start();
      }
    }
  }

  disable_raw_mode(orig_termios);
  return 0;
}