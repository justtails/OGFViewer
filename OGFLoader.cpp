#include "stdafx.h"
#include "OGFLoader.h"
#include "nv_dds.h"

using namespace nv_dds;

void ReadChunk1284(FILE* inputFile, int pos,OGFModel *model,const std::string &pathToTextures)
{
	fseek(inputFile, pos,SEEK_SET);

	Chunk1284 *c1284 = new Chunk1284;
	fread(c1284,sizeof(Chunk1284),1,inputFile);

	if(c1284->header[0] != 1 && c1284->header[1] != 44 && (c1284->header[2] != 1284 || c1284->header[2] != 1028))
	{
		delete c1284;
		return;
	}
	delete c1284;
	TextData *td = new TextData;
	fread(&td->type,sizeof(int),1,inputFile);
	fread(&td->size,sizeof(int),1,inputFile);
	memset(td->data,0,1024);
	fread((char*)td->data,td->size,1,inputFile);

	model->m_MaterialCount++;
	char *materialName = strcat(td->data,".dds");

	std::string fullName(pathToTextures);
	fullName += std::string(materialName);

	CDDSImage image;
	GLuint texobj;

	if(image.load(fullName.c_str()))
	{
		glGenTextures(1, &texobj);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texobj);

		glCompressedTexImage2DARB(GL_TEXTURE_2D, 0, image.get_format(), 
			image.get_width(), image.get_height(), 0, image.get_size(), 
			image);

		for (int i = 0; i < image.get_num_mipmaps(); i++)
		{
			glCompressedTexImage2DARB(GL_TEXTURE_2D, i+1, image.get_format(), 
				image.get_mipmap(i).get_width(), image.get_mipmap(i).get_height(), 0, 
				image.get_mipmap(i).get_size(), image.get_mipmap(i));
		}
		model->m_Material[model->m_MaterialCount-1].m_MaterialID = texobj;
	}else
	{
		model->m_Material[model->m_MaterialCount-1].m_MaterialID = 0;
	}

	int num,size,ident,numverts;
	fread(&num,sizeof(int),1,inputFile);
	fread(&size,sizeof(int),1,inputFile);
	fread(&ident,sizeof(int),1,inputFile);
	//	ident = 302455168;		Проверял на 2х файлах
	fread(&numverts,sizeof(int),1,inputFile);
	if(ident == 302455168)
	{
		OGFVertex *v = new OGFVertex[numverts];
		fread(v,numverts*sizeof(OGFVertex),1,inputFile);

		model->m_Material[model->m_MaterialCount-1].m_VerticesCount = numverts;
		model->m_Material[model->m_MaterialCount-1].m_Vertices = new Vertex[numverts];
		OGFVertex vert;
		Vertex mVertex;
		for(int i=0; i< numverts;i++)
		{
			vert = v[i];
			mVertex.x = vert.x/1.5f; mVertex.y = vert.y; mVertex.z = vert.z; mVertex.tx = vert.s[1]; mVertex.ty = -vert.d[0];
			model->m_Material[model->m_MaterialCount-1].m_Vertices[i] = mVertex;
		}

		int type,datasize,count;
		fread(&type,sizeof(int),1,inputFile);
		fread(&datasize,sizeof(int),1,inputFile);
		fread(&count,sizeof(int),1,inputFile);

		Indices indices;
		indices.count = count;
		indices.idx = new unsigned short[count];

		fread(indices.idx,sizeof(unsigned short)*count,1,inputFile);
		model->m_Material[model->m_MaterialCount-1].m_Indices = new unsigned int[count];
		model->m_Material[model->m_MaterialCount-1].m_IndicesCount = count;

		for (int i=0; i<count;i++)
		{
			model->m_Material[model->m_MaterialCount-1].m_Indices[i] = indices.idx[i];
		}
		delete indices.idx;
		delete v;
	}
}

OGFModel *Load(char *fileName,const std::string &pathToTextures)
{
	FILE *inputFile;
	//MeshBuilder meshBuilder;
	GLeeInit();
	inputFile = fopen(fileName,"rb");
	if(!inputFile)
	{
		//		Log::Get().Write("<*> OGF : File not found. %s", fileName);
		return NULL;
	}

	OGFModel *model = new OGFModel();
	fseek(inputFile,0,SEEK_SET);

	Chunk2564 *c2564 = new Chunk2564;
	fread(c2564,sizeof(Chunk2564),1,inputFile);
	delete c2564;

	OGFHeader header;

	fread( &header.count, sizeof(int),1,inputFile);
	fread( &header.size, sizeof(int),1,inputFile);

	memset(header.modelname,0,1024);

	fread( (char*)header.modelname, header.size,1,inputFile );

	fread( &header.hz1, sizeof(int),1,inputFile);
	header.pos1 = ftell(inputFile);
	fread( &header.hz2, sizeof(int),1,inputFile);
	header.pos2 = ftell(inputFile);
	fread( &header.hz3, sizeof(int),1,inputFile);
	header.pos3 = ftell(inputFile);
	fread( &header.hz4, sizeof(int),1,inputFile);
	header.pos4 = ftell(inputFile);

	ReadChunk1284(inputFile,header.pos1 + header.hz1 + 3,model,pathToTextures);

	fseek(inputFile,header.pos2 + header.hz2,SEEK_SET);

	structF *ds = new structF;
	fread(ds,sizeof(structF),1,inputFile);

	Bone *bone = new Bone;
	fread( &bone->id, sizeof(int),1,inputFile );
	fread( bone->bone, ds->f[1]-sizeof(int),1,inputFile);

	fread(ds,sizeof(structF),1,inputFile);
	delete bone;
	delete ds;

	if(header.hz2 != header.hz4+ 8)
	{
		ReadChunk1284(inputFile,header.pos4 + header.hz4 + 8,model,pathToTextures);
	}
	//Log::Get().Write("<!> OGF : Loaded %s successfull", fileName);
	//inputFile.Close();
	return model;
}