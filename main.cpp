#include <iostream>
#include "HuffmanCoding.h"

#ifdef RELEASE
#define MODE1

using std::cout;
using std::string;
using std::endl;

int main(int argc, char** argv){
#ifdef MODE1
	if (argc >= 2) {
		string mode(argv[1]);
		if (mode == "-c" || mode == "-C") {
			for (int i = 2; i < argc; i++) {
				string path(argv[i]);
				HuffmanCoding huffmanCoding;
				huffmanCoding.open(path, c);
				huffmanCoding.compress();
				huffmanCoding.showPerformance();
				huffmanCoding.close();
			}
		}
		else if (mode == "-d" || mode == "-D") {
			for (int i = 2; i < argc; i++) {
				string path(argv[i]);
				HuffmanCoding huffmanCoding;
				huffmanCoding.open(path, d);
				huffmanCoding.decompress();
				huffmanCoding.showPerformance();
				huffmanCoding.close();
			}
		}
		else {
			cout << "Command format:" << endl;
			cout << "Compress: HuffmanCoding -c filename" << endl;
			cout << "Decompress: HuffmanCoding -d filename" << endl;
		}
	}
	else {
		cout << "Command format:" << endl;
        cout << "Compress: HuffmanCoding -c filename" << endl;
        cout << "Decompress: HuffmanCoding -d filename" << endl;
	}
#endif
#ifdef MODE2
	string path;
	int mode;
	cout << "pleade input the filename your want to compress or decompress: " << endl;
	cin >> path;
	cout << "input 0 for compress and 1 for decompress: " << endl;
	cin >> mode;
	HuffmanCoding obj;
	if (mode == 0) {
		obj.open(path, c);
		obj.compress();
		obj.showPerformance();
		obj.close();
	}
	else if(mode == 1){
		obj.open(path, d);
		obj.decompress();
		obj.showPerformance();
		obj.close();
	}
	else {
		cout << "Invalid mode." << endl;
	}
#endif
	return 0;
}

#endif
