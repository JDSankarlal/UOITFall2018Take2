#define _CRT_SECURE_NO_WARNINGS 
#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <MiniMath/Core.h>
//VERSION VERSION

#define CHAR_BUFFER_SIZE 128
#define BUFFER_OFFSET(i) ((char *)0+(i))

struct MeshFace
{
	MeshFace()
	{
		vertices[0] = 0;
		vertices[1] = 0;
		vertices[2] = 0;
		// No need for UV yet

		textureUVs[0] = 0;
		textureUVs[1] = 0;
		textureUVs[2] = 0;

		normals[0] = 0;
		normals[1] = 0;
		normals[2] = 0;
	}
	MeshFace(unsigned v1, unsigned v2, unsigned v3, unsigned t1, unsigned t2, unsigned t3,unsigned n1, unsigned n2, unsigned n3)
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;
		// No need for UV yet

		textureUVs[0] = t1;
		textureUVs[1] = t2;
		textureUVs[2] = t3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
	}
	unsigned vertices[3];
	unsigned normals[3];
	unsigned textureUVs[3];

};

Mesh::Mesh()
{
}

Mesh::~Mesh()
{

}

bool Mesh::LoadFromFile(const std::string &file)
{
	std::ifstream input;
	input.open(file);

	if (!input)
	{
		std::cout << "Couldn't open file" << std::endl;
		return false;
	}

	char inputString[CHAR_BUFFER_SIZE];

	//Unique data 
	std::vector<vec3> vertexData;
	std::vector<vec2> textureData;
	std::vector<vec3> normalData;

	//index and face data
	std::vector<MeshFace> faceData;
	//OpenGL ready data
	std::vector<float> unPackedVertexData;
	std::vector<float> unPackedTextureData;
	std::vector<float> unPackedNormalData;

	while (!input.eof())
	{
		input.getline(inputString, CHAR_BUFFER_SIZE);

		if (std::strstr(inputString, "#") != nullptr)
		{
			//Comment is on this line
			continue;
		}

		else if (std::strstr(inputString, "vn") != nullptr)
		{
			//This line has normal data
			vec3 temp;
			std::sscanf(inputString, "vn %f %f %f", &temp.x, &temp.y, &temp.z);
			normalData.push_back(temp);
		}

		else if (std::strstr(inputString, "vt") != nullptr)
		{
			//This line has normal data
			vec2 temp;
			std::sscanf(inputString, "vt %f %f", &temp.x, &temp.y);
			textureData.push_back(temp);
		}

		else if (std::strstr(inputString, "v") != nullptr)
		{
			//This line has vertex data
			vec3 temp;
			std::sscanf(inputString, "v %f %f %f", &temp.x, &temp.y, &temp.z);
			vertexData.push_back(temp);
		}

		else if (std::strstr(inputString, "f") != nullptr)
		{
			//This line has vertex data
			MeshFace temp;
			std::sscanf(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u", 
				&temp.vertices[0] ,&temp.textureUVs[0], &temp.normals[0],
				&temp.vertices[1] ,&temp.textureUVs[1], &temp.normals[1],
				&temp.vertices[2] ,&temp.textureUVs[2], &temp.normals[2]);
			
			faceData.push_back(temp);
		}
	}

	input.close();
	//Unpack the data
	for (unsigned i = 0; i < faceData.size(); i++)
	{
		for (unsigned j = 0; j < 3; j++)
		{
			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].x);
			//std::cout << unPackedVertexData[i] << std::endl;
			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].y);
			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].z);

			unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].x);
			unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].y);

			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].x);
			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].y);
			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].z);

		}
	}

	_NumFaces = faceData.size();
	_NumVertices = _NumFaces * 3;

	//Send Data to OpenGL
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_Vertices);
	glGenBuffers(1, &VBO_UVs);
	glGenBuffers(1, &VBO_Normals);

	glBindVertexArray(VAO);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedVertexData.size(), &unPackedVertexData[0],GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedTextureData.size(), &unPackedTextureData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedNormalData.size(), &unPackedNormalData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	vertexData.clear();
	normalData.clear();
	textureData.clear();
	faceData.clear();
	unPackedVertexData.clear();
	unPackedNormalData.clear();
	unPackedTextureData.clear();

	return true;
}

void Mesh::Unload()
{
	glDeleteBuffers(1, &VBO_Normals);
	glDeleteBuffers(1, &VBO_UVs);
	glDeleteBuffers(1, &VBO_Vertices);
	glDeleteVertexArrays(1, &VAO);

	VBO_Normals = 0;
	VBO_UVs = 0;
	VBO_Vertices = 0;

	_NumFaces = 0;
	_NumVertices = 0;

}

unsigned int Mesh::GetNumFaces() const
{
	return _NumFaces;
}

unsigned int Mesh::GetNumVertices() const
{
	return _NumVertices;
}