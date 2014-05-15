#include <string>
#include <iostream>
#include <vector>

using namespace std;

class document
{
public:
	document(void);
	void parseInput(string s);
	int addLine(int atLine, string s);
	void removeLine();
	int insertString(int atLine, int atChar, string s);
	void splitLine(int atLine, int atChar);
	string toString();
	string debugString();

private:
	int pointLine; //store the current point line number
	int pointChar; //store the current point character position in the current line
	vector<string> line; //a vector of strings for each individual line
	vector<string> clipBoard; //a vector of strings representing the clipboard

};