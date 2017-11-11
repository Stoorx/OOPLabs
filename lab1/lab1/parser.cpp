/********************************
 *			Lab 1				*
 *			IniParser			*
 ********************************/


#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <time.h>
#include "IniParser.h"
#include "IParserExcept.h

using namespace std;





int main()
{
	cout << "IniParser" << endl;

	while (true) {
		string filename;
		cout << "File name or X>";
		cin >> filename;
		cout << filename << endl;
		if (filename == "X" || filename == "x")
			break;
		
		try {
			//time_t start = clock();
			//for (int i = 0; i < 10000; i++) {
				IniParser parser(filename);
				//cout << i << endl;
			//}
			//time_t finish = clock();
			//cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;
			
			while (true)
			{
				string command;
				cout << "Section or X>";
				cin >> command;
				if (command == "X" || command == "x")
					break;
				else {
					string par;
					cout << "Parameter>";
					cin >> par;
					cout << parser.get(command, par) << endl;
				}
			}
		}
		catch (EParserIO e) {
			cout << e.errorMsg << endl;
		}
		catch (EParserFormat e){
			cout << e.errorMsg << endl;
		}
		catch (EParserParameterNotFound e){
			cout << e.errorMsg << endl;
		}
		
	}

	return 0;
}
