#include <iostream>
#include <fstream>
#include <string>
#include "TuringMachine.h"

using namespace std;

int main(int argc, char* argv[]) {
    bool verbose = false;
	string filename = "";
	string input = "";
	if (argc == 1) {
		cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
		return 0;
	}
	for(int i = 1; i < argc; i++) {
		string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
			cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
			return 0;
		}
		else if (arg == "--verbose" || arg == "-v") {
			argc--;
			verbose = true;
		}
		else {
			if (filename == "") {
				filename = "programs/";
				filename += arg;
			}
			else {
				input = arg;
			}
		}
	}
	if (argc == 2) {
		cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
		return 0;
	}

	TuringMachine tm = TuringMachine(filename, verbose);
    tm.simulate(input);
	return 0;
}
