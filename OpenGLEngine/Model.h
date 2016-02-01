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
#include "Component.h"
#include "Shader.h"

class Model
{
public:
	std::vector<Mesh*> meshes;

	Model();
	Model(std::string path);
	~Model();
	void LoadModel(std::string path);
	Mesh* AddMesh();
	void Draw();

private:
	std::string directory;
	
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
};

#endif