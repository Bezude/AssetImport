#include "ObjParser.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>


using namespace std;

ObjParser::ObjParser(const char* filename) {
	string line;
	pointCount = texPointCount = elemCount = normCount = 0;
	vector<string> tokens;
	unordered_map<string, int> dupFinder;
	const array<float, 3> nullVec = {0.f, 0.f, 0.f}; //convenience placeholder because .obj is 1 indexed
	const array<float, 2> nullUV = {0.f, 0.f};
	pointVec.push_back(nullVec);
	texPointVec.push_back(nullUV);
	normVec.push_back(nullVec);

	ifstream inFile(filename);
	if(inFile.is_open()) {
		while(getline(inFile, line)) {
			split(line, ' ', tokens);
			if(tokens[0] == "v") {
				if(tokens.size() > 4) {
					ngonError();
					break;
				}
				pointCount++;
				array<float, 3> point;
				point[0] = stof(tokens[1]);
				point[1] = stof(tokens[2]);
				point[2] = stof(tokens[3]);
				pointVec.push_back(point);
			}
			else if(tokens[0] == "vt") {
				if(tokens.size() > 4) {
					ngonError();
					break;
				}
				texPointCount++;
				array<float, 2> texPoint;
				texPoint[0] = stof(tokens[1]);
				texPoint[1] = stof(tokens[2]);
				texPointVec.push_back(texPoint);
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
				array<unsigned short, 3> elem;
				for(int i = 1; i < tokens.size(); i++){
					array<unsigned short, 3> vert;
					pair<unordered_map<string,int>::iterator, bool> result = dupFinder.insert(make_pair(tokens[i], vertVec.size()));
					if(result.second) {
						vector<string> triplet;
						split(tokens[i], '/', triplet);
						vert[0] = stos(triplet[0]);
						// if no texture coordinate was given the second element is empty
						if(triplet[1].empty()) {
							vert[1] = 0;
						}
						else {
							vert[1] = stos(triplet[1]);
						}
						// if no vertex normal was given the triplet will just be a "doublet"
						if(triplet.size() < 3) {
							vert[2] = 0;
						}
						else {
							vert[2] = stos(triplet[2]);
						}
						elem[i-1] = vertVec.size();
						vertVec.push_back(vert);
					}
					else {
						elem[i-1] = result.first->second;
					}
				}
				elemVec.push_back(elem);
			}
			tokens.clear();
		}
		inFile.close();
	}
}

ObjParser::~ObjParser() {
	// noop
}

void ObjParser::fillVertArray(float* buffer, bool UVs, bool normals) {
	int stride = 3;
	if(UVs) stride += 2;
	if(normals) stride += 3;
	for(int i = 0; i < vertVec.size(); i++) {
		int index = i*stride;
		unsigned short point = vertVec[i][0];
		buffer[index] = pointVec[point][0];
		buffer[index+1] = pointVec[point][1];
		buffer[index+2] = pointVec[point][2];
		if(UVs) {
			unsigned short texPoint = vertVec[i][1];
			buffer[index+3] = texPointVec[texPoint][0];
			buffer[index+4] = texPointVec[texPoint][1];
		}
		if(normals) {
			int offset = 3;
			if(UVs) offset += 2;
			unsigned short norm = vertVec[i][2];
			buffer[index+offset] = normVec[norm][0];
			buffer[index+offset+1] = normVec[norm][1];
			buffer[index+offset+2] = normVec[norm][2];
		}
	}
}

void ObjParser::fillElementArray(unsigned short* buffer) {
	int stride = 3;
	for(int i = 0; i < elemCount; i++) {
		int index = i*stride;
		buffer[index] = elemVec[i][0];
		buffer[index+1] = elemVec[i][1];
		buffer[index+2] = elemVec[i][2];
	}
}

int ObjParser::getVertCount() {
	return vertVec.size();
}

int ObjParser::getPointCount() {
	return pointCount;
}

int ObjParser::gettexPointCount() {
	return texPointCount;
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