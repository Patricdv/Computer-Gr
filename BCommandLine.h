/***************************** License **********************************

 Copyright (C) 2011 by Jose Carlos Bins Filho josebins@gmail.com

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

************************************************************************/

#if !defined(_BCOMMANDLINE_H_)
#define _BCOMMANDLINE_H_

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>


// Type of  options
#define NAME_EQUAL_VALUE_CLPARAM				                "10101"
#define NAME_OPTIONALSPACE_EQUAL_OPTIONALSPACE_VALUE_CLPARAM 	"12121"
#define NAME_SPACE_EQUAL_SPACE_VALUE_CLPARAM			        "11111"
#define NAME_SPACE_VALUE_CLPARAM				                "11001"
#define NAME_CLPARAM						                 	"10000"
#define VALUE_CLPARAM							                "00001"

// Type of parameters
#define NULL_CLPARAM							'\0'
#define STRING_CLPARAM							's'
#define INTEGER_CLPARAM							'i'
#define BOOLEAN_CLPARAM							'b'
#define FLOAT_CLPARAM							'f'
#define CHAR_CLPARAM							'c'

template <class T> const T& max(const T& a, const T& b) {
	return (a < b) ? b : a;
}
template <class T> const T& min(const T& a, const T& b) {
	return (a > b) ? b : a;
}
template <class T> bool intType(T) {return false;};
bool intType(int);
template <class T> bool booleanType(T) {return false;};
bool booleanType(bool);
template <class T> bool floatType(T) {return false;};
bool floatType(float);
template <class T> bool charType(T) {return false;};
bool charType(char);

inline bool invalidValue(bool aux) {return false;};
inline int invalidValue(int aux) {return -1;};
inline float invalidValue(float aux) {return -1.0;};
inline char invalidValue(char aux) {return '\0';};
inline char * invalidValue(char *aux) {return NULL;};
inline bool validValue(bool aux) {return true;};
inline int validValue(int aux) {return 1;};
inline float validValue(float aux) {return 1.0;};
inline char validValue(char aux) {return '1';};
inline char * validValue(char *aux) {return NULL;};

template <class T> bool isInvalidValue(T val) {
	return (val == invalidValue(val));
}

bool stob(char * str);
bool getValue(bool t, char * str);
int getValue(int t, char * str);
float getValue(float t, char * str);
char getValue(char t, char * str);
char * getValue(char* t, char * str);


template <class T> T getCommandLineParameter(int nc,
		char ** cline,
		char * opttype,
		char * optname) {

	int i;
	int desl;
	T type;
	bool hasname = false;
	bool hasequal = false;
	bool hasvalue = false;
	int optlen = 0;
	if (optname != NULL)
		optlen = strlen(optname);

	for (i=0; i < nc; i++) {
		desl=0;
		hasname=false;
		hasequal=false;
		hasvalue=false;
		char str[1000];
		strcpy(str,cline[i]);
		if (opttype[0] == '1') {
			if (strncmp(cline[i]+desl, optname, optlen) == 0) {
				desl += optlen;
				hasname=true;
			} else
				continue;
		} else if (opttype[0] == '2' && strncmp(cline[i]+desl, optname, optlen) == 0){
				desl += optlen;
				hasname=true;
		};

		if (opttype[1] == '1') {
			if (strlen(cline[i]) == desl) {
				i++;
				desl = 0;
			} else
				return invalidValue(type);
		} else if (opttype[1] == '2' && strlen(cline[i]) == desl) {
			i++;
			desl = 0;
		};

		if (opttype[2] == '1') {
			if (strncmp(cline[i]+desl, "=", 1) == 0) {
 				desl++;
				hasequal = true;
			} else
				return invalidValue(type);
		} else if (opttype[2] == '2' && strncmp(cline[i]+desl, "=", 1) == 0) {
			desl++;
			hasequal = true;
		};

		if (hasequal) {
			if (opttype[3] == '1') {
				if (strlen(cline[i]) == desl) {
					i++;
					desl = 0;
				} else
					return invalidValue(type);
			} else if (opttype[3] == '2' && strlen(cline[i]) == desl) {
				i++;
				desl = 0;
			};
		};

		if (opttype[4] == '1') {
			if (strlen(cline[i]+desl) > 0) {
				hasvalue=true;
				return getValue(type, cline[i]+desl);
			} else
				return invalidValue(type);
		} else if (opttype[4] == '2' && strlen(cline[i]+desl) > 0){
				hasvalue=true;
				return getValue(type, cline[i]+desl);
		}

		if (hasname && booleanType(type))
			return validValue(type);
	};
	return invalidValue(type);
};
template <class T> T getCommandLineParameter(int nc,
		char ** cline,
		char * opttype,
		char * optname,
		bool & ok) {

	int i;
	int desl;
	T type = NULL;
	bool hasname = false;
	bool hasequal = false;
	bool hasvalue = false;

	ok = false;

	int optlen = 0;
	if (optname != NULL)
		optlen = strlen(optname);

	for (i=0; i < nc; i++) {
		desl=0;
		hasname=false;
		hasequal=false;
		hasvalue=false;
		char str[1000];
		strcpy(str,cline[i]);
		if (opttype[0] == '1') {
			if (strncmp(cline[i]+desl, optname, optlen) == 0) {
				desl += optlen;
				hasname=true;
			} else
				continue;
		} else if (opttype[0] == '2' && strncmp(cline[i]+desl, optname, optlen) == 0){
				desl += optlen;
				hasname=true;
		};

		if (opttype[1] == '1') {
			if (strlen(cline[i]) == desl) {
				i++;
				desl = 0;
			} else
				return invalidValue(type);
		} else if (opttype[1] == '2' && strlen(cline[i]) == desl) {
			i++;
			desl = 0;
		};

		if (opttype[2] == '1') {
			if (strncmp(cline[i]+desl, "=", 1) == 0) {
 				desl++;
				hasequal = true;
			} else
				return invalidValue(type);
		} else if (opttype[2] == '2' && strncmp(cline[i]+desl, "=", 1) == 0) {
			desl++;
			hasequal = true;
		};

		if (hasequal) {
			if (opttype[3] == '1') {
				if (strlen(cline[i]) == desl) {
					i++;
					desl = 0;
				} else
					return invalidValue(type);
			} else if (opttype[3] == '2' && strlen(cline[i]) == desl) {
				i++;
				desl = 0;
			};
		};

		if (opttype[4] == '1') {
			if (strlen(cline[i]+desl) > 0) {
				hasvalue=true;
				ok = true;
				return getValue(type, cline[i]+desl);
			} else
				return invalidValue(type);
		} else if (opttype[4] == '2' && strlen(cline[i]+desl) > 0){
				hasvalue=true;
				ok = true;
				return getValue(type, cline[i]+desl);
		}

		if (hasname && booleanType(type)) {
			ok = true;
			return validValue(type);
		};
	};
	return invalidValue(type);
};
template <class T> T getCommandLineParameterN(int nc,
		char ** cline,
		int n) {

	T type = NULL;

	if (n < 0 || n > nc)
		return invalidValue(type);

	return getValue(type, cline[n]);
}
template <class T> T getCommandLineParameterN(int nc,
		char ** cline,
		int n,
		bool & ok) {

	T type = NULL;

	ok = false;

	if (n < 0 || n > nc)
		return invalidValue(type);

	ok = true;

	return getValue(type, cline[n]);
}

#endif
