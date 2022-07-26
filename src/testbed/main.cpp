//std includes
#include <iostream>
#include <conio.h>
#include <map>
#include <algorithm>
#include <vector>
using namespace std;

//arkhe includes
#include "apps.h"

void (*pt2Function)(float,char,char) = 0;

void DoIt(float a,char b,char c) { cout << "balh"; }

//pt2Function = DoIt;

#define RUN_APP(app) Application::run(new app,argc,argv);

int main(int argc,char **argv)
{
	try
	{
		//RUN_APP(particleApp)
		//RUN_APP(boxApp)
		//UN_APP(integratorApp)
		//RUN_APP(femApp)
		//RUN_APP(rbApp)
		//RUN_APP(marchingCubesApp);
		RUN_APP(marchingCubesGridApp);
		//while(!kbhit()) ;

		std::map<int,std::vector<char>> mymap;
		std::vector<char> charvec;
		charvec.push_back('a');
		charvec.push_back('b');
		charvec.push_back('c');
		charvec.push_back('d');
		charvec.push_back('e');
		mymap[0] = charvec;
		charvec.clear();
		charvec.push_back('f');
		charvec.push_back('g');
		charvec.push_back('h');
		charvec.push_back('i');
		charvec.push_back('j');
		mymap[1] = charvec;
		charvec.clear();
		charvec.push_back('k');
		charvec.push_back('l');
		charvec.push_back('m');
		charvec.push_back('n');
		charvec.push_back('o');
		mymap[2] = charvec;

		std::map<int,std::vector<char>>::iterator i = mymap.begin();
		while(i != mymap.end())
		{
			std::vector<char>::iterator j = i->second.begin();
			while(j != i->second.end())
			{
				cout << *j << ' ';
				j++;
			}
			cout << endl;
			i++;
		}

		while(!kbhit());
	}
	catch(std::exception &e)
	{
		cerr << e.what() << endl;
	}
	return 0;
}
