/*
+-----------------------------------------------------------------------------+
| WTPL. A text template C++ library.                                          |
+-----------------------------------------------------------------------------+
| Gives an opportunity of change of design of your pages (HTML, etc.)         |
| without recompilation of the program.                                       |
| Copyright (C) 2004 Stepan Volkov                                            |
+ ----------------------------------------------------------------------------+
| This program is free software; you can redistribute it and/or               |
| modify it under the terms of the GNU General Public License                 |
| as published by the Free Software Foundation; either version 2              |
| of the License, or (at your option) any later version.                      |
|                                                                             |
| This program is distributed in the hope that it will be useful,             |
| but WITHOUT ANY WARRANTY; without even the implied warranty of              |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               |
| GNU General Public License for more details.                                |
|                                                                             |
| You should have received a copy of the GNU General Public License           |
| along with this program; if not, write to the Free Software                 |
| Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. |
+-----------------------------------------------------------------------------+
| Autors: Stepan Volkov <stepan_werd@sourceforge.net>                         |
+-----------------------------------------------------------------------------+

$Id: wtpl2.h,v 1.5 2004/01/14 11:55:53 werd Exp $ */


#include <string>
#include <iostream>
#include <pcre.h>

using namespace std;


class TPars {
	string key;
	string value;
	TPars *next;
	bool isErase;
public:
	TPars(bool inErase = true) {
		next = NULL;
		isErase = inErase;
		value = "";
	}
	TPars(string inKey, string inValue, bool inErase = true) {
		this->assign(inKey, &inValue); 
		this->next = NULL;
		this->isErase = inErase;
	}
	~TPars() {
		if (this->next != NULL) {
			delete this->next;
		}
	}
	void assign(string inKey, string inValue);
	void assign(string inKey, string *const inValue);
	void assign(TPars* const pars);
	string getKey() {return this->key; }
	string getValue() {return this->value;}
	TPars* getNext() {return this->next;}
	void setNext(TPars* inNext);
};


class TParsCollect {
	void insert(string inKey, string inValue);
	void insert(TPars* const pars);
	bool isErase;
public:
	TPars *collect;
	TParsCollect(bool inErase = true) {collect = NULL;isErase = inErase;}
	~TParsCollect() {
		if (this->collect != NULL) {
			delete this->collect;
		}
	}
	void assign(string inKey, string inValue);
	TPars* isKey(string inKey);
	void setValue(string inValue);
};


// Constant in PCRE syntax
const string wtplSBeginStart = "<!--[ ]*BEGIN:[ ]*";
const string wtplSBeginEnd   = "[ ]*-->";
const string wtplSEndStart   = "<!--[ ]*END:[ ]*";
const string wtplSEndEnd     = "[ ]*-->";
const string wtplSName       = "[a-zA-Z0-9\\-_\\.\\:]+";

// Constant in PLAIN syntax
const string wtplSVarStart   = "@--";
const string wtplSVarEnd     = "--@";
const string wtplSKnotFirst  = "<##";
const string wtplSKnotLast   = "##>";

const int OVECCOUNT = 30;
const unsigned int wtplSubBlockCount = 32;  //Количество дочерних блоков
const int wtplMaxBlockCount = 256; //Максимальное кол-во блоков в шаблоне

class wtplBlockList;

class wtplKnot {
	string name;
	string orig;
	string out;
	wtplKnot *next[wtplSubBlockCount];
	unsigned int blockCount;
public:
	wtplKnot();
	wtplKnot(string inName, string inOrig, wtplBlockList *inKnotList);
	~wtplKnot();
	string getName() {return this->name;}
	void show();
	void parse(TPars *inPars);
	void setNext(wtplKnot* inNext);
	void syntax(unsigned int i);
	string setVar(string inRelay, TPars *inPars);
	string text();
	string get_text_and_clear();
};


class wtplBlockList {
	
	int blockCount;
public:
	wtplKnot *next[wtplMaxBlockCount]; //временно
	wtplBlockList() {blockCount = 0;}
	~wtplBlockList() {
	}
	int isKey(string inKey);
	void assign(string inKey, wtplKnot *inKnot);
	int count() { return this->blockCount;}
	wtplKnot *operator [](int i) { return next[i];}
};


class wtpl {
	TParsCollect *vars;
public:
	wtplBlockList *knotList;
	wtplKnot *knots;
	wtpl();
	wtpl(string inFileName) {
		this->vars = new TParsCollect();
	   this->knots = NULL;
	   this->knotList = new wtplBlockList();
		this->loadFromFile(inFileName);
	}
	~wtpl();
	void loadFromStr(string inOrig);
	void loadFromFile(string inFileName);
	void syntax() {knots->syntax(0);}
	void assign(string inKey, string inValue);
	void parse(string inName);
	string text(string inName);
	string text();
	void showVars();
};

