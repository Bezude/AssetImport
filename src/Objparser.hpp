// objparser enables reading .obj files describing 3D meshes

#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <vector>
#include <string>
#include <array>

class ObjParser {
	public:
		ObjParser(char* filename);
		~ObjParser();
		int getVertCount();
		int getTexVertCount();
		int getNormCount();
		int getElemCount();
		void fillVertArray(float*);
		void fillUVArray(float*);
		void fillNormArray(float*);
		void fillElementArray(unsigned short*, bool, bool);

	private:
		int vertCount;
		int elemCount;
		int texVertCount;
		int normCount;
		void split(const std::string &s, char delim, std::vector<std::string> &elems);
		unsigned short stos(std::string s);
		inline void ngonError();
		std::vector< std::array<float, 3> > vertVec;
		std::vector< std::array<float, 2> > texVertVec;
		std::vector< std::array<float, 3> > normVec;
		std::vector< std::array<unsigned short, 9> > elemVec;
};


#endif /* OBJPARSER_H */