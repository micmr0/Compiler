/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "def.yy" /* yacc.c:339  */

#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <stack>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#define INFILE_ERROR 1
#define OUTFILE_ERROR 2
extern "C" int yylex();
extern "C" int yyerror(const char *msg, ...);
using namespace std;
ofstream outfile("trojki.txt");
ofstream outfile2("czworki.txt");
ofstream progASM("program.asm");
stringstream Main;
stringstream Table;
map<string, string>symbolicTable;
map<string, string>tablicaFloat;
map<string, string>tablicaString;
map<string, string>tabTablic;
string variableName;
int liczbaZmiennych = 0;
static int tmpNumber = 1;
static int var = 1;
static int power = 1;
stack <string> cards;
int id = 0;
int id_global = 0;
int id_global_while = 0;
int id_while = 0;
template<typename T> 
string toString(T value)
{
	stringstream sstream;
	sstream << value;
	return sstream.str();
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int ifExist(string arg)
{
	string temp = arg;
	for(map<string,string>::const_iterator it = symbolicTable.begin(); it!= symbolicTable.end();it++)
	{
		cout << "Tablica symboli: " << it->first << " " << it->second << endl;
		if( it->first.compare(temp) == 0)
		{
			//cout << "Znalazlem !!!!" << endl;
			return 1;
		}		
	}
	return 0;
}

int ifExistFloat(string arg)
{
	string temp = arg;
	for(map<string,string>::const_iterator it = tablicaFloat.begin(); it!= tablicaFloat.end();it++)
	{
		cout << "Tablica float: " << it->first << " " << it->second << endl;
		if( it->first.compare(temp) == 0)
		{
			//cout << "Znalazlem !!!!" << endl;
			return 1;
		}		
	}
	return 0;
}


int wykonaj_dzialanie(string arg, string ID)
{
	string arg2 = cards.top();
	cards.pop();
	if(arg == "="){
		outfile << arg << " " <<  ID << " " << arg2 <<endl;
		outfile2 << "$null" << " " << arg << " " <<  ID << " " << arg2 <<endl;
		
		if(symbolicTable[ID] == "float") //zapisanie do float
		{
			if(arg2[0] == '-')
			{
				cout << "liczba jest ujemna" << endl;

				int len = arg2.length();

				arg2 = arg2.substr(1, len);

				cout << "arg2 wynosi " << arg2 << endl;
				
				if( isdigit(arg2[0]) )
				{
					symbolicTable["var"+toString(var)] = "float";
					tablicaFloat["var"+toString(var)] = arg2;

					Main << "\tl.s " << "$f0" << ", "  << "var"+toString(var) << endl;
					Main << "\tneg.s $f0, $f0" << endl;
					Main << "\ts.s $f0, " <<  "var"+toString(var) << endl;

					Main << "\tl.s $f0, " << "var"+toString(var) << endl;
					Main << "\ts.s " << "$f0" << ", "  << ID << endl;
					cout << "jetsem tutaj " << endl;
					var++;
				}
				else
				{
					Main << "\tl.s $f0, " << arg2 << endl;
					Main << "\tneg.s $f0, $f0" << endl;
					Main << "\ts.s " << "$f0" << ", "  << ID << endl;
				}

				return 0;
			}

			
			if( isdigit(arg2[0]) )
			{
				symbolicTable["var"+toString(var)] = "float";
				tablicaFloat["var"+toString(var)] = arg2;

				Main << "\tl.s $f0, " << "var"+toString(var) << endl;
				Main << "\ts.s " << "$f0" << ", "  << ID << endl;
				cout << "jetsem tutaj " << endl;
				var++;
			}
			else
			{
				Main << "\tl.s $f0, " << arg2 << endl;
				Main << "\ts.s " << "$f0" << ", "  << ID << endl;
			}
				
		}

		if(symbolicTable[ID] == "int") // zapisanie do int
		{
			if(arg2[0] == '-')
			{
				cout << "liczba jest ujemna" << endl;

				int len = arg2.length();

				arg2 = arg2.substr(1, len);

				cout << "arg2 wynosi " << arg2 << endl;

				

				if(arg2.find_first_not_of("1234567890") != string::npos) //jak float to potrzebna konwersja
				{
					symbolicTable["var"+toString(var)] = "float";
					tablicaFloat["var"+toString(var)] = arg2;

					Main << "\tl.s " << "$f0" << ", "  << "var"+toString(var) << endl;
					Main << "\tneg.s $f0, $f0" << endl;
					Main << "\ts.s $f0, " <<  "var"+toString(var) << endl;

					Main << "\tl.s " << "$f0" << ", "  << "var"+toString(var) << endl;
					Main << "\tcvt.w.s $f0, $f0" << endl;
					Main << "\tswc1 $f0, " << ID << endl;

				}
				else // jak int to zwykle dodanie np. c 7
				{
					Main << "\tli " << "$t0" << ", "  << arg2 << endl;
					Main << "\tneg $t0, $t0" << endl;
					Main << "\tsw $t0, " << ID << endl;
				}
				
				var++;

				return 0;
			}


			if( isdigit(arg2[0]) ) // jesli liczba
			{
				if(arg2.find_first_not_of("1234567890") != string::npos) //jak float to potrzebna konwersja
				{
					symbolicTable["var"+toString(var)] = "float";
					tablicaFloat["var"+toString(var)] = arg2;

					Main << "\tl.s $f0, " << "var"+toString(var) << endl; //konwersja float na int
					Main << "\tcvt.w.s $f0, $f0" << endl;
					Main << "\tswc1 $f0, " << ID << endl;

					var++;

				}
				else // jak int to zwykle dodanie np. c 7
				{
					Main << "\tli $t0, " << arg2 << endl;
					Main << "\tsw $t0, " << ID << endl;
				}
			}
			else //jesli zmienna np. c tmp2
			{
				Main << "\tl.s $f0, " << arg2 << endl; //konwersja float na int
				Main << "\tcvt.w.s $f0, $f0" << endl;
				Main << "\tswc1 $f0, " << ID << endl;
			}
		}

		return 0;
		
	}else{
		string arg1 =  cards.top();
		cards.pop();
		symbolicTable[("tmp"+toString(tmpNumber))]  = toString("float");
		tablicaFloat[("tmp"+toString(tmpNumber))]  = toString("0");

		liczbaZmiennych++;
		cards.push("tmp"+toString(tmpNumber));
		outfile << arg << " " <<  arg1 << " " << arg2 <<endl;
		outfile2 << ("tmp"+toString(tmpNumber)) << " " << arg << " " <<  arg1 << " " << arg2 <<endl;
		
		if(isdigit(arg1[0]))
		{
				symbolicTable["var"+toString(var)] = "float";
				tablicaFloat["var"+toString(var)] = arg1;
				Main << "\tl.s " << "$f0" << ", "  << "var"+toString(var) << endl;
				var++;

		}else
		{
			if( ifExist(arg1) )
			{
				if(symbolicTable[arg1] == "float")
				{
					Main << "\tl.s $f0, " << arg1 << endl;
				}
				else
				{
					Main << "\tlw $t0, " << arg1 << endl;
					Main << "\tmtc1 $t0, $f0" << endl;
					Main << "\tcvt.s.w $f0, $f0" << endl;

				}

			}else
			{
				if(arg1[0] == '-')
				{
					cout << "liczba jest ujemna" << endl;

					int len = arg1.length();

					arg1 = arg1.substr(1, len);

					cout << "arg1 wynosi " << arg1 << endl;

					symbolicTable["var"+toString(var)] = "float";
					tablicaFloat["var"+toString(var)] = arg1;

					Main << "\tl.s " << "$f0" << ", "  << "var"+toString(var) << endl;
					Main << "\tneg.s $f0, $f0" << endl; 
					
					var++;
				}
				else
				{
					cout << "Zmienna o nazwie " << arg1  << " nie istnieje ! Blad\n" << endl;
					return 1;
				}
				
			}
		}

		if(isdigit(arg2[0]))
		{
				symbolicTable["var"+toString(var)] = "float";
				tablicaFloat["var"+toString(var)] = arg2;
				Main << "\tl.s " << "$f1" << ", "  << "var"+toString(var) << endl;
				var++;		
		}else
		{
			if( ifExist(arg2) )
			{
				if(symbolicTable[arg2] == "float")
				{
					Main << "\tl.s $f1, " << arg2 << endl;
				}
				else //jak nie float to potrzebna konwersja
				{
					Main << "\tlw $t0, " << arg2 << endl;
					Main << "\tmtc1 $t0, $f1" << endl;
					//Main << "\tmfc1 $t0, $f0" << endl;
					Main << "\tcvt.s.w $f1, $f1" << endl;
				}

			}else
			{
				if(arg2[0] == '-')
				{
					cout << "liczba jest ujemna" << endl;

					int len = arg2.length();

					arg2 = arg2.substr(1, len);

					cout << "arg2 wynosi " << arg2 << endl;

					symbolicTable["var"+toString(var)] = "float";
					tablicaFloat["var"+toString(var)] = arg2;

					Main << "\tl.s " << "$f1" << ", "  << "var"+toString(var) << endl;
					Main << "\tneg.s $f1, $f1" << endl; 
					
					var++;
				}
				else
				{
					cout << "Zmienna o nazwie " << arg2  << " nie istnieje ! Blad\n" << endl;
					return 1;
				}
			}
		}
		
		if(arg == "+")
		{
			Main << "\tadd.s " << "$f0, " << "$f0, " << "$f1" << endl;
			Main << "\ts.s " << "$f0" << ", "  << "tmp"+toString(tmpNumber) << endl;
		}
		if(arg == "-")
		{
			Main << "\tsub.s " << "$f0, " << "$f0, " << "$f1" << endl;
			Main << "\ts.s " << "$f0" << ", "  << "tmp"+toString(tmpNumber) << endl;
		}
		if(arg == "*")
		{
			Main << "\tmul.s " << "$f0, " << "$f0, " << "$f1" << endl;
			Main << "\ts.s " << "$f0" << ", "  << "tmp"+toString(tmpNumber) << endl;
		}
		if(arg == "/")
		{
			Main << "\tdiv.s " << "$f0, " << "$f0, " << "$f1" << endl;
			Main << "\ts.s " << "$f0" << ", "  << "tmp"+toString(tmpNumber) << endl;
		}

		if(arg == "^")
		{
			/*symbolicTable["var"+toString(var)] = "float";
			tablicaFloat["var"+toString(var)] = "0";
			symbolicTable["potega"+toString(power)] = "float";

			symbolicTable["licznik"] = "float";
			tablicaFloat["licznik"] = "0";

			string warunek1 = toString("var"+toString(var));
			cards.push();

			string warunek2 = arg2;
			cards.push();

			++id_while;
			sprawdz_warunek("<", "while");

			Main << "" << endl;
			Main << "\tmul.s " << "$f0, " << "$f0, " << "$f0" << endl;
			Main << "\ts.s " << "$f0" << ", "  << "potega"+toString(power) << endl;
			Main << "l.s $f0, " << "var"+toString(var) << endl;


			zakoncz_while();
			--id_while;*/
		}

		tmpNumber++;

		return 0;
	}
}



int operacja_print(string type)
{
	if( type.compare("TAB") == 0 )
	{
		string tab = cards.top();
		cards.pop();
		string it = cards.top();
		cards.pop();

		int len = tab.length();

		tab = tab.substr(1, len);

		if(isdigit(it[0])){
			Main << "\tli $t1," << it << endl;
		}
		else{
			Main << "\tlw $t1," << it << endl;
		}

		for(map<string,string>::const_iterator it = tabTablic.begin(); it != tabTablic.end(); it++)
		{
			if(it->first == tab)
			{
				if(it->second.compare("@float") == 0)
				{
					cout << "jestem tutaj XXXXXXXXXXXXXXXXXXX" << endl;
					symbolicTable["var"+toString(var)] = "float";
					tablicaFloat["var"+toString(var)] = "0";
					//Date << p << ": .float 0" << endl;
					Main << "\tla $t0,"<< tab << endl;
					Main << "\tmul $t1,$t1,4" << endl;
					Main << "\tadd $t0,$t0,$t1" << endl;

					Main << "\tl.s $f1,($t0)" << endl;
					Main << "\ts.s $f1," << "var"+toString(var) << endl;

					Main << "\tli " << "$v0,2" << endl;
					Main << "\tl.s " << "$f12," << "var"+toString(var) <<endl;
					Main << "\tsyscall" << endl;

					var++;
				}
				else
				{
					symbolicTable["var"+toString(var)] = "int";


					Main << "\tla $t0,"<< tab << endl;
					Main << "\tmul $t1,$t1,4" << endl;
					Main << "\tadd $t0,$t0,$t1" << endl;
					Main << "\tlw $t1,($t0)" << endl;
					Main << "\tsw $t1," << "var"+toString(var) << endl;
					Main << "\tli $v0, 1" << endl;  // $system call code for print_str
    				Main << "\tlw " << "$a0," << "var"+toString(var) <<endl;
    				Main << "\tsyscall" << endl; 

    				var++;
				}
				
				return 1;
			}
		}

		
	}

	if( type.compare("PARAM") == 0 )
	{
		string value = toString(cards.top());
		cards.pop(); 
		liczbaZmiennych++;

		cout << "pobrany string to " << value << endl;

		symbolicTable["var"+toString(var)] = toString("string");
		tablicaString["var"+toString(var)] = value;

		Main << "\tli $v0, 4" << endl;  // $system call code for print_str
		Main << "\tla $a0, "<< "var"+toString(var) << endl;
		Main << "\tsyscall" << endl; 

		var++;

		return 0;
	}

	string arg = toString(cards.top());
	cards.pop(); 
	liczbaZmiennych++;

	if((arg[0]>=65 && arg[0]<=122) ) //jesli pierwszy znak to litera(nazwa zmiennej)
	{
			string typ;
			
		if( ifExist(arg) )
		{
			if(symbolicTable[arg] == "int")
			{
				Main << "\tli $v0, 1" << endl;
				Main << "\tlw $t0, " << arg << endl;
			    Main << "\tadd $a0, $t0, $zero" << endl;  
			    Main << "\tsyscall" << endl;

			}
			
			if(symbolicTable[arg] == "float")
			{
				Main << "\tli $v0, 2" << endl;
				Main << "\tl.s $f12, " << arg << endl;
				Main << "\tsyscall" << endl;
			}

			if((symbolicTable[arg] == "string") || symbolicTable[arg] == "string\n")
			{
				Main << "\tli $v0, 4" << endl;  // $system call code for print_str
    			Main << "\tla $a0, "<< arg << endl;
    			Main << "\tsyscall" << endl; 
			}


		}
		else
		{
			cout << "Zmienna o takiej nazwie nie istnieje ! Blad\n" << endl;
			return 1;
		}
					
			
						
	}
	else //wypisanie liczby
	{
			if( arg.find_first_not_of("1234567890") != string::npos ) //float
			{
				symbolicTable["tmp"+toString(tmpNumber)]  = toString("float");
				tablicaFloat["tmp"+toString(tmpNumber)] = arg;
				
				Main << "\tli $v0, 2" << endl;
				Main << "\tla $t0, " << "tmp"+toString(tmpNumber) << endl;
				Main << "\tl.s $f12 ($t0)" << endl;
				Main << "\tsyscall" << endl;

				tmpNumber++;

			}
			else //int
			{
				Main << "\tli $v0, 1" << endl;

				Main << "\tadd $a0, $t0, " << arg << endl;
				Main << "\tsyscall" << endl;
			}
	}

	return 0;
}

int deklaracja_int(string arg)
{

	if(arg[0] == 48)
	{
		string variable = toString(cards.top());
		cards.pop(); 
		liczbaZmiennych++;

		//cout << "variable wynosi: " << variable << endl;

		for(map<string,string>::const_iterator it = symbolicTable.begin(); it!= symbolicTable.end();it++)
		{
			cout << "stos1" << it->first << endl;
			if((it->first == variable))
			{
				printf("Zmienna o takiej nazwie juz istnieje! Blad \n");
				return 1;
			}		
		}

		symbolicTable[variable]  = toString("int");
		
		return 0;
	}

	if(arg[0] != 48)
	{
		string variable = arg;

		liczbaZmiennych++;

		for(map<string,string>::const_iterator it = symbolicTable.begin(); it!= symbolicTable.end();it++)
		{
			//cout << "stos2" << it->first << endl;
			if((it->first == variable))
			{
				printf("Zmienna o takiej nazwie juz istnieje! Blad \n");
				return 1;
			}		
		}

		symbolicTable[variable]  = toString("int");
		
		return 0;
	}
}

int deklaracja_float(string arg)
{
	if(arg[0] == 48)
	{
		string variable = toString(cards.top());
		cards.pop(); 
		liczbaZmiennych++;

		for(map<string,string>::const_iterator it = tablicaFloat.begin(); it!= tablicaFloat.end();it++)
		{
			cout << "stos1" << it->first << endl;
			if((it->first == variable))
			{
				printf("Zmienna o takiej nazwie juz istnieje! Blad \n");
				return 1;
			}		
		}

		tablicaFloat[variable]  = toString("0");
		symbolicTable[variable] = "float";
		//cout << "jestem tutaj" << endl;
		return 0;
	}

	if(arg[0] != 48)
	{
		string variable = arg;

		liczbaZmiennych++;

		for(map<string,string>::const_iterator it = symbolicTable.begin(); it!= symbolicTable.end();it++)
		{
			//cout << "stos2" << it->first << endl;
			if((it->first == variable))
			{
				printf("Zmienna o takiej nazwie juz istnieje! Blad \n");
				return 1;
			}		
		}

		symbolicTable[variable]  = toString("float");
		tablicaFloat[variable] = toString("0");
		
		return 0;
	}

}

int deklaracja_string()
{
	string value = toString(cards.top());
	cards.pop(); 
	liczbaZmiennych++;

	string variable = toString(cards.top());
	cards.pop(); 

	cout << "variable wynosi: " << variable << endl;
	

		for(map<string,string>::const_iterator it = symbolicTable.begin(); it!= symbolicTable.end();it++)
		{
			//cout << "stos2" << it->first << endl;
			if((it->first == variable))
			{
				printf("Zmienna o takiej nazwie juz istnieje! Blad \n");
				return 1;
			}		
		}

		symbolicTable[variable] = toString("string");
	

		tablicaString[variable] = value;


	return 0;
}


int sprawdz_warunek(string warunek, string typ)
{
	string arg2 = toString(cards.top());
	cards.pop();

	string arg1 = toString(cards.top());
	cards.pop();

	string arg1_type;
	string arg2_type;

	cout << "Bede sprawdzal, czy " << arg1 << " jest " << warunek << " od " << arg2 << endl;

	if( isdigit(arg1[0]) ) // jesli liczba
	{
		if(arg1.find_first_not_of("1234567890") != string::npos) //jak float to potrzebna konwersja
		{
			symbolicTable["var"+toString(var)] = "float";
			tablicaFloat["var"+toString(var)] = arg1;

			Main << "\tl.s $f4, " << "var"+toString(var) << endl;
			//Main << "\tcvt.s.w $f0, $f0" << endl;

			var++;

		}
		else // jak int to zwykle dodanie np. c 7
		{
			symbolicTable["var"+toString(var)] = "float";
			tablicaFloat["var"+toString(var)] = arg1;

			Main << "\tl.s $f4, " << "var"+toString(var) << endl; //konwersja float na int
			Main << "\tcvt.w.s $f4, $f4" << endl;
			var++;
		}
	}
	else //jesli zmienna np. c 
	{
		if( ifExistFloat(arg1) )
		{
			Main << "\tl.s $f4, " << arg1 << endl; //konwersja float na int
			arg1_type = "float";
		}
		else
		{
			Main << "\tlw $t0, " << arg1 << endl;
			Main << "\tmtc1 $t0, $f4" << endl;
			Main << "\tcvt.s.w $f4, $f4" << endl;
			arg1_type = "int";
		}
		
	}


	if( isdigit(arg2[0]) ) // jesli liczba
	{
		if(arg1.find_first_not_of("1234567890") != string::npos) //jak float to potrzebna konwersja
		{
			symbolicTable["var"+toString(var)] = "float";
			tablicaFloat["var"+toString(var)] = arg2;

			Main << "\tl.s $f5, " << "var"+toString(var) << endl;
			var++;

		}
		else // jak int to zwykle dodanie np. c 7
		{
			symbolicTable["var"+toString(var)] = "float";
			tablicaFloat["var"+toString(var)] = arg2;

			Main << "\tl.s $f5, " << "var"+toString(var) << endl; //konwersja float na int
			Main << "\tcvt.w.s $f5, $f5" << endl;

			var++;
		}
	}
	else //jesli zmienna np. c
	{
		if( ifExistFloat(arg2) )
		{
			Main << "\tl.s $f5, " << arg2 << endl; //konwersja float na int
			arg2_type = "float";
		}
		else
		{
			Main << "\tlw $t1, " << arg2 << endl;
			Main << "\tmtc1 $t1, $f5" << endl;
			Main << "\tcvt.s.w $f5, $f5" << endl;
			arg2_type = "int";
		}
	}

	if( typ == "if")
	{
		if(warunek == "==")
		{
			Main << "c.eq.s  $f4, $f5" << endl;
			Main << "bc1t LABEL" << id_global <<  id << endl;
			Main << "bc1f ELSE"<< id_global <<  id << endl;
			Main << "LABEL" << id_global <<  id << ": " << endl;
			Main << endl;
		}

		if(warunek == "!=")
		{
			Main << "c.eq.s  $f4, $f5" << endl;
			Main << "bc1t ELSE" << id_global <<  id << endl;
			Main << "bc1f LABEL" << id_global <<  id << endl;
			Main << "LABEL" << id_global <<  id << ": " << endl;
			Main << endl;
		}


		if(warunek == "<")
		{
			

			Main << "c.lt.s  $f4, $f5" << endl;
			Main << "bc1t LABEL" << id_global <<  id << endl;
			Main << "bc1f ELSE"<< id_global <<  id << endl;
			Main << "LABEL" << id_global <<  id << ": " << endl;
			Main << endl;
		}

		if(warunek == ">")
		{
			Main << "c.le.s  $f4, $f5" << endl;
			Main << "bc1t ELSE" << id_global <<  id << endl;
			Main << "bc1f LABEL" << id_global <<  id << endl;
			Main << "LABEL" << id_global <<  id << ": " << endl;
			Main << endl;
		}

		if(warunek == "<=")
		{
			Main << "c.le.s  $f4, $f5" << endl;
			Main << "bc1t LABEL" << id_global <<  id << endl;
			Main << "bc1f ELSE"<< id_global <<  id << endl;
			Main << "LABEL" << id_global <<  id << ": " << endl;
			Main << endl;
		}

		if(warunek == ">=")
		{
			Main << "c.lt.s  $f4, $f5" << endl;
			Main << "bc1t ELSE" << id_global <<  id << endl;
			Main << "bc1f LABEL" << id_global <<  id << endl;
			Main << "LABEL" << id_global <<  id << ": " << endl;
			Main << endl;
		}

	}

	if( typ == "while" )
	{
		if(warunek == "==")
		{
			Main << "WHILE" <<  id_global_while << id_while << ": " << endl;

			if(arg1_type == "float")
			{
				Main << "\tl.s $f4, " << arg1 << endl;
			}
			if(arg1_type == "int")
			{
				Main << "\tlw $t0, " << arg1 << endl;
				Main << "\tmtc1 $t0, $f4" << endl;
				Main << "\tcvt.s.w $f4, $f4" << endl;
			}

			if(arg2_type == "float")
			{
				Main << "\tl.s $f5, " << arg2 << endl; 
			}
			if(arg2_type == "int")
			{
				Main << "\tlw $t1, " << arg2 << endl;
				Main << "\tmtc1 $t1, $f5" << endl;
				Main << "\tcvt.s.w $f5, $f5" << endl;
			}

			Main << "c.eq.s  $f4, $f5" << endl;
			Main << "\tbc1f OUT" << id_global_while << id_while << endl;
			Main << endl;
		}

		if(warunek == "!=")
		{
			Main << "WHILE" << id_global_while << id_while << ":" << endl;

			if(arg1_type == "float")
			{
				Main << "\tl.s $f4, " << arg1 << endl;
			}
			if(arg1_type == "int")
			{
				Main << "\tlw $t0, " << arg1 << endl;
				Main << "\tmtc1 $t0, $f4" << endl;
				Main << "\tcvt.s.w $f4, $f4" << endl;
			}

			if(arg2_type == "float")
			{
				Main << "\tl.s $f5, " << arg2 << endl; 
			}
			if(arg2_type == "int")
			{
				Main << "\tlw $t1, " << arg2 << endl;
				Main << "\tmtc1 $t1, $f5" << endl;
				Main << "\tcvt.s.w $f5, $f5" << endl;
			}

			Main << "c.eq.s  $f4, $f5" << endl;
			Main << "\tbc1f OUT" << id_global_while << id_while << endl;
			Main << endl;
		}


		if(warunek == "<")
		{
			Main << "WHILE" << id_global_while << id_while << ":" << endl;

			if(arg1_type == "float")
			{
				Main << "\tl.s $f4, " << arg1 << endl;
			}
			if(arg1_type == "int")
			{
				Main << "\tlw $t0, " << arg1 << endl;
				Main << "\tmtc1 $t0, $f4" << endl;
				Main << "\tcvt.s.w $f4, $f4" << endl;
			}

			if(arg2_type == "float")
			{
				Main << "\tl.s $f5, " << arg2 << endl; 
			}
			if(arg2_type == "int")
			{
				Main << "\tlw $t1, " << arg2 << endl;
				Main << "\tmtc1 $t1, $f5" << endl;
				Main << "\tcvt.s.w $f5, $f5" << endl;
			}
			
			Main << "\tc.lt.s  $f4, $f5" << endl;
			Main << "\tbc1f OUT" << id_global_while << id_while << endl;
			Main << endl;
		}

		if(warunek == ">")
		{
			Main << "WHILE" << id_global_while << id_while << ":" << endl;

			if(arg1_type == "float")
			{
				Main << "\tl.s $f4, " << arg1 << endl;
			}
			if(arg1_type == "int")
			{
				Main << "\tlw $t0, " << arg1 << endl;
				Main << "\tmtc1 $t0, $f4" << endl;
				Main << "\tcvt.s.w $f4, $f4" << endl;
			}

			if(arg2_type == "float")
			{
				Main << "\tl.s $f5, " << arg2 << endl; 
			}
			if(arg2_type == "int")
			{
				Main << "\tlw $t1, " << arg2 << endl;
				Main << "\tmtc1 $t1, $f5" << endl;
				Main << "\tcvt.s.w $f5, $f5" << endl;
			}

			Main << "c.le.s  $f4, $f5" << endl;

			Main << "\tbc1f OUT" << id_global_while << id_while << endl;
			Main << endl;
		}

		if(warunek == "<=")
		{
			Main << "WHILE" << id_global_while << id_while << ":" << endl;

			if(arg1_type == "float")
			{
				Main << "\tl.s $f4, " << arg1 << endl;
			}
			if(arg1_type == "int")
			{
				Main << "\tlw $t0, " << arg1 << endl;
				Main << "\tmtc1 $t0, $f4" << endl;
				Main << "\tcvt.s.w $f4, $f4" << endl;
			}

			if(arg2_type == "float")
			{
				Main << "\tl.s $f5, " << arg2 << endl; 
			}
			if(arg2_type == "int")
			{
				Main << "\tlw $t1, " << arg2 << endl;
				Main << "\tmtc1 $t1, $f5" << endl;
				Main << "\tcvt.s.w $f5, $f5" << endl;
			}

			Main << "c.le.s  $f4, $f5" << endl;
			Main << "\tbc1f OUT" << id_global_while << id_while << endl;
			Main << endl;
		}

		if(warunek == ">=")
		{
			Main << "WHILE" << id_global_while << id_while << ":" << endl;

			if(arg1_type == "float")
			{
				Main << "\tl.s $f4, " << arg1 << endl;
			}
			if(arg1_type == "int")
			{
				Main << "\tlw $t0, " << arg1 << endl;
				Main << "\tmtc1 $t0, $f4" << endl;
				Main << "\tcvt.s.w $f4, $f4" << endl;
			}

			if(arg2_type == "float")
			{
				Main << "\tl.s $f5, " << arg2 << endl; 
			}
			if(arg2_type == "int")
			{
				Main << "\tlw $t1, " << arg2 << endl;
				Main << "\tmtc1 $t1, $f5" << endl;
				Main << "\tcvt.s.w $f5, $f5" << endl;
			}

			Main << "c.lt.s  $f4, $f5" << endl;
			Main << "\tbc1f OUT" << id_global_while << id_while << endl;
			Main << endl;
		}
	}
	
	return 0;

}

int zakoncz_if()
{
	Main << "NEXT" << id_global <<  id << ": " << endl;
}

int zakoncz_while()
{
	Main << "\tjal WHILE" << id_global_while << id_while << endl;
	Main << "OUT" << id_global_while << id_while << ":" << endl;

}

int generuj_else()
{
	Main << "\tjal NEXT" << id_global <<  id << endl;
	Main << endl;
	Main << "ELSE" << id_global <<  id << ": " << endl;
	Main << endl;

	
}

int deklaracja_tab_int()
{
		string variable = toString(cards.top());
		cards.pop(); 
		liczbaZmiennych++;

		string value = toString(cards.top());
		cards.pop();

		int len = variable.length();

		variable = variable.substr(1, len);

		cout << "deklaracja tablicy o nazwie " << variable << " i wielkosci: " << value << endl;

		cout << "Variable wynosi: " << variable << endl;

		symbolicTable[variable] = "@int";
		tabTablic[variable] = "@int";

		Table << "\t" << variable << ": .word 0:" << value << endl;

}


int deklaracja_tab_float()
{

		string variable = toString(cards.top());
		cards.pop(); 
		liczbaZmiennych++;

		string value = toString(cards.top());
		cards.pop(); 

		int len = variable.length();

		variable = variable.substr(1, len);

		cout << "deklaracja tablicy o nazwie " << variable << " i wielkosci: " << value << endl;


		cout << "Variable wynosi: " << variable << endl;

		symbolicTable[variable] = "@float";
		tabTablic[variable] = "@float";

		Table << "\t" << variable << ": .float 0:" << value << endl;

}

int przypisanie_tab(string arg)
{
	string wartosc = toString(cards.top());
	cards.pop(); 
	liczbaZmiennych++;

	string nazwa_tab = toString(cards.top());
	cards.pop();

	string offset = toString(cards.top());
	cards.pop();

	int len = nazwa_tab.length();

	nazwa_tab = nazwa_tab.substr(1, len);

	cout << "przypisanie do tablicy o nazwie " 	<< nazwa_tab 
												<< " do elementu: " << offset 
												<< " wartosci "<< wartosc << endl;

	outfile << "=" << " " <<  nazwa_tab << " " << wartosc <<endl;
	outfile2 << "$null" << " " << "=" << " " <<  nazwa_tab << " " << wartosc <<endl;

	if(isdigit(offset[0]))
	{
		Main << "\tli $t1," << offset << endl;
	}
	else
	{
		Main << "\tlw $t1," << offset << endl;
	}

	Main << "\tla $t0," << nazwa_tab << endl;
	Main << "\tmul $t1,$t1,4" << endl;
	Main << "\tadd $t0,$t0,$t1" << endl;



	if(symbolicTable[nazwa_tab] == "@float") //zapisanie do float
		{
			
			if( isdigit(wartosc[0]) )
			{
				symbolicTable["var"+toString(var)] = "float";
				tablicaFloat["var"+toString(var)] = wartosc;

				Main << "\tl.s $f0, " << "var"+toString(var) << endl;

				Main << "\ts.s " << "$f0,($t0)" << endl;	

				var++;
			}
			else
			{
				Main << "\tl.s $f0, " << wartosc << endl;
				Main << "\ts.s " << "$f0,($t0)" << endl;
				//Main << "\ts.s " << "$f0" << ", "  << ID << endl;
			}
				
		}

		if(symbolicTable[nazwa_tab] == "@int") // zapisanie do int
		{
			if( isdigit(wartosc[0]) ) // jesli liczba
			{
				if(wartosc.find_first_not_of("1234567890") != string::npos) //jak float to potrzebna konwersja
				{
					symbolicTable["var"+toString(var)] = "float";
					tablicaFloat["var"+toString(var)] = wartosc;

					Main << "\tl.s $f0, " << "var"+toString(var) << endl; //konwersja float na int
					Main << "\tcvt.w.s $f0,$f0" << endl;
					Main << "\tmfc1 $t1,$f0" << endl;
					Main << "\tsw $t1,($t0)" << endl;	

					var++;

				}
				else // jak int to zwykle dodanie np. c 7
				{
					Main << "\tli $t1, " << wartosc << endl;
					/*Main << "\tmtc1 $t1,$f0" << endl;
					Main << "\tcvt.w.s $f0,$f0" << endl;
					Main << "\tmfc1 $t1,$f0" << endl;*/
					Main << "\tsw $t1,($t0)" << endl;	
					//Main << "\tsw $t0, " << ID << endl;
				}
			}
			else //jesli zmienna np. c tmp2
			{
				Main << "\tl.s $f0, " << wartosc << endl; //konwersja float na int
				Main << "\tcvt.w.s $f0,$f0" << endl;
				Main << "\ts.s " << "$f0,($t0)" << endl;
			}
		}



	return 0;
}

int obliczElementTAB()
{
		
	/*cout << "teraz wypisuje ze stosu:" << endl;
	while (!cards.empty())
  	{
     	std::cout << ' ' << cards.top();
    	cards.pop();
 	}
  		std::cout << '\n';*/

	string tab = cards.top();
	cards.pop();
	string element = cards.top();
	cards.pop();

	int len = tab.length();

	tab = tab.substr(1, len);

	cout << "bede obliczal tablice: " << tab <<" o elemencie: " << element << endl;

	symbolicTable["var"+toString(var)] = "float";
	tablicaFloat["var"+toString(var)] = "0";
	
	if(isdigit(element[0])){ 
		Main << "\tli " << "$t1," << element << endl;
	}
	else{
		Main << "\tlw " << "$t1," << element << endl;
	} 

	Main << "\tla $t0," << tab << endl;
	Main << "\tmul $t1,$t1,4" << endl;
	Main << "\tadd $t0,$t0,$t1" << endl;

	if(tabTablic[tab] == "@float")
	{	
		Main << "\tl.s $f0,($t0)" << endl;
		Main << "\ts.s $f0,"<< "var"+toString(var) << endl;
	}
	else
	{
		Main << "\tlw $t2,($t0)" << endl;
		Main << "\tmtc1 $t2, $f0" << endl;
		Main << "\tcvt.s.w $f0, $f0" << endl; 
		Main << "\ts.s $f0,"<< "var"+toString(var) << endl;
	}

	
	cards.push("var"+toString(var));

	var++;
	return 0;


}

int operacja_printl()
{
	Main << "\tli $v0, 4" << endl;  // $system call code for print_str
	Main << "\tla $a0, "<< "linia" << endl;
	Main << "\tsyscall" << endl; 
}

int operacja_write()
{
	string arg = cards.top();
	cards.pop();

	if((arg[0]>=65 && arg[0]<=122) ) //jesli pierwszy znak to litera(nazwa zmiennej)
	{
			string typ;
			
		if( ifExist(arg) )
		{
			if(symbolicTable[arg] == "int")
			{
				symbolicTable["var"+toString(var)] = "float";
				tablicaFloat["var"+toString(var)] = "0";

				Main << "\tli $v0, 6" << endl;
				Main << "syscall" << endl;
				Main << "\ts.s $f0, " << "var"+toString(var) << endl; //konwersja float na int
				Main << "\tl.s $f0, " << "var"+toString(var) << endl; 
				Main << "\tcvt.w.s $f0,$f0" << endl;
				Main << "\tmfc1 $t0,$f0" << endl;
				Main << "\tsw $t0," << arg << endl;

				var++;
			}
			
			if(symbolicTable[arg] == "float")
			{
				Main << "\tli $v0, 6" << endl;
				Main << "syscall" << endl;
				Main << "\ts.s $f0, " << arg << endl;
			}


		}
		else
		{
			cout << "Zmienna o takiej nazwie nie istnieje ! Blad\n" << endl;
			return 1;
		}
					
			
						
	}
	else //liczba
	{
		cout << "Podany argument w write() jest nieprawidlowy" << endl;
		return 1;
	}

	return 0;
}


#line 1379 "def.tab.cc" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "def.tab.hh".  */
#ifndef YY_YY_DEF_TAB_HH_INCLUDED
# define YY_YY_DEF_TAB_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    NAZWA_TAB = 259,
    LN = 260,
    LF = 261,
    LFN = 262,
    ROZMIAR_TAB = 263,
    EQ = 264,
    LE = 265,
    GE = 266,
    NEQ = 267,
    WHILE = 268,
    IF = 269,
    ELSE = 270,
    FOR = 271,
    READ = 272,
    AND = 273,
    INT = 274,
    STRING = 275,
    LC = 276,
    STR = 277,
    FLOAT = 278,
    PRINT = 279,
    PRINTL = 280,
    WRITE = 281
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 1314 "def.yy" /* yacc.c:355  */
char *text;
int	ival;
double fval;

#line 1451 "def.tab.cc" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_DEF_TAB_HH_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 1466 "def.tab.cc" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  27
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   211

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  57
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  140

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   281

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      29,    30,    40,    37,     2,    38,     2,    41,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    27,
      35,    28,    36,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    33,     2,    34,    39,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,     2,    32,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1345,  1345,  1346,  1350,  1355,  1368,  1373,  1385,  1392,
    1395,  1396,  1397,  1398,  1399,  1401,  1406,  1410,  1412,  1417,
    1422,  1430,  1434,  1438,  1445,  1446,  1449,  1456,  1462,  1468,
    1474,  1480,  1486,  1492,  1502,  1508,  1514,  1520,  1526,  1532,
    1542,  1553,  1557,  1566,  1573,  1577,  1582,  1588,  1592,  1596,
    1600,  1603,  1604,  1605,  1606,  1607,  1608,  1609
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "NAZWA_TAB", "LN", "LF", "LFN",
  "ROZMIAR_TAB", "EQ", "LE", "GE", "NEQ", "WHILE", "IF", "ELSE", "FOR",
  "READ", "AND", "INT", "STRING", "LC", "STR", "FLOAT", "PRINT", "PRINTL",
  "WRITE", "';'", "'='", "'('", "')'", "'{'", "'}'", "'['", "']'", "'<'",
  "'>'", "'+'", "'-'", "'^'", "'*'", "'/'", "$accept", "program",
  "operacja", "odwolanie_tab", "nazwa", "STRI", "start", "IFSTART",
  "WHILESTART", "warunek_if", "warunek_while", "ENDIF_ONE", "ELSE_START",
  "ENDIF", "ENDWHILE", "wyr", "skladnik", "czynnik", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,    59,    61,    40,
      41,   123,   125,    91,    93,    60,    62,    43,    45,    94,
      42,    47
};
# endif

#define YYPACT_NINF -106

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-106)))

#define YYTABLE_NINF -22

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     155,   -21,    -3,  -106,  -106,    13,    29,    13,    10,    21,
      23,   147,  -106,    65,    28,    36,    20,    20,  -106,    45,
     -18,    71,    87,    61,    -1,    88,    29,  -106,  -106,    20,
      20,    20,  -106,    89,  -106,  -106,  -106,  -106,    20,    59,
       7,  -106,    53,  -106,  -106,    20,    99,  -106,  -106,    20,
     102,  -106,   111,   112,    78,   109,   119,   126,   124,    25,
     132,    44,    20,   118,  -106,    20,    20,    20,    20,    20,
    -106,   149,   150,   156,    20,   157,   158,   163,  -106,   164,
    -106,   151,    20,    20,    20,    20,    20,    20,   161,    20,
      20,    20,    20,    20,    20,   162,  -106,     7,     7,     6,
    -106,  -106,  -106,  -106,  -106,   168,  -106,  -106,  -106,  -106,
     155,     6,     6,     6,     6,     6,     6,   155,     6,     6,
       6,     6,     6,     6,  -106,   159,  -106,    81,   106,   182,
    -106,  -106,  -106,  -106,  -106,   167,   155,   120,  -106,  -106
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,    27,    26,     0,     0,     0,     0,     0,
       0,     0,     3,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     2,     0,
       0,     0,    51,     0,    53,    54,    55,    52,     0,     0,
      47,    50,     0,    18,     4,     0,     0,    19,     6,     0,
       0,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     9,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    12,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    44,    45,    46,
      48,    49,     5,     8,     7,     0,    11,    14,    10,    13,
       0,    28,    31,    33,    29,    30,    32,     0,    34,    37,
      39,    35,    36,    38,    57,    57,    24,     0,     0,    40,
      25,    16,    43,    17,    41,     0,     0,     0,    42,    15
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -106,  -106,     0,    35,    12,   165,  -105,  -106,  -106,  -106,
    -106,  -106,  -106,  -106,  -106,   -16,    37,    43
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    11,   126,    13,    20,    53,   127,    14,    15,    58,
      60,   131,   135,   139,   133,    39,    40,    41
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      12,    42,    32,    50,    34,    35,    36,    16,    54,    44,
      45,    28,   128,    57,    59,    61,    18,     2,    21,    23,
      37,    51,    63,    32,    33,    34,    35,    36,    38,    71,
      17,   137,    18,    73,    82,    83,    84,    85,    56,    24,
      19,    37,    22,    65,    66,    67,    95,    68,    69,    38,
      25,    99,    26,    89,    90,    91,    92,    30,   105,    52,
      86,    87,    65,    66,    67,    31,   111,   112,   113,   114,
     115,   116,    43,   118,   119,   120,   121,   122,   123,    93,
      94,    65,    66,    67,     1,     2,    64,    70,    48,    49,
      65,    66,    67,    29,     3,     4,    65,    66,    67,    46,
       5,     6,    97,    98,     7,     8,     9,    10,    77,     1,
       2,   100,   101,   129,    47,    65,    66,    67,    55,     3,
       4,    51,    62,     1,     2,     5,     6,   130,   130,     7,
       8,     9,    10,     3,     4,    74,    78,   130,   132,     5,
       6,    75,    76,     7,     8,     9,    10,    27,    96,    79,
       1,     2,   138,    80,    81,    65,    66,    67,     1,     2,
       3,     4,    88,    65,    66,    67,     5,     6,     3,     4,
       7,     8,     9,    10,     5,     6,   102,   103,     7,     8,
       9,    10,   110,   104,   106,   107,    65,    66,    67,   -21,
     108,   109,   117,    65,    66,    67,   124,   134,   136,    65,
      66,    67,   125,     0,     0,    65,    66,    67,     0,     0,
       0,    72
};

static const yytype_int16 yycheck[] =
{
       0,    17,     3,     4,     5,     6,     7,    28,    24,    27,
      28,    11,   117,    29,    30,    31,     3,     4,     6,     7,
      21,    22,    38,     3,     4,     5,     6,     7,    29,    45,
      33,   136,     3,    49,     9,    10,    11,    12,    26,    29,
       5,    21,     7,    37,    38,    39,    62,    40,    41,    29,
      29,    67,    29,     9,    10,    11,    12,    29,    74,    24,
      35,    36,    37,    38,    39,    29,    82,    83,    84,    85,
      86,    87,    27,    89,    90,    91,    92,    93,    94,    35,
      36,    37,    38,    39,     3,     4,    27,    34,    27,    28,
      37,    38,    39,    28,    13,    14,    37,    38,    39,    28,
      19,    20,    65,    66,    23,    24,    25,    26,    30,     3,
       4,    68,    69,    32,    27,    37,    38,    39,    30,    13,
      14,    22,    33,     3,     4,    19,    20,   127,   128,    23,
      24,    25,    26,    13,    14,    33,    27,   137,    32,    19,
      20,    30,    30,    23,    24,    25,    26,     0,    30,    30,
       3,     4,    32,    27,    30,    37,    38,    39,     3,     4,
      13,    14,    30,    37,    38,    39,    19,    20,    13,    14,
      23,    24,    25,    26,    19,    20,    27,    27,    23,    24,
      25,    26,    31,    27,    27,    27,    37,    38,    39,    30,
      27,    27,    31,    37,    38,    39,    34,    15,    31,    37,
      38,    39,    34,    -1,    -1,    37,    38,    39,    -1,    -1,
      -1,    46
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,    13,    14,    19,    20,    23,    24,    25,
      26,    43,    44,    45,    49,    50,    28,    33,     3,    45,
      46,    46,    45,    46,    29,    29,    29,     0,    44,    28,
      29,    29,     3,     4,     5,     6,     7,    21,    29,    57,
      58,    59,    57,    27,    27,    28,    28,    27,    27,    28,
       4,    22,    45,    47,    57,    30,    46,    57,    51,    57,
      52,    57,    33,    57,    27,    37,    38,    39,    40,    41,
      34,    57,    47,    57,    33,    30,    30,    30,    27,    30,
      27,    30,     9,    10,    11,    12,    35,    36,    30,     9,
      10,    11,    12,    35,    36,    57,    30,    58,    58,    57,
      59,    59,    27,    27,    27,    57,    27,    27,    27,    27,
      31,    57,    57,    57,    57,    57,    57,    31,    57,    57,
      57,    57,    57,    57,    34,    34,    44,    48,    48,    32,
      44,    53,    32,    56,    15,    54,    31,    48,    32,    55
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    43,    43,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    45,    46,    47,    48,    48,    49,    50,    51,    51,
      51,    51,    51,    51,    52,    52,    52,    52,    52,    52,
      53,    54,    55,    56,    57,    57,    57,    57,    58,    58,
      58,    59,    59,    59,    59,    59,    59,    59
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     3,     5,     3,     5,     5,     4,
       5,     5,     4,     5,     5,    11,     7,     7,     3,     3,
       4,     4,     1,     1,     1,     2,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     3,     3,     3,     1,     3,     3,
       1,     1,     1,     1,     1,     1,     3,     4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 1345 "def.yy" /* yacc.c:1646  */
    {printf("program skladajacy sie z wiecej niz jednej linii\n");}
#line 2642 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 3:
#line 1346 "def.yy" /* yacc.c:1646  */
    {printf("program jedno liniowy, w danym momencie\n");}
#line 2648 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 4:
#line 1350 "def.yy" /* yacc.c:1646  */
    { 	
							printf("Deklaracja zmiennej typu int \n"); 
							int result = deklaracja_int("0");
							if(result) exit(1); 
						}
#line 2658 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 5:
#line 1355 "def.yy" /* yacc.c:1646  */
    { 
							printf("Deklaracja zmiennej typu int i przypisanie wartosci \n"); 
							cout << "dolar wynosi : " << variableName << endl;
							int result =deklaracja_int(variableName);
							if(result) exit(1);
							printf("wyrazenie z = \n");
							result = wykonaj_dzialanie("=", variableName);
							if(result) exit(1);

							cards.pop();
							liczbaZmiennych++;
						}
#line 2675 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 6:
#line 1368 "def.yy" /* yacc.c:1646  */
    { 	printf("Deklaracja zmiennej typu float \n"); 
							int result = deklaracja_float("0");
							if(result) exit(1); 
						}
#line 2684 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 7:
#line 1373 "def.yy" /* yacc.c:1646  */
    { 
							printf("Deklaracja zmiennej typu float i przypisanie wartosci \n"); 
							cout << "dolar wynosi : " << variableName << endl;
							int result =deklaracja_float(variableName);
							if(result) exit(1);
							printf("wyrazenie z = \n");
							result = wykonaj_dzialanie("=", variableName);
							if(result) exit(1);
							cards.pop();
							liczbaZmiennych++;
						}
#line 2700 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 8:
#line 1385 "def.yy" /* yacc.c:1646  */
    {
									cout << "deklaracja zmiennej typu string" << endl;
									cout << "dolar wynosi : " << variableName << endl;
									int result = deklaracja_string();
								}
#line 2710 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 9:
#line 1392 "def.yy" /* yacc.c:1646  */
    {printf("wyrazenie z = \n");wykonaj_dzialanie("=", (yyvsp[-3].text)); 
					liczbaZmiennych++;
				}
#line 2718 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 10:
#line 1395 "def.yy" /* yacc.c:1646  */
    {printf("Operacja wyswietlania \n"); operacja_print("0"); }
#line 2724 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 11:
#line 1396 "def.yy" /* yacc.c:1646  */
    {printf("Operacja wyswietlania tablicy \n"); operacja_print("TAB"); }
#line 2730 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 12:
#line 1397 "def.yy" /* yacc.c:1646  */
    {printf("Operacja wyswietlania \n"); operacja_printl(); }
#line 2736 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 13:
#line 1398 "def.yy" /* yacc.c:1646  */
    {printf("Operacja wprowadzania \n"); operacja_write(); }
#line 2742 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 14:
#line 1399 "def.yy" /* yacc.c:1646  */
    {printf("Operacja wyswietlania string jako parametr\n"); operacja_print("PARAM"); }
#line 2748 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 15:
#line 1401 "def.yy" /* yacc.c:1646  */
    {
																				printf("Operacja if z else\n");
																			}
#line 2756 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 16:
#line 1406 "def.yy" /* yacc.c:1646  */
    { 
													printf("Operacja if bez else\n");
													}
#line 2764 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 18:
#line 1412 "def.yy" /* yacc.c:1646  */
    {
											cout << "deklaracja tablicy typu int" << endl;
											deklaracja_tab_int();
									}
#line 2773 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 19:
#line 1417 "def.yy" /* yacc.c:1646  */
    {
											cout << "deklaracja tablicy typu float" << endl;
											deklaracja_tab_float();
										}
#line 2782 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 20:
#line 1422 "def.yy" /* yacc.c:1646  */
    {
										cout << "przypisanie do tablicy" << endl;
										przypisanie_tab(variableName);
								}
#line 2791 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 21:
#line 1430 "def.yy" /* yacc.c:1646  */
    {printf("nazwa tablicy\n"); cards.push((yyvsp[-3].text)); variableName = cards.top();}
#line 2797 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 22:
#line 1434 "def.yy" /* yacc.c:1646  */
    {printf("czynnik znakowy\n"); cards.push((yyvsp[0].text)); variableName = cards.top();}
#line 2803 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 23:
#line 1438 "def.yy" /* yacc.c:1646  */
    {
					cout << "Wyrazenie typu string" << endl;
					cards.push((yyvsp[0].text));
				}
#line 2812 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 26:
#line 1449 "def.yy" /* yacc.c:1646  */
    {
							++id;
							++id_global;
							cout << "poczatek instrukcji if o id=" << id << endl;
						}
#line 2822 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 27:
#line 1456 "def.yy" /* yacc.c:1646  */
    {
							++id_while;
							cout << "poczatek petli while o id=" << id_while << endl;
						}
#line 2831 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 28:
#line 1462 "def.yy" /* yacc.c:1646  */
    { 
							printf("Warunek z ==\n");
							int result = sprawdz_warunek("==", "if");
							if(result) exit(1);
						}
#line 2841 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 29:
#line 1468 "def.yy" /* yacc.c:1646  */
    { 
							printf("Warunek z !=\n");
							int result = sprawdz_warunek("!=", "if");
							if(result) exit(1);
						}
#line 2851 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 30:
#line 1474 "def.yy" /* yacc.c:1646  */
    { 
							printf("Warunek z <\n");
							int result = sprawdz_warunek("<", "if");
							if(result) exit(1);
						}
#line 2861 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 31:
#line 1480 "def.yy" /* yacc.c:1646  */
    { 
							printf("Warunek z <=\n");
							int result = sprawdz_warunek("<=", "if");
							if(result) exit(1);
						}
#line 2871 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 32:
#line 1486 "def.yy" /* yacc.c:1646  */
    { 
							printf("Warunek z >\n");
							int result = sprawdz_warunek(">", "if");
							if(result) exit(1);
						}
#line 2881 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 33:
#line 1492 "def.yy" /* yacc.c:1646  */
    { 
							printf("Warunek z >=\n");
							int result = sprawdz_warunek(">=", "if");
							if(result) exit(1);
						}
#line 2891 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 34:
#line 1502 "def.yy" /* yacc.c:1646  */
    { 
							printf("Warunek z == while\n");
							int result = sprawdz_warunek("==", "while");
							if(result) exit(1);
						}
#line 2901 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 35:
#line 1508 "def.yy" /* yacc.c:1646  */
    { 
							printf("Warunek z !=\n");
							int result = sprawdz_warunek("!=", "while");
							if(result) exit(1);
						}
#line 2911 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 36:
#line 1514 "def.yy" /* yacc.c:1646  */
    { 
							printf("Warunek z < while\n");
							int result = sprawdz_warunek("<", "while");
							if(result) exit(1);
						}
#line 2921 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 37:
#line 1520 "def.yy" /* yacc.c:1646  */
    { 
							printf("Warunek z <=\n");
							int result = sprawdz_warunek("<=", "while");
							if(result) exit(1);
						}
#line 2931 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 38:
#line 1526 "def.yy" /* yacc.c:1646  */
    { 
							printf("Warunek z >\n");
							int result = sprawdz_warunek(">", "while");
							if(result) exit(1);
						}
#line 2941 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 39:
#line 1532 "def.yy" /* yacc.c:1646  */
    { 
							printf("Warunek z >=\n");
							int result = sprawdz_warunek(">=", "while");
							if(result) exit(1);
						}
#line 2951 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 40:
#line 1542 "def.yy" /* yacc.c:1646  */
    {
							printf("koniec wyrazenia warunkowego if -bez else\n");
							generuj_else();
							zakoncz_if();
							--id;
							--id_global;
							if(id ==  0)		id_global++;
						}
#line 2964 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 41:
#line 1553 "def.yy" /* yacc.c:1646  */
    { generuj_else(); printf("generuje else\n"); }
#line 2970 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 42:
#line 1557 "def.yy" /* yacc.c:1646  */
    {
							printf("koniec wyrazenia warunkowego if z else\n");
							zakoncz_if();
							--id;
							--id_global;
							if(id ==  0)		id_global++;
						}
#line 2982 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 43:
#line 1566 "def.yy" /* yacc.c:1646  */
    {
							printf("koniec petli while\n");
							zakoncz_while();
							--id_while;
						}
#line 2992 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 44:
#line 1573 "def.yy" /* yacc.c:1646  */
    {	printf("wyrazenie z + \n"); 
							int result = wykonaj_dzialanie("+", "0");
							if(result) exit(1);
						}
#line 3001 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 45:
#line 1577 "def.yy" /* yacc.c:1646  */
    {	printf("wyrazenie z - \n"); 
							int result = wykonaj_dzialanie("-", "0");
							if(result) exit(1);
						}
#line 3010 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 46:
#line 1582 "def.yy" /* yacc.c:1646  */
    {
							cout << "Podnoszenie do potegi " << endl;
							int result = wykonaj_dzialanie("^", "0");
							if(result) exit(1);
						}
#line 3020 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 47:
#line 1588 "def.yy" /* yacc.c:1646  */
    {printf("wyrazenie pojedyncze \n");}
#line 3026 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 48:
#line 1592 "def.yy" /* yacc.c:1646  */
    {	printf("skladnik z * \n"); 
								int result = wykonaj_dzialanie("*", "0");
								if(result) exit(1);
							}
#line 3035 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 49:
#line 1596 "def.yy" /* yacc.c:1646  */
    {	printf("skladnik z / \n"); 
								int result = wykonaj_dzialanie("/", "0");
								if(result) exit(1);
							}
#line 3044 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 50:
#line 1600 "def.yy" /* yacc.c:1646  */
    {printf("skladnik pojedynczy \n");}
#line 3050 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 51:
#line 1603 "def.yy" /* yacc.c:1646  */
    {printf("czynnik znakowy\n"); cards.push((yyvsp[0].text));}
#line 3056 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 52:
#line 1604 "def.yy" /* yacc.c:1646  */
    {printf("czynnik liczbowy calkowity\n"); cards.push(toString((yyvsp[0].text)));}
#line 3062 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 53:
#line 1605 "def.yy" /* yacc.c:1646  */
    {printf("czynnik liczbowy naturalny\n"); cards.push(toString((yyvsp[0].ival)));}
#line 3068 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 54:
#line 1606 "def.yy" /* yacc.c:1646  */
    {printf("czynnik liczbowy\n"); cards.push(toString((yyvsp[0].fval)));}
#line 3074 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 55:
#line 1607 "def.yy" /* yacc.c:1646  */
    {printf("czynnik liczbowy zmiennoprzecinkowy ujemny\n"); cards.push(toString((yyvsp[0].fval)));}
#line 3080 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 56:
#line 1608 "def.yy" /* yacc.c:1646  */
    {printf("wyrazenie w nawiasach\n");}
#line 3086 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 57:
#line 1609 "def.yy" /* yacc.c:1646  */
    {printf("odwolanie do tablicy\n"); cards.push((yyvsp[-3].text)); obliczElementTAB();}
#line 3092 "def.tab.cc" /* yacc.c:1646  */
    break;


#line 3096 "def.tab.cc" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1611 "def.yy" /* yacc.c:1906  */

int main(int argc, char *argv[])
{
	yyparse();

	progASM << ".data" << endl;
	for(map<string,string>::const_iterator it = symbolicTable.begin(); it!= symbolicTable.end();it++)
	{
		if( it->second.compare("int") == 0 )
			{
				progASM << "\t" << it->first;
				progASM << ": .word 0" << endl;
			}

		if( it->second.compare("float") == 0 )
			{
				progASM << "\t" << it->first;
				progASM << ": .float " << tablicaFloat[it->first] << endl;
			}

		if( it->second.compare("string") == 0 )
			{
				progASM << "\t" << it->first;
				progASM << ": .asciiz " << tablicaString[it->first] << endl;
			}

		if( it->second.compare("string\n") == 0 )
			{
				progASM << "\t" << it->first;
				progASM << ": .asciiz " << tablicaString[it->first] << endl;
			}

	}

	progASM << Table.str();

	progASM << "\tlinia:" << " .asciiz  \"\\n\"" << endl;
	
	cout << "teraz wypisuje ze stosu:" << endl;
	while (!cards.empty())
  	{
     	std::cout << ' ' << cards.top();
    	cards.pop();
 	}
  		std::cout << '\n';

  	progASM << ".text" << endl;
	progASM << ".globl main" << endl;
	progASM << "main:" << endl;
	string s = Main.str();
	progASM << s;

	return 0;

}
