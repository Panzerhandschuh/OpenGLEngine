#include "Bounds.h"

Bounds::Bounds(Model& model)
{
	glm::vec3 min(std::numeric_limits<float>::max());
	glm::vec3 max(std::numeric_limits<float>::min());
	for (int i = 0; i < model.meshes.size(); i++)
	{
		std::vector<glm::vec3>& vertices = model.meshes[i]->vertices;
		for (int j = 0; j < vertices.size(); j++)
		{
			for (int k = 0; k < 3; k++)
			{
				float val = vertices[j][k];
				if (val < min[k])
					min[k] = val;
				if (val > max[k])
					max[k] = val;
			}
		}
	}

	size = max - min;
	center = (min + max) / 2.0f;
}

Bounds::Bounds(Mesh& mesh)
{
	glm::vec3 min(std::numeric_limits<float>::max());
	glm::vec3 max(std::numeric_limits<float>::min());
	std::vector<glm::vec3>& vertices = mesh.vertices;
	for (int i = 0; i < vertices.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			float val = vertices[i][j];
			if (val < min[j])
				min[j] = val;
			if (val > max[j])
				max[j] = val;
		}
	}

	size = max - min;
	center = (min + max) / 2.0f;
}