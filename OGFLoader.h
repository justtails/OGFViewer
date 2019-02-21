#pragma once
#include "OGFModel.h"

struct Chunk2564
{
	int header[3];	//1 44 2564
	float info[10];	//data chunk
};

struct Chunk1284
{
	int header[3];	//1 44 284
	char data[40];
};

struct OGFHeader
{
	int count;
	int size;
	char modelname[1024];
	int hz1;		//9
	int pos1;
	int hz2;		//1638
	int pos2;
	int hz3;		//0
	int pos3;
	int hz4;		//1630
	int pos4;
};


struct Bone
{
	int id;
	char bone[8192];
};

struct  structF
{
	int f[2];
};

struct TextData 
{
	int type;
	int size;
	char data[1024];
};

struct Indices
{
	int				count;
	unsigned short	*idx;
};

struct OGFVertex
{
	float x,y,z;
	float t1,t2;
	float aa;
	char flag;
	float t[2];
	float b[2];
	float s[2];
	float d[2];
};

OGFModel *Load(char *fileName,const std::string &pathToTextures);