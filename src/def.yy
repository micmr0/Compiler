%{
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

%}
%union 
{char *text;
int	ival;
double fval;};
%type <text> wyr
%token <text> ID
%token <text> NAZWA_TAB
%token <ival> LN
%token <fval> LF
%token <fval> LFN
%token <ival> ROZMIAR_TAB
%token EQ
%token LE
%token GE
%token NEQ
%token WHILE
%token IF
%token ELSE
%token FOR
%token READ
%token AND
%token <text> INT
%token <text> STRING
%token <text> LC
%token <text> STR
%token <text> FLOAT
%token <text> PRINT
%token <text> PRINTL
%token <text> WRITE
%%

program
	:program  operacja		{printf("program skladajacy sie z wiecej niz jednej linii\n");}
	|operacja				{printf("program jedno liniowy, w danym momencie\n");}
	;

operacja
	:INT nazwa ';'		{ 	
							printf("Deklaracja zmiennej typu int \n"); 
							int result = deklaracja_int("0");
							if(result) exit(1); 
						}
	|INT nazwa '=' wyr ';'{ 
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

	|FLOAT nazwa ';' 	{ 	printf("Deklaracja zmiennej typu float \n"); 
							int result = deklaracja_float("0");
							if(result) exit(1); 
						}

	|FLOAT nazwa '=' wyr ';'{ 
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

	|STRING nazwa '=' STRI ';'	{
									cout << "deklaracja zmiennej typu string" << endl;
									cout << "dolar wynosi : " << variableName << endl;
									int result = deklaracja_string();
								}

				
	|ID '=' wyr ';' {printf("wyrazenie z = \n");wykonaj_dzialanie("=", $1); 
					liczbaZmiennych++;
				}
	|PRINT '(' wyr ')' ';' 	{printf("Operacja wyswietlania \n"); operacja_print("0"); }
	|PRINT '(' odwolanie_tab ')' ';' 	{printf("Operacja wyswietlania tablicy \n"); operacja_print("TAB"); }
	|PRINTL '('')' ';' {printf("Operacja wyswietlania \n"); operacja_printl(); }
	|WRITE '(' nazwa ')' ';' {printf("Operacja wprowadzania \n"); operacja_write(); }
	|PRINT '(' STRI ')' ';' 	{printf("Operacja wyswietlania string jako parametr\n"); operacja_print("PARAM"); }
	
	|IFSTART '(' warunek_if ')'  '{' start '}' ELSE_START '{' start ENDIF		{
																				printf("Operacja if z else\n");
																			}
	;

	|IFSTART '(' warunek_if ')'  '{' start ENDIF_ONE  	{ 
													printf("Operacja if bez else\n");
													}

	|WHILESTART '(' warunek_while ')' '{' start ENDWHILE

	|INT odwolanie_tab  ';'	{
											cout << "deklaracja tablicy typu int" << endl;
											deklaracja_tab_int();
									}

	|FLOAT odwolanie_tab  ';'	{
											cout << "deklaracja tablicy typu float" << endl;
											deklaracja_tab_float();
										}
										
	|odwolanie_tab '=' wyr ';'	{
										cout << "przypisanie do tablicy" << endl;
										przypisanie_tab(variableName);
								}



odwolanie_tab
	:NAZWA_TAB '[' wyr ']'	{printf("nazwa tablicy\n"); cards.push($1); variableName = cards.top();} 
	

nazwa
	:ID			{printf("czynnik znakowy\n"); cards.push($1); variableName = cards.top();} 
	;

STRI
	: STR 		{
					cout << "Wyrazenie typu string" << endl;
					cards.push($1);
				}
;

start
	:operacja
	|start operacja

IFSTART
	:IF 				{
							++id;
							++id_global;
							cout << "poczatek instrukcji if o id=" << id << endl;
						}

WHILESTART
	:WHILE 				{
							++id_while;
							cout << "poczatek petli while o id=" << id_while << endl;
						}

warunek_if
	:wyr EQ wyr 		{ 
							printf("Warunek z ==\n");
							int result = sprawdz_warunek("==", "if");
							if(result) exit(1);
						}

	|wyr NEQ wyr 		{ 
							printf("Warunek z !=\n");
							int result = sprawdz_warunek("!=", "if");
							if(result) exit(1);
						}

	|wyr '<' wyr 		{ 
							printf("Warunek z <\n");
							int result = sprawdz_warunek("<", "if");
							if(result) exit(1);
						}

	|wyr LE wyr 		{ 
							printf("Warunek z <=\n");
							int result = sprawdz_warunek("<=", "if");
							if(result) exit(1);
						}

	|wyr '>' wyr 		{ 
							printf("Warunek z >\n");
							int result = sprawdz_warunek(">", "if");
							if(result) exit(1);
						}

	|wyr GE wyr 		{ 
							printf("Warunek z >=\n");
							int result = sprawdz_warunek(">=", "if");
							if(result) exit(1);
						}

	;


warunek_while
	:wyr EQ wyr 		{ 
							printf("Warunek z == while\n");
							int result = sprawdz_warunek("==", "while");
							if(result) exit(1);
						}

	|wyr NEQ wyr 		{ 
							printf("Warunek z !=\n");
							int result = sprawdz_warunek("!=", "while");
							if(result) exit(1);
						}

	|wyr '<' wyr 		{ 
							printf("Warunek z < while\n");
							int result = sprawdz_warunek("<", "while");
							if(result) exit(1);
						}

	|wyr LE wyr 		{ 
							printf("Warunek z <=\n");
							int result = sprawdz_warunek("<=", "while");
							if(result) exit(1);
						}

	|wyr '>' wyr 		{ 
							printf("Warunek z >\n");
							int result = sprawdz_warunek(">", "while");
							if(result) exit(1);
						}

	|wyr GE wyr 		{ 
							printf("Warunek z >=\n");
							int result = sprawdz_warunek(">=", "while");
							if(result) exit(1);
						}

	;


ENDIF_ONE
	: '}' 				{
							printf("koniec wyrazenia warunkowego if -bez else\n");
							generuj_else();
							zakoncz_if();
							--id;
							--id_global;
							if(id ==  0)		id_global++;
						}
	;

ELSE_START
	:ELSE					{ generuj_else(); printf("generuje else\n"); }
	;

ENDIF
	: '}'				{
							printf("koniec wyrazenia warunkowego if z else\n");
							zakoncz_if();
							--id;
							--id_global;
							if(id ==  0)		id_global++;
						}

ENDWHILE
	: '}'				{
							printf("koniec petli while\n");
							zakoncz_while();
							--id_while;
						}				

wyr
	:wyr '+' skladnik	{	printf("wyrazenie z + \n"); 
							int result = wykonaj_dzialanie("+", "0");
							if(result) exit(1);
						}
	|wyr '-' skladnik	{	printf("wyrazenie z - \n"); 
							int result = wykonaj_dzialanie("-", "0");
							if(result) exit(1);
						}

	|wyr '^' wyr		{
							cout << "Podnoszenie do potegi " << endl;
							int result = wykonaj_dzialanie("^", "0");
							if(result) exit(1);
						}

	|skladnik			{printf("wyrazenie pojedyncze \n");}
	;

skladnik
	:skladnik '*' czynnik	{	printf("skladnik z * \n"); 
								int result = wykonaj_dzialanie("*", "0");
								if(result) exit(1);
							}
	|skladnik '/' czynnik	{	printf("skladnik z / \n"); 
								int result = wykonaj_dzialanie("/", "0");
								if(result) exit(1);
							}
	|czynnik		{printf("skladnik pojedynczy \n");}
	;
czynnik
	:ID			{printf("czynnik znakowy\n"); cards.push($1);}
	|LC			{printf("czynnik liczbowy calkowity\n"); cards.push(toString($1));} 
	|LN			{printf("czynnik liczbowy naturalny\n"); cards.push(toString($1));}
	|LF			{printf("czynnik liczbowy\n"); cards.push(toString($1));}
	|LFN			{printf("czynnik liczbowy zmiennoprzecinkowy ujemny\n"); cards.push(toString($1));}
	|'(' wyr ')'		{printf("wyrazenie w nawiasach\n");}
	|NAZWA_TAB '[' wyr ']'	{printf("odwolanie do tablicy\n"); cards.push($1); obliczElementTAB();}
	;
%%
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
