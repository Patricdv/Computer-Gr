#include "BCommandLine.h"
#include <stdio.h>

bool stob(char * str) {
	if ((str[0] == 't'|| str[0] == 'T') &&
	    (str[1] == 'r'|| str[1] == 'R') &&
		(str[2] == 'u'|| str[2] == 'U') &&
		(str[3] == 'e'|| str[3] == 'E'))
		return true;
	else
		return false;
}

bool intType(int) {return true;};
bool booleanType(bool) {return true;};
bool floatType(float) {return true;};
bool charType(char) {return true;};

bool getValue(bool t, char * str) { return stob(str);};
int getValue(int t, char * str) { return atoi(str);};
float getValue(float t, char * str) { return atof(str);};
char getValue(char t, char * str) { return str[0];};
char * getValue(char* t, char * str) { return str;};
