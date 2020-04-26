#pragma once

#include <vector>

class Model
{
public:
	std::vector<float> modelVertexPositions;
	std::vector<float> modelTexturePositions;
	std::vector<float> modelNormals;

	void loadModelData();

};
