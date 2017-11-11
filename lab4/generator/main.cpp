#include <random>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

using namespace std;

int main() {

	string filename;
	cin >> filename;
	ofstream out(filename);
	int count;
	cin >> count;
	srand(time(NULL));
	for(int i = 0; i < count; i++) {
		out << abs(rand()) << endl;
		cout << 100 * ((double)i / count) << "%\n";
	}
	return 0;
}