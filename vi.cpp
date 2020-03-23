#include <iostream>
#include <vector>
#include <cstdio>
#include <termios.h>
using namespace std;

// editor state
vector<vector<char>> text; // vector of rows of text
unsigned int row = 0, col = 0; // current cursor position
bool command = false; // are we in command mode?

// motion commands
void left() {
	if(col > 0) { // not first column
		col--; // move left
	} else if(row > 0) { // first column, not first row
		row--; // move up
		col = text.at(row).size(); // move to end of line
	}
}
void right() {
	if(col < text.at(row).size()) { // not last column
		col++; // move right
	} else if(row < text.size() - 1) { // last column, not last row
		row++; // move down
		col = 0; // move to beginning of line
	}
}
void up() {
	if(row > 0) { // not first row
		row--; // move up
		// if moving up put us past the end, put the cursor at the end
		if(col > text.at(row).size())
			col = text.at(row).size();
	}
}
void down() {
	if(row < text.size() - 1) {
		row++; // move down
		// if moving down put us past the end, put the cursor at the end
		if(col > text.at(row).size())
			col = text.at(row).size();
	}
}

// insert and remove characters
void insert(char c) {
	if(c == '\n') {
		vector<char> line; // the newly created row
		
		// append characters after the cursor to the new row
		for(unsigned int i = col; i < text.at(row).size(); i++)
			line.push_back(text.at(row).at(i));
		text.at(row).resize(col); // truncate the old row
		
		text.insert(text.begin() + row + 1, line); // add the new row
	} else { // insert the character
		text.at(row).insert(text.at(row).begin() + col, c);
	}
	
	right(); // position the cursor after the new character
}
void remove() {
	// no first character to delete
	if(row == 0 && col == 0) return;
	left(); // move over top of the character to be deleted
	
	if(col < text.at(row).size()) { // not at the end of the row
		text.at(row).erase(text.at(row).begin() + col); // delete as normal
	} else {
		// append the next row to this one
		for(unsigned int i = 0; i < text.at(row + 1).size(); i++)
			text.at(row).push_back(text.at(row + 1).at(i));
		text.erase(text.begin() + row + 1); // delete the next row
	}
}

// get a character without any buffering or echoing
int getch() {
	// get a copy of the germinal attributes
	struct termios old;
	tcgetattr(0, &old);
	
	// turn off...
	struct termios current = old;
	current.c_lflag &= ~ICANON; // read characters immediately
	current.c_lflag &= ~ECHO; // don't print them when we read them
	tcsetattr(0, TCSANOW, &current); // set new attributes
	
	int ch = getchar(); // get a single character
	
	// reset to old attributes
	tcsetattr(0, TCSANOW, &old);
	return ch;
}

// show the state of the editor
void display() {
	cout << "\x1B[2J"; // clear the screen
	cout << "\x1B[0;0f"; // move the cursor to the top left corner
	
	// for each row
	for(unsigned int i = 0; i < text.size(); i++) {
		cout << "\x1B[K"; // clear the row in the display
		
		// print out the row
		for(unsigned int j = 0; j < text.at(i).size(); j++)
			cout << text.at(i).at(j);
		cout << "\n";
	}
	
	// clear the row, and print the mode
	cout << "\x1B[K" << (command ? "command mode\n" : "insert mode\n");
	
	// position the cursor where it belongs
	// +1 because the terminals want 1-based rows and cols
	cout << "\x1B[" << row + 1 << ';' << col + 1 << 'H';
}

// process a command
void go(int ch) {
	switch(ch) {
		case 'i': command = false; break; // switch to insert mode
		
		// move
		case 'h': left(); break;
		case 'j': up(); break;
		case 'k': down(); break;
		case 'l': right(); break;
		
		case 'q': exit(0); break; // quit
	}
}
int main() {
	text.push_back(vector<char>()); // start with a single blank line
	
	for(;;) {
		display();
		int ch = getch();
		if(command) // in command mode
			go(ch);
		else if(ch == '\e') // escape switches to command mode
			command = true;
		else if(ch == 127) // backspace, which linux spells 'delete'
			remove();
		else
			insert(ch); // otherwise, insert
	}
}
