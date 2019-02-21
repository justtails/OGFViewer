#pragma once

class OGFModel
{
public:
	OGFModel();
	~OGFModel();

	void Render();

	Material	m_Material[2];
	int			m_MaterialCount;
};