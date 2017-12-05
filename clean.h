/*
 * clean.h
 *
 *  Created on: Nov 3, 2017
 *      Author: Peter
 */
#ifndef CLEAN_H_
#define CLEAN_H_
/*
 * main.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: Ryanc
 */
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_set>
using namespace std;

void clean(ifstream &a, ofstream &b);

void clean(ifstream &a, ofstream &b){
	string line;
	while(getline(a,line)){
		if(line.find("|||")!= std::string::npos){
			b << "||| \n";
			getline(a,line);
		}
		string newline;
		regex r("[^A-Za-z0-9']"), r2("\\s{2,}"), r3("what's"), r4("\\'s"), r5("\\'ve"), r6("can't"), r7("n't"), r8("i'm"), r9("\\'re"), r10("\\'d"), r11("\\'ll"), r12("'"), r13(":"), r14(" e g "), r15(" i e "), r16(" u s "), r17("\\0s"), r18("e - mail"), r19(" j k "), r20("\\({3}[\\S]+\\){3}"), r21("abcdefghijklmnopqrstuvxyz");

		for(int i= 0; line[i]; i++)
			line[i]= tolower(line[i]);
		line= regex_replace(line, r20, "abcdefghijklmnopqrstuvxyz");
		line= regex_replace(line, r21, "((()))");
		line= regex_replace(line, r, " ");
		line= regex_replace(line, r3, "what is ");
		line= regex_replace(line, r4, " ");
		line= regex_replace(line, r5, " have ");
		line= regex_replace(line, r6, "cannot ");
		line= regex_replace(line, r7, " not ");
		line= regex_replace(line, r8, "i am ");
		line= regex_replace(line, r9, " are ");
		line= regex_replace(line, r10, " would ");
		line= regex_replace(line, r11, " will ");
		line= regex_replace(line, r12, " ");
		line= regex_replace(line, r13, " : ");
		line= regex_replace(line, r14, " eg " );
		line= regex_replace(line, r15, " ie " );
		line= regex_replace(line, r16, " us " );
		line= regex_replace(line, r17, "0" );
		line= regex_replace(line, r18, "email");
		line= regex_replace(line, r19, " jk ");
		line= regex_replace(line, r2, " ");

		b << line << "\n";
	}
}



#endif /* CLEAN_H_ */
