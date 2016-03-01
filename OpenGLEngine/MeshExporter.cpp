#include "MeshExporter.h"

using namespace std;
using namespace glm;

void MeshExporter::Export(const std::string& filename, Mesh& mesh)
{
	ofstream file;
	file.open(filename);

	vector<vec3>& verts = mesh.vertices;
	vector<vec3>& norms = mesh.normals;
	vector<vec2>& uvs = mesh.uvs;
	vector<GLuint>& indices = mesh.indices;
	for (int i = 0; i < verts.size(); i++)
		file << "v " << verts[i].x << " " << verts[i].y << " " << verts[i].z << endl;
	for (int i = 0; i < norms.size(); i++)
		file << "vn " << norms[i].x << " " << norms[i].y << " " << norms[i].z << endl;
	for (int i = 0; i < uvs.size(); i++)
		file << "vt " << uvs[i].x << " " << -uvs[i].y << endl;
	for (int i = 0; i < indices.size(); i += 3)
		file << "f " << indices[i] + 1 << "/" << indices[i] + 1 << "/" << indices[i] << " " <<
		indices[i + 1] + 1 << "/" << indices[i + 1] + 1 << "/" << indices[i + 1] << " " <<
		indices[i + 2] + 1 << "/" << indices[i + 2] + 1 << "/" << indices[i + 2] << endl;

	file.close();

	cout << "Saved " << filename << endl;
}
