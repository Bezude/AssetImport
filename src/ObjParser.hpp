// objparser enables reading .obj files describing 3D meshes

#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <vector>
#include <string>
#include <array>

class ObjParser {
	public:
		ObjParser(const char* filename);
		~ObjParser();
		int getVertCount();
		int getPointCount();
		int gettexPointCount();
		int getNormCount();
		int getElemCount();
		void fillVertArray(float*, bool, bool);
		void fillElementArray(unsigned short*);

	private:
		int pointCount;
		int elemCount;
		int texPointCount;
		int normCount;
		void split(const std::string &s, char delim, std::vector<std::string> &elems);
		unsigned short stos(std::string s);
		inline void ngonError();
		std::vector< std::array<float, 3> > pointVec;
		std::vector< std::array<float, 2> > texPointVec;
		std::vector< std::array<float, 3> > normVec;
		std::vector< std::array<unsigned short, 3> > elemVec;
		std::vector< std::array<unsigned short, 3> > vertVec;
};


#endif /* OBJPARSER_H */