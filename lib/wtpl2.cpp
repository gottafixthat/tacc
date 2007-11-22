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

$Id: wtpl2.cc,v 1.6 2004/01/15 08:44:49 werd Exp $ */


#include <cstdio>
#include "wtpl2.h"


//**********************************************************
// TPars
//**********************************************************


void TPars::assign(string inKey, string *const inValue) {
	this->key = inKey;
	this->value = *inValue;
}


void TPars::assign(string inKey, string inValue) {
	this->key = inKey;
		/*
		if (this->isErase && this->value) {
			delete this->value;
		}
		*/
	this->value = inValue;
}


void TPars::assign(TPars* const pars) {
	this->key = pars->key;
	/*
	if (this->isErase && this->value) {
		delete this->value;
	}
	*/
	this->value = pars->value;
}


void TPars::setNext(TPars* inNext) {
	if (this->next) {
		this->next->setNext(inNext);
	} else {
		this->next = inNext;
	}
}



//**********************************************************
// TParsCollect
//**********************************************************


void TParsCollect::insert(string inKey, string inValue) {
	TPars* pars;
	pars = new TPars(inKey, inValue, this->isErase);
	this->insert(pars);
}


void TParsCollect::insert(TPars *const pars) {
	if (this->collect) {
		this->collect->setNext(pars);
	} else {
		this->collect = pars;
	}
}


TPars* TParsCollect::isKey(string inKey) {
	TPars *i;
	for (i = this->collect; i; i = i->getNext()) {
		if (i->getKey() == inKey) return i;
	}
	return NULL;
}


void TParsCollect::assign(string inKey, string inValue) {
/*
	TPars *pars;
	pars = new TPars(inKey, inValue, this->isErase);
	this->assign(pars);
*/
	TPars *i, *pars;
	i = this->isKey(inKey);
	if (i) {	
		i->assign(inKey, inValue);
	} else {
		pars = new TPars(inKey, inValue, this->isErase);
		this->insert(pars);
	}
}

void TParsCollect::setValue(string inValue) {
	TPars *i;
	for (i = this->collect; i; i = i->getNext()) {
		i->assign(i->getKey(), inValue);
	}
}



//**********************************************************
// wtplKnot
//**********************************************************

wtplKnot::wtplKnot() {
}

wtplKnot::~wtplKnot() {
	//cout << "delete knot: '" << this->name << "'" << endl;
	//cout << "count sub-knot's: " << this->blockCount << endl;
	for (unsigned int i = 0; i < this->blockCount; i++) {	
		delete this->next[i];
	}
}


wtplKnot::wtplKnot(string inName, string inOrig, wtplBlockList *inKnotList) {
	this->name = inName;
	this->orig = "";
	this->out  = "";
	this->blockCount = 0;
	inKnotList->assign(inName, this);
	

	string exp = wtplSBeginStart + "(" + wtplSName + ")" + wtplSBeginEnd;
	string tmpOrig = inOrig;
	unsigned int blockOffset = 0;

	pcre *reg;
	pcre *regBlock;
	const char *error;
	int erroffset, rc;
	int ovector[OVECCOUNT];
	char *c_reg = (char*)exp.c_str();
	reg = pcre_compile(c_reg, 0, &error, &erroffset, NULL);
	
	while(1) {
		rc = pcre_exec(reg, NULL, (char*)tmpOrig.c_str(), tmpOrig.size(), blockOffset, 0, ovector, OVECCOUNT);
		if (rc > 0) {
			string nameBlock = tmpOrig.substr(ovector[2], ovector[3] - ovector[2]);
			unsigned int startBegin = ovector[0];
			unsigned int startEnd = ovector[1];
			
			string expBlock = wtplSEndStart + nameBlock + wtplSEndEnd;
			regBlock = pcre_compile((char*)expBlock.c_str(), 0, &error, &erroffset, NULL);
			rc = pcre_exec(regBlock, NULL, (char*)tmpOrig.c_str(), tmpOrig.size(), 0, 0, ovector, OVECCOUNT);
			if (rc > 0) {
			//Polnoe sovpodenie
	  			unsigned int endBegin = ovector[0];
	  			unsigned int endEnd = ovector[1];
	  			pcre_free(regBlock);
				
				string newBlockContent;
				string newBlockName;
				wtplKnot* newBlockPointer;
				newBlockName = ((this->name == "") ? nameBlock : this->name + "." + nameBlock);
				newBlockContent = tmpOrig.substr(startEnd, endBegin - startEnd);
				newBlockPointer = new wtplKnot(newBlockName, newBlockContent, inKnotList);
				this->setNext(newBlockPointer);
				
				this->orig += tmpOrig.substr(blockOffset, startBegin - blockOffset) + wtplSKnotFirst + newBlockName + wtplSKnotLast;
				
				if (endEnd + 1 < tmpOrig.size()) {
					blockOffset = endEnd;
				} else break;
			} else {
				pcre_free(regBlock);
				//tut esli ne naydeno okonchanie blocka
				this->orig += tmpOrig.substr(blockOffset, startEnd - blockOffset);
				if (startEnd < tmpOrig.size()) {
					blockOffset = startEnd;
				} else break;
			}
		} else {
			this->orig += tmpOrig.substr(blockOffset, tmpOrig.size() - blockOffset);
			break;
		}
	} //while
	pcre_free(reg);
}

void wtplKnot::show() {
	cout << "Block '" << this->name << "'" << endl;
	cout << "Orig source:" << endl;
	cout << "------------" << endl;
	cout << this->orig << endl;
	cout << "Out source:" << endl;
	cout << "-----------" << endl;
	cout << (this->out == "" ? "NULL" : this->out) << endl;
}

void wtplKnot::parse(TPars *inPars) {
	string relay = this->orig;
	//set variables
	if (inPars != NULL) {
		for (TPars *p = inPars; p != NULL; p = p->getNext()) {
			relay = this->setVar(relay, p);
		}
	}

	//load child knot's
	for (unsigned int i = 0; i < this->blockCount; i++) {
		string ttt, name;
		name = wtplSKnotFirst + this->next[i]->getName() + wtplSKnotLast;
		ttt = this->next[i]->get_text_and_clear();
		relay.replace(relay.find(name), name.size(), ttt);
	}
	this->out += relay;
}

string wtplKnot::setVar(string inRelay, TPars *inPars) {
	string exp, relay;
	int pos;
	relay = inPars->getValue();
	exp = wtplSVarStart + inPars->getKey() + wtplSVarEnd;
	while (inRelay.find(exp) < inRelay.size()) {
		pos = inRelay.find(exp);
		if (pos != 0) {
			if (inRelay.substr(pos - 1, 1) != "\\") {
				inRelay.replace(pos, exp.size(), relay);
			}
		} else {
			inRelay.replace(pos, exp.size(), relay);
		}
	}
	relay = inRelay;
	return relay;
}

string wtplKnot::text() {
	return this->out;
}

void wtplKnot::setNext(wtplKnot* inNext) {
	if (this->blockCount < wtplSubBlockCount) {
		this->next[this->blockCount] = inNext;
		this->blockCount++;
	}
}

void wtplKnot::syntax(unsigned int i = 0) {
	for (unsigned int x = 0; x < i; x++) {
		cout << "  ";
	}
	cout << "{" << this->name << "}" << endl;;
	for (unsigned int xx = 0; xx < blockCount; xx++) {
		this->next[xx]->syntax(i+1);
	}
}

string wtplKnot::get_text_and_clear() {
	string tmpOut = this->out;
	this->out = "";
	return tmpOut;
}


//**********************************************************
// wtplBlockList
//**********************************************************

int wtplBlockList::isKey(string inKey) {
	int out = -1;
	for (int i = 0; i < this->count(); i++) {
		if (inKey == this->next[i]->getName()) {
			out = i;
			break;
		}
	}
	return out;
}

void wtplBlockList::assign(string inKey, wtplKnot *inKnot) {
	int i = this->isKey(inKey);
	if (i >= 0) {
		this->next[i] = inKnot;
	} else {
		if (this->blockCount < wtplMaxBlockCount) {
			this->next[this->blockCount] = inKnot;
			this->blockCount++;
		}
	}
}


//**********************************************************
// wtpl main object
//**********************************************************

wtpl::wtpl() {
	this->vars = new TParsCollect();
	this->knots = NULL;
	this->knotList = new wtplBlockList();
}

wtpl::~wtpl () {
	delete this->vars;
	delete this->knots;
	delete this->knotList;
}

void wtpl::loadFromStr(string inOrig) {
	if (this->knots != NULL) delete this->knots;
	this->knots = new wtplKnot("", inOrig, this->knotList);
}

void wtpl::assign(string inKey, string inValue) {
	this->vars->assign(inKey, inValue);
}

void wtpl::parse(string inName) {
	int i = this->knotList->isKey(inName);
	if (i >= 0) {
		if (this->knotList->next[i]) {
		}
		this->knotList->next[i]->parse(vars->collect);
	}
}

string wtpl::text(string inName) {
	int i = this->knotList->isKey(inName);
	if (i >= 0) {
		return this->knotList->next[i]->text();
	}
	return "";
}

string wtpl::text() {
	int i = this->knotList->isKey("");
	if (this->knotList->next[i]->text() == "") this->parse("");
	return this->text("");
}

void wtpl::showVars() {
	TPars *i;
	cout << "Variables:" << endl;
	for (i = this->vars->collect; i != NULL; i = i->getNext()) {
		cout << i->getKey() << " = " << i->getValue() << endl;
	}
}

void wtpl::loadFromFile(string inFileName) {
	string inOrig = "";	
	
	FILE* fd;
	fd = fopen(inFileName.c_str(), "r");
	if (fd) {
		char cz;
		while ((cz = fgetc(fd)) != EOF) {
			inOrig += cz;
		}
		fclose(fd);
	}
	
	this->loadFromStr(inOrig);	
}
