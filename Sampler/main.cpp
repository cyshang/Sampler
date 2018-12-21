#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
	string fTrainSet;
	string fData;
	string fRst;

	if (argc == 3) {

		

		fData = argv[1];
		fRst = argv[2];
	}
	else if (argc == 4) {

		fTrainSet = argv[1];
		fData = argv[2];
		fRst = argv[3];
	}
	else {

	}

    return 0;
}