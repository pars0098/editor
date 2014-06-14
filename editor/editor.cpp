//Flinders University CP2, Semester 1 2014 - Assignment 2 Core
//Author: David Parsons
//Student ID: pars0098
//Email: pars0098@flinders.edu.au

#include "document.h"

#include <string>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {

	//The document stores the complete document text
	document d;

	//A string to hold the current input
	string s;


	//debug mode allows commands to be inputed one at a time and a verbose output produced with each command
	bool debugMode = false;

	//debug mode will be switched on by the presence of the -d command line argument
	if (argc > 1) {
		for(int i=1; i < argc; i++) {
			string a = argv[i];
			if (a=="-d") {
				debugMode=true;
			}
		}
	}

	if(!debugMode) {
		//Start taking input one line at a time and processing it
		while (!cin.eof()) {
			getline(cin, s);
			if (cin.good()) {
				//Line is good, send it to the document to be parsed
				d.parseInput(s);
			}
		}
		cout << d.toString();
	}

	else {
		//Debugging mode
		//Output the initial state
		cout << d.debugString();

		while (cin){
			getline(cin, s);
			d.parseInput(s);
			cout << d.debugString();
		}
	}

	return 0;
}

