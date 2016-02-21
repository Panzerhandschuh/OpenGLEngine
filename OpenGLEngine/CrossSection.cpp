#include "CrossSection.h"

using namespace glm;
using namespace std;

bool FloatCompare::operator()(const float a, const float b)
{
	const float roundFactor = 0.015625f; // 1 / 64 (6 bits of precision)
	return (MathUtil::RoundToNearest(a, roundFactor) < MathUtil::RoundToNearest(b, roundFactor));
}

CrossSection::CrossSection(float tValue, vector<int> indices)
{
	this->tValue = tValue;
	this->indices = indices;
}

vector<CrossSection> CrossSection::GetCrossSections(Mesh& mesh)
{
	Bounds bounds(mesh);
	float length = bounds.size.z;
	float extents = length / 2.0f;
	float offset = extents - bounds.center.z;
	vector<vec3>& verts = mesh.vertices;

	// Use dictionary so vertex positions are grouped by an coordinate cross section (useful for optimizing path deformation)
	map<float, vector<int>, FloatCompare> crossSectionIndices;
	for (int i = 0; i < verts.size(); i++)
	{
		float vertPos = verts[i].z;
		if (crossSectionIndices.count(vertPos)) // Found key
			crossSectionIndices[vertPos].push_back(i);
		else // Key not found
			crossSectionIndices.insert(pair<float, vector<int>>(vertPos, vector<int> { i }));
	}

	// Sort dictionary keys
	vector<float> keys(crossSectionIndices.size());
	for (map<float, vector<int>>::iterator it = crossSectionIndices.begin(); it != crossSectionIndices.end(); ++it)
		keys.push_back(it->first);
	sort(keys.begin(), keys.end());

	// Copy dictionary to 2D array using sorted keys
	vector<CrossSection> crossSections(keys.size());
	for (int i = 0; i < keys.size(); i++)
	{
		// Get vertex position relative to the start of the mesh
		float pos = keys[i] + offset;
	
		// Get vertex position as a percent from the start of the mesh
		float tValue = pos / length;
	
		vector<int>& indices = crossSectionIndices[keys[i]];
		crossSections[i] = CrossSection(tValue, indices);
	}
	
	return crossSections;
}
