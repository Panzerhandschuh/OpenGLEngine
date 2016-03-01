#ifndef MESHEXPORTER_H
#define MESHEXPORTER_H

#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <string>
#include "Mesh.h"

class MeshExporter
{
public:
	static void Export(const std::string& filename, Mesh& mesh);
};

#endif