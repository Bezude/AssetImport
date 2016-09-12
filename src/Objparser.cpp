#include "Objparser.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <array>


using namespace std;

Objparser::Objparser(char* filename) {
	string line;
	vertCount = texVertCount = elemCount = normCount = 0;
	vector<string> tokens;
	vector< array<float, 3> > vertVec;
	vector< array<float, 2> > texVertVec;
	vector< array<float, 3> > normVec;
	vector< array<int, 9> > elemVec;

	ifstream inFile(filename);
	// todo allocate buffers
	if(inFile.is_open()) {
		while(getline(inFile, line)) {
			split(line, ' ', tokens);
			if(tokens[0] == "v") {
				if(tokens.size() > 4) {
					ngonError();
					break;
				}
				vertCount++;
				array<float, 3> vert;
				vert[0] = stof(tokens[1]);
				vert[1] = stof(tokens[2]);
				vert[2] = stof(tokens[3]);
				vertVec.push_back(vert);
			}
			else if(tokens[0] == "vt") {
				if(tokens.size() > 4) {
					ngonError();
					break;
				}
				texVertCount++;
				// stuff
			}
			else if(tokens[0] == "vn") {
				if(tokens.size() > 4) {
					ngonError();
					break;
				}
				normCount++;
				// stuff
			}
			else if(tokens[0] == "f") {
				if(tokens.size() > 4) {
					ngonError();
					break;
				}
				elemCount++;
				// stuff
			}
			tokens.clear();
		}
		cout << vertCount << ' ' << texVertCount << ' ' << normCount << ' ' << elemCount << '\n';
		cout << vertVec[0][0] << ',' << vertVec[0][1] << ',' << vertVec[0][2] << '\n';
	}
}

Objparser::~Objparser() {
	// noop
}



void Objparser::split(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

inline void Objparser::ngonError() {
	cout << "A feature with more than 3 elements was found. "
		<< "An obj being parsed may contain non-triangle faces. "
		<< "This software only supports triangles" << '\n';
}