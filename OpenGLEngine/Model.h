#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

using namespace std;

class Model
{
public:
	Model(GLchar* path);
	~Model();
	void Draw();

private:
	vector<Mesh*> meshes;
	string directory;
	
	void LoadModel(string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
};

#endif