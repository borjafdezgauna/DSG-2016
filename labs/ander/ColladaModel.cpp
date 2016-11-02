#include "stdafx.h"
#include "ColladaModel.h"



ColladaModel::ColladaModel(char* fileName)
{
	
	tinyxml2::XMLDocument doc;
	doc.LoadFile(fileName);
	//library_image
	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("COLLADA");
	tinyxml2::XMLElement* pLibraryImages = pRoot->FirstChildElement("library_images");
	tinyxml2::XMLElement* pImage = pLibraryImages->FirstChildElement("image");
	tinyxml2::XMLElement* pInitFrom = pImage->FirstChildElement("init_from");
	char* pFileName = (char*)pInitFrom->GetText();
	pFileName = &(pFileName[8]);
	textureId = SOIL_load_OGL_texture(pFileName,0,0,0);

	//library_geometries
	tinyxml2::XMLElement* pLibraryGeometries = pRoot->FirstChildElement("library_geometries");
	tinyxml2::XMLElement* pGeometry = pLibraryGeometries->FirstChildElement("geometry");
	tinyxml2::XMLElement* pMesh = pGeometry->FirstChildElement("mesh");
	tinyxml2::XMLElement* pVenomPos = pMesh->FirstChildElement("source")->FirstChildElement("float_array");
	parseXMLFloatArray(pVenomPos, m_positions);

	tinyxml2::XMLElement* pSource = pMesh->FirstChildElement("source");
	tinyxml2::XMLElement* pVenomNrm = pSource->NextSibling()->FirstChildElement("float_array");
	parseXMLFloatArray(pVenomNrm, m_normals);

	tinyxml2::XMLElement* pVenomUV = pSource->NextSibling()->NextSibling()->FirstChildElement("float_array");
	parseXMLFloatArray(pVenomUV, m_texCoords);

	tinyxml2::XMLElement* pTriangles = pMesh->FirstChildElement("triangles")->FirstChildElement("p");
	parseXMLIntArray(pTriangles,m_index);

}


ColladaModel::~ColladaModel()
{
}

void ColladaModel::draw()
{
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	int index = 0;
	for (int i = 0; i < m_index.size(); i++)
	{
		index= m_index[i];
		glNormal3f(m_normals[index*3], m_normals[index*3 + 1], m_normals[index*3 + 2]);
		glTexCoord2f(m_texCoords[index *2], m_texCoords[index *2 + 1]);
		glVertex3f(m_positions[index *3], m_positions[index *3 +1], m_positions[index *3 + 2]);

	}
	glEnd();
	glPopMatrix();
}



void ColladaModel::parseXMLFloatArray(tinyxml2::XMLElement* pFloatArray, std::vector < double> &vector)
{
	char* pCharArray = (char*)pFloatArray->GetText();
	char* nextToken;
	char* pt = strtok_s(pCharArray, " ", &nextToken);
	while (pt != 0)
	{
		vector.push_back(atof(pt));
		pt = strtok_s(0, " ", &nextToken);
	}
}
void ColladaModel::parseXMLIntArray(tinyxml2::XMLElement* pFloatArray, std::vector<int>&vector)
{
	char* pCharArray = (char*)pFloatArray->GetText();
	char* nextToken;
	char* pt = strtok_s(pCharArray, " ", &nextToken);
	while (pt != 0)
	{
		vector.push_back(atoi(pt));
		pt = strtok_s(0, " ", &nextToken);
	}
}
