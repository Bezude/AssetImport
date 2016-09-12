// objparser enables reading .obj files describing 3D meshes

#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <vector>
#include <string>


class Objparser {
	public:
		Objparser(char* filename);
		~Objparser();

	private:
		int vertCount;
		int elemCount;
		int texVertCount;
		int normCount;
		void split(const std::string &s, char delim, std::vector<std::string> &elems);
		inline void ngonError();
};


#endif /* OBJPARSER_H */