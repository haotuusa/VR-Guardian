#include "Model.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>


using namespace std;
#include <Windows.h>

Model::Model(GLchar* path, string directory)
{
	this->loadModel(path);
	this->directory = directory;
}
void Model::setToWorld(glm::mat4 toWorld)
{
	this->toWorld = toWorld;
}
void Model::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView)
{
	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].draw(shaderProgram, projection, modelView * toWorld);
}
void Model::loadModel(string path)
{	
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		char buff[100];
		sprintf_s(buff, "ERROR::ASSIMP:: %s \n", import.GetErrorString());
		OutputDebugStringA(buff);
		return;
	}
	this->directory = path.substr(0, path.find_last_of('/'));

	this->processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode* node, const aiScene* scene)
{
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// Process vertex positions 
		glm::vec3 inPos;
		inPos.x = mesh->mVertices[i].x;
		inPos.y = mesh->mVertices[i].y;
		inPos.z = mesh->mVertices[i].z;
		vertex.position = inPos;

		//Now for the vertex normals 
		glm::vec3 inNorm;	
		/*inNorm.x = mesh->mNormals[i].x;
		inNorm.y = mesh->mNormals[i].y;
		inNorm.z = mesh->mNormals[i].z;*/
		vertex.normal = inNorm;

		//Now for the texCoords
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// Process indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	glm::vec3 color;
	// Process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];

		// For the project 1 special case
		aiColor3D diffuseColor(0.0f, 0.0f, 0.0f);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
		color.x = diffuseColor.r;
		color.y = diffuseColor.g;
		color.z = diffuseColor.b;	

		//For the more general case 
		vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertices, indices, textures, color);
}
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = textureFromFile(str.C_Str(), this->directory);
		texture.type = typeName;
		//texture.path = str;
		textures.push_back(texture);
	}
	return textures;
}
unsigned int Model::textureFromFile(const char *path, const string &directory)
{
	string filename = string(path);
	filename = directory + '/' + filename;
	//cout << " The filename is " << filename << endl;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}
void Model::spin(float deg, glm::vec3 axis)
{
	// If you haven't figured it out from the last project, this is how you fix spin's behavior
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), axis);
}
Model::~Model()
{
}
