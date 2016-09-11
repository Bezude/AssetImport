// objparser enables reading .obj files describing 3D meshes

#ifndef OBJPARSER_H
#define OBJPARSER_H

class Objparser {
	public:
		Objparser(char* filename);
		~Objparser();

	private:
		int placeholder;
};


#endif /* OBJPARSER_H */