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
	bool searchInLine(string searchFor, int atLine, int atChar, bool exactChar, bool wholeMatch);

private:
	int pointLine; //store the current point line number
	int pointChar; //store the current point character position in the current line
	vector<string> line; //a vector of strings for each individual line
	vector<string> buffer; //a vector of strings representing the buffer
	vector<string> clipboard; //a vector of strings representing the clipboard
	int markStart; //store the position of the mark start;
	int markEnd; //store the position of the mark end;
	int markStartLine; //store the position of the mark start;
	int markEndLine; //store the position of the mark end;
	int markStartChar; //store the position of the mark start;
	int markEndChar; //store the position of the mark end;
	bool marking; //the current document is being marked
};
