#ifndef MODELRENDERER_H
#define MODELRENDERER_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <string>
#include "Component.h"
#include "Transform.h"
#include "Model.h"

class ModelRenderer : public Component
{
public:
	void Draw(Shader& shader);
	void Init(const std::string& path);

private:
	Model model;
};

#endif