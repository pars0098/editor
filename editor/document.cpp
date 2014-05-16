#include "document.h"

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

document::document(void)
{
	pointLine = 0;
	pointChar = 0;
}

//parseInput function decides what command is being executed from the current line of input and executes that command
void document::parseInput(string s)
{

	//Checks to see whether the input is a command
	if (s.substr(0,1)==".") {

		//drop the dot
		s.erase(0,1);
		
		string c = s.substr(0,1);

		//check whether the command was an insert only
		if(c==".") {
			pointLine = addLine(pointLine, s);
		}

		if(c=="<") {
			pointLine = 0;
			pointChar = 0;
		}

		if(c==">") {
			pointLine = line.size();
			pointChar = line[line.size()-1].length();
		}

		if(c=="p") {
			if(pointLine > 0) { pointLine--; }
			pointChar = 0;
		}
		
		if(c=="n") {
			if(pointLine < line.size()) { pointLine++; }
			pointChar = 0;
		}

		if(c=="k") {
			if(line.size() > 0) { removeLine(); }
		}

		if(c=="a") {
			pointChar = 0;
		}

		if(c=="e") {
			if(pointLine < line.size()) {
				pointChar = line[pointLine].length();
			}
		}

		if(c=="f") {
			pointChar++;
			if(pointChar > line[pointLine].length()-1) {
				pointLine++;
				pointChar=0;
			}
		}

		if(c=="b") {
			pointChar--;
			if(pointChar < 0) {
				if(pointLine > 0) {
					pointLine--;
					pointChar=line[pointLine].length()-1;
				}
				else
				{
					pointChar=0;
				}
			}
		}

		if(c=="d") {


			//if(pointChar >= line[pointLine].length()) {
			//	line[pointLine]+=line[pointLine+1];
			//	line.erase(line.begin()+pointLine+1);
			//}
			//else {
			//	line[pointLine].erase(pointChar, 1);
			//}

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
						line.erase(line.begin()+pointLine+1);
					}
					else
					{
						line.erase(line.begin()+pointLine);
					}
				}
				else {
					line[pointLine].erase(pointChar, 1);
				}

			}
		}

		if(c=="i") {
			s.erase(0,1);

			//store the current input into a temporary string
			string t = s;

			//check for the presence of a newline character and loop through until there are no more left
			size_t p;
			while((p = t.find("\\n",0)) < t.length()) {

				//find the string that exists just before the newline character
				string x = t.substr(0, p);

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
	//Otherwise just insert the string into the document
	else
	{
		pointLine = addLine(pointLine, s);
	}

}

int document::addLine(int atLine, string s)
{
	if (atLine <= line.size()) {
		line.insert(line.begin()+atLine, s);
		return atLine+1;
	}
	else
	{
		line.push_back(s);
		return line.size();
	}
}

void document::removeLine()
{
	if (pointLine < line.size()) {
		line.erase(line.begin()+pointLine);
	}
}

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

void document::splitLine(int atLine, int atChar) {

	if(line.size()==0) {
			line.push_back("");
	}
	else {
		if (atLine >= line.size()) {
			atLine = line.size()-1;
			atChar = line[atLine].length();
		}
		string s = line[atLine].substr(atChar, s.length()-atChar);
		line[atLine] = line[atLine].substr(0, atChar);
		addLine(atLine+1, s);
	}

}

string document::toString()
{
	string s;

	for (vector<string>::iterator it = line.begin() ; it != line.end(); ++it){
		s+=*it;
		s+='\n';
	}
	return s;
}

string document::debugString()
{
	//string s;
	ostringstream s;

	s << "pointLine=" << (pointLine) << ";" << "pointChar=" << (pointChar) << "line.size()=" << line.size() << "\n";
	s << "----------------------------------------------\n";
	for (vector<string>::iterator it = line.begin() ; it != line.end(); ++it){
		s << *it;
		s << '\n';
	}
	s << "----------------------------------------------\n";
	return s.str();
}
