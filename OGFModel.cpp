#include "StdAfx.h"
#include "OGFModel.h"

OGFModel::OGFModel()
{
	m_MaterialCount = 0;
}

OGFModel::~OGFModel()
{
}

void OGFModel::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glTexCoordPointer(2,GL_FLOAT,sizeof(Vertex),&m_Material->m_Vertices[0].tx);
	glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), &m_Material->m_Vertices[0].x);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_Material[0].m_MaterialID);
	glDrawElements( GL_TRIANGLES, m_Material->m_IndicesCount, GL_UNSIGNED_INT, &m_Material->m_Indices[0]);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
