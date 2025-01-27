#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>

using namespace std;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};
struct Texture
 {
	GLuint id;
	string type;
};
class Mesh
{
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	glm::mat4 toWorld;
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, glm::vec3 color);
	void draw(GLint shaderProgram, glm::mat4 projection, glm::mat4 modelView);
	void setupTexturesForDraw();
	~Mesh();
private:
	/*  Render data  */
	GLuint VAO, VBO, EBO, TBO;
	GLuint uProjection, uModelView, uColor;
	glm::vec3 color;
	/*  Functions    */
	void setupMesh();
};

