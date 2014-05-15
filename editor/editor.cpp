#include "editor.h"
#include "document.h"

#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	document d;
	string s;

	//debug mode will be switched on by the presence of any arguments
	//not the most elegant but a quick and easy way to turn debug on while developing, but also allowing "try" to be run without needing to change code
	bool debugMode = false;
	if (argc > 1) { debugMode=true;	}

	if(!debugMode) {
		//Start taking input one line at a time and processing it
		while (!cin.eof()) {
			getline(cin, s);
			if (cin.good()) {
				//Line is good, send it to the document to be parsed
				d.parseInput(s);
			}
			//Add error handling here if I get time
		}
		cout << d.toString();
	}
	else {
		//Debugging code
		//Allows commands to be inputed one at a time, and verbose output produced with each command
		while (cin){
			getline(cin, s);
			d.parseInput(s);
			cout << d.debugString();
		}
	}

	return 0;
}

