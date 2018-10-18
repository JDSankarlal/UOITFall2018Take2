#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include "Transform.h"
class Mesh:public Transform
{
public:
	Mesh();
	~Mesh();
	
	//- Load a mesh and send it to OpenGL
	bool LoadFromFile(const std::string &file);
	//- Release all data from RAM and GPU 
	void Unload();

	unsigned int GetNumFaces() const;
	unsigned int GetNumVertices() const;
	
	//Open GL buffers and objects
	GLuint VBO_Vertices = 0;
	GLuint VBO_UVs		= 0;
	GLuint VBO_Normals	= 0;
	GLuint VAO			= 0;
	
private:
	unsigned int _NumFaces = 0;
	unsigned int _NumVertices = 0;


};