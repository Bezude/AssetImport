#include "ObjParser.hpp"

#include <iostream>
#include <sstream>
#include <fstream>



using namespace std;

ObjParser::ObjParser(char* filename) {
	string line;
	vertCount = texVertCount = elemCount = normCount = 0;
	vector<string> tokens;
	

	ifstream inFile(filename);
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
				array<float, 2> texVert;
				texVert[0] = stof(tokens[1]);
				texVert[1] = stof(tokens[2]);
				texVertVec.push_back(texVert);
			}
			else if(tokens[0] == "vn") {
				if(tokens.size() > 4) {
					ngonError();
					break;
				}
				normCount++;
				array<float, 3> norm;
				norm[0] = stof(tokens[1]);
				norm[1] = stof(tokens[2]);
				norm[2] = stof(tokens[3]);
				normVec.push_back(norm);
			}
			else if(tokens[0] == "f") {
				if(tokens.size() > 4) {
					ngonError();
					break;
				}
				elemCount++;
				array<unsigned short, 9> elem;
				// obj format provides the vertex, texture vertex, and normal indices interleaved
				// I have separated them here
				for(int i = 1; i < tokens.size(); i++){
					vector<string> triplet;
					split(tokens[i], '/', triplet);
					elem[0+(i-1)] = stoi(triplet[0]);
					// if no texture coordinate was given the second element is empty
					if(!triplet[1].empty()) {
						elem[3+(i-1)] = stos(triplet[1]);
					}
					// if no vertex normal was given the triplet will just be a "doublet"
					if(triplet.size() > 2) {
						elem[6+(i-1)] = stos(triplet[2]);
					}
				}
				elemVec.push_back(elem);
			}
			tokens.clear();
		}
		cout << vertCount << ' ' << texVertCount << ' ' << normCount << ' ' << elemCount << '\n';
		cout << vertVec[0][0] << ',' << vertVec[0][1] << ',' << vertVec[0][2] << '\n';
	}
}

ObjParser::~ObjParser() {
	// noop
}

void ObjParser::fillVertArray(float* buffer) {
	for(int i = 0; i < vertCount; i++) {
		int index = i*3;
		buffer[index] = vertVec[i][0];
		buffer[index+1] = vertVec[i][1];
		buffer[index+2] = vertVec[i][2];
	}
}

void ObjParser::fillUVArray(float* buffer) {
	for(int i = 0; i < texVertCount; i++) {
		int index = i*2;
		buffer[index] = texVertVec[i][0];
		buffer[index+1] = texVertVec[i][1];
	}
}

void ObjParser::fillNormArray(float* buffer) {
	for(int i = 0; i < normCount; i++) {
		int index = i*3;
		buffer[index] = normVec[i][0];
		buffer[index+1] = normVec[i][1];
		buffer[index+2] = normVec[i][2];
	}
}

void ObjParser::fillElementArray(unsigned short* buffer, bool UVs, bool normals) {
	int width = 3;
	if(UVs) {
		width += 3;
	}
	if(normals) {
		width += 3;
	}
	for(int i = 0; i < elemCount; i++) {
		int index = i*width;
		buffer[index] = elemVec[i][0];
		buffer[index+1] = elemVec[i][1];
		buffer[index+2] = elemVec[i][2];
		if(UVs) {
			buffer[index+3] = elemVec[i][3];
			buffer[index+4] = elemVec[i][4];
			buffer[index+5] = elemVec[i][5];
		}
		if(normals) {
			int offset = UVs ? 6 : 3;
			buffer[index+offset] = elemVec[i][6];
			buffer[index+offset+1] = elemVec[i][7];
			buffer[index+offset+2] = elemVec[i][8];
		}
	}
}

int ObjParser::getVertCount() {
	return vertCount;
}

int ObjParser::getTexVertCount() {
	return texVertCount;
}

int ObjParser::getNormCount() {
	return normCount;
}

int ObjParser::getElemCount() {
	return elemCount;
}

unsigned short ObjParser::stos(string s) {
	return stoi(s);
}

void ObjParser::split(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

inline void ObjParser::ngonError() {
	cout << "A feature with more than 3 elements was found. "
		<< "An obj being parsed may contain non-triangle faces. "
		<< "This software only supports triangles" << '\n';
}