//Flinders University CP2, Semester 1 2014 - Assignment 2 Core
//Author: David Parsons
//Student ID: pars0098
//Email: pars0098@flinders.edu.au
//The document class is where the current contents of the editor are stored
//The contents are stored as a vector of lines, each line being a string of characters
//The current point position is referenced by the current line and the current character in that line

#include "document.h"

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

document::document(void)
{
	//The current line the point is at is initialised
	pointLine = 0;
	//The character within the current line that the point is at is initialised
	pointChar = 0;
}

//parseInput function decides what command(s) should be executed from the current line of input and executes the command(s)
void document::parseInput(string s)
{
	//Checks to see whether the input is a command by looking for the preceding "."
	//If no "." is found then the input string is just added to the document
	if (s.substr(0,1)!=".")	{
		pointLine = addLine(pointLine, s);
		return;
	}
	else {

		//drop the dot
		s.erase(0,1);

		//get the next character to determine what type of command this is
		string c = s.substr(0,1);

		//check whether the command was an insert only, denoted by another ".", and if it is, then just add the line to the document and return
		if(c==".") {
			pointLine = addLine(pointLine, s);
			return;
		}
		
		//Having determined that there is at least one command, keep parsing through the string until there are no commands left
		while (s.length()>0) {

			//Check whether the command is to be repeated by looking for any numbers and storing them in a string
			string repeatString = "";
			while (s.find_first_of("0123456789",0)==0) {
				//Consume the next number
				repeatString+=s.substr(0,1);
				s.erase(0,1);
			}

			//initialise the repeat count to default of one repeat
			int repeatCount = 1;
			if (repeatString.length() > 0) {
				//Convert the numbers from a string to an int
				istringstream (repeatString) >> repeatCount;
				if(repeatCount<=0) {
					//If all else fails then set repeat to back 1
					repeatCount = 1;
				}
			}

			//Consume the next command
			c = s.substr(0,1);
			s.erase(0,1);

			//Repeat the command however many times is required
			for (int counter=0;counter < repeatCount; counter++){

				//Command to move point to start of document
				if(c=="<") {
					pointLine = 0;
					pointChar = 0;
				}

				//Command to move point to end of document
				if(c==">") {
					pointLine = line.size();
					pointChar = line[line.size()-1].length();
				}

				//Command to move point to previous line
				if(c=="p") {
					if(pointLine > 0) { pointLine--; }
					pointChar = 0;
				}

				//Command to move point to next line
				if(c=="n") {
					if(pointLine < line.size()) { pointLine++; }
					pointChar = 0;
				}

				if(c=="y") {
					if(buffer.size() > 0) { line.insert(line.begin()+pointLine, buffer.begin(), buffer.end()); pointLine+=buffer.size(); pointChar=0; }
				}

				//Command to remove entire line
				if(c=="k") {
					if(line.size() > 0) { removeLine(); }
				}
				else
				{
					buffer.empty();
				}

				//Command to move point to beginning of current line
				if(c=="a") {
					pointChar = 0;
				}

				//Command to move point to end of current line
				if(c=="e") {
					//Check that point is on a line
					if(pointLine < line.size()) {
						pointChar = line[pointLine].length();
					}
				}

				//Command to move point forward 1 character
				if(c=="f") {
					pointChar++;
					//Check whether point has moved past the end of the line
					if(pointChar > line[pointLine].length()-1) {
						//Move point to the start of the next line
						pointLine++;
						pointChar=0;
					}
				}

				//Command to move point back 1 character
				if(c=="b") {
					pointChar--;
					//Check whether point has moved past the start of the line
					if(pointChar < 0) {
						//Check that point is not before the start of the first line
						if(pointLine > 0) {
							//There is a preceding line so move point to the end of the previous line
							pointLine--;
							pointChar=line[pointLine].length();
						}
						else
						{
							//Point is already at the first line so just move point to the start of the line
							pointChar=0;
						}
					}
				}

				//Command to delete a character
				if(c=="d") {

					//make sure there are some lines to work with, otherwise do nothing
					//check if the current line point is not at the end
					if(line.size() > 0 && pointLine < line.size()) {

						//check if the current character point is at the end of the line
						if(pointChar >= line[pointLine].length()) {

							//check there another line below the current
							if((pointLine + 1) < line.size()) {

								//append the string from the next line to the current line
								line[pointLine]+=line[pointLine+1];

								//erase the next line
								vector<string>::iterator i = line.erase(line.begin()+pointLine+1);
							}
							else
							{
								vector<string>::iterator i = line.erase(line.begin()+pointLine);
							}
						}
						else {
							//Nothing special so just erase a single character at point
							line[pointLine].erase(pointChar, 1);
						}
					}
				}
			}

			if(c=="i") {

				//consume the input string
				string t = s;
				s = "";

				//Repeat the command however many times is required
				for (int counter=0;counter < repeatCount; counter++){

					//check for the presence of a newline character and loop through until there are no more left
					size_t p;
					while((p = t.find("\\n",0)) < t.length()) {

						//find the string that exists just before the newline character
						string x;

						x = t.substr(0, p);

						//check there is a string to insert, and if there is then insert it
						if (x.length() > 0) {
							pointChar = insertString(pointLine, pointChar, x);
						}

						//split the line at the point where the newline character appears
						splitLine(pointLine, pointChar);

						//move to the next line
						pointLine++;
						pointChar=0;

						//erase the portion of the string up to and including the newline
						t.erase(0, p + 2);
					}

					//if there is anything left over after the last newline character, or there was no newline character, then insert that string
					if (t.length() > 0) {
						pointChar = insertString(pointLine, pointChar, t);
					}
				}
			}

			if (c=="s") {
				size_t p;

				bool f = false;
				int firstFoundAtLine;
				int firstFoundAtChar;

				if ((p = s.find("\\n",0)) < s.length()) {
					string x;
					x = s.substr(0, p);
					f = searchEndOfLine(x);
					s.erase(0, p + 2);
					while (((p = s.find("\\n",0)) < s.length())&&f) {
						string x;
						x = s.substr(0, p);
						f = searchEntireLine(x);
						s.erase(0, p + 2);
					}
					if (f) {
						string x;
						x = s.substr(0, p);
						f = searchStartOfLine(x);
						s.erase(0, p + 2);
					}
				}
				else if (s.length() > 0) {
					search(s);
					s.erase(0,s.length());

				}
			}
		}
	}
}


//Function to add a line to the document
int document::addLine(int atLine, string s)
{
	if (atLine <= line.size()) {
		vector<string>::iterator i = line.insert(line.begin()+atLine, s);
		return atLine+1;
	}
	else
	{
		line.push_back(s);
		return line.size();
	}
}


//Remove a line from the document at the current point
void document::removeLine()
{
	if (pointLine < line.size()) {
		buffer.insert(buffer.end(),line[pointLine]);
		vector<string>::iterator i = line.erase(line.begin()+pointLine);
	}
}

//Insert a string into the document at the specified line and character point
int document::insertString(int atLine, int atChar, string s) {

	//if line is currently empty then add a new item
	if(line.size()==0) { line.push_back(""); }

	//if atLine is outside of the bounds then move it to the last entry
	if (atLine >= line.size()) { atLine = line.size()-1; }

	//insert the string
	line[atLine].insert(atChar, s);

	//move the character point to the end of the new string
	atChar += s.length();

	return atChar;
}

//Split the specified line into 2 lines at the specified character point
void document::splitLine(int atLine, int atChar) {

	if(line.size()==0) {
			line.push_back("");
	}
	else {
		if (atLine >= line.size()) {
			atLine = line.size()-1;
			atChar = line[atLine].length();
		}
		string s = line[atLine];
		string t = s.substr(atChar, s.length()-atChar);
		line[atLine] = line[atLine].substr(0, atChar);
		addLine(atLine+1, t);
	}
}

bool document::search(string searchFor) {

	//start from pointLine, pointChar and work forwards
	while (pointLine<line.size()) {
		int p = line[pointLine].find(searchFor, pointChar);
		if (p >= pointChar) {
			pointChar=p;
			return true;
		}
		else
		{
			pointChar=0;
			pointLine++;
		}
	}
	
	//loop back to the beginning of the document
	pointLine=0;
	pointChar=0;
	while (pointLine<=startLine) {
		int p = line[pointLine].find(searchFor, pointChar);
		if ((pointLine!=startLine && p > pointChar) || (pointLine==startLine && p < startChar)) {
			pointChar=p;
			return true;
		}
		else
		{
			pointChar=0;
			pointLine++;
		}
	}
	return false;
}

bool document::searchEndOfLine(string searchFor) {

	int startLine = pointLine;
	int startChar = pointChar;

	//start from pointLine, pointChar and work forwards
	while (pointLine<line.size()) {
		int pointCharSearch = line[pointLine].length() - searchFor.length();
		if (pointCharSearch > pointChar) {
			int p = line[pointLine].find(searchFor, pointCharSearch);
			if (p == pointCharSearch) {
				pointChar=p;
				return true;
			}
			else
			{
				pointChar=0;
				pointLine++;
			}
		}
	}
	
	//loop back to the beginning of the document
	pointLine=0;
	pointChar=0;
	while (pointLine<=startLine) {
		int pointCharSearch = line[pointLine].length() - searchFor.length();
		if (pointCharSearch > pointChar) {
			int p = line[pointLine].find(searchFor, pointCharSearch);
			if (p == pointCharSearch) {
				pointChar=p;
				return true;
			}
			else
			{
				pointChar=0;
				pointLine++;
			}
		}
	}

	return false;

}

bool document::searchStartOfLine(string searchFor) {

	int p = line[pointLine].find(searchFor, 0);
	if (p == 0) {
		pointChar=p;
		return true;
	}
	return false;

}

bool document::searchEntireLine(string searchFor) {
	return line[pointLine]==searchFor;
}
//Return the entire document as a string
string document::toString()
{
	string s;

	for (vector<string>::iterator it = line.begin() ; it != line.end(); ++it){
		s+=*it;
		s+='\n';
	}
	return s;
}


//Return the entire document as a string with additional debugging information included
//Debugging mode can be invoked by starting the program with the -d argument
string document::debugString()
{
	//string s;
	ostringstream s;

	s << "\nOutput after command\n-------------------------------------------------------------------\n";
	for (vector<string>::iterator it = line.begin() ; it != line.end(); ++it){
		s << *it;
		s << '\n';
	}
	s << "-------------------------------------------------------------------\n";
	s << "pointLine=" << (pointLine) << "; pointChar=" << (pointChar) << "; line.size()=" << line.size() << "\n";
	s << "-------------------------------------------------------------------\n\nCommand: ";

	return s.str();
}
