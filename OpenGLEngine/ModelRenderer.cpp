#include "ModelRenderer.h"

using namespace glm;

void ModelRenderer::Draw(Shader& shader)
{
	shader.Use();

	mat4 mat;
	mat = translate(mat4(1.0f), transform->position);
	mat = rotate(mat, angle(transform->rotation), axis(transform->rotation));
	mat = scale(mat, transform->scale);
	shader.SetUniform("model", mat);
	shader.SetUniform("objectColor", vec3(1.0f));

	model.Draw();
}

void ModelRenderer::Init(const std::string& path)
{
	model.LoadModel(path);
}
