#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

// mesh
#include "includes/ObjParser_OGL3.h"

class CMyApp
{
public:
	CMyApp();
	~CMyApp();

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

protected:
	// shaderekhez szükséges változók
	ProgramObject		m_program;			// mesh shader
	ProgramObject		m_programSkybox;	// skybox shader
	ProgramObject		m_programSimpleColor;	// mesh shader

	VertexArrayObject	m_CubeVao;			// VAO
	IndexBuffer			m_CubeIndices;		// index buffer
	ArrayBuffer			m_CubeVertexBuffer;	// VBO

	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;	
	ArrayBuffer			m_SkyboxPos;		

	VertexArrayObject	m_ConeVao;
	IndexBuffer			m_ConeIndices;
	ArrayBuffer			m_ConeVertexBuffer;

	VertexArrayObject	m_SphereVao;
	IndexBuffer			m_SphereIndices;
	ArrayBuffer			m_SphereVertexBuffer;
	GLsizei				m_SphereIndexNum;

	VertexArrayObject	m_QuadVao;
	IndexBuffer			m_QuadIndices;
	ArrayBuffer			m_QuadVertexBuffer;
	
	gCamera				m_camera;

	Texture2D			m_pebblesTexture;
	Texture2D			m_coralTexture;
	TextureCubeMap		m_skyboxTexture;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	// mesh adatok
	std::unique_ptr<Mesh> m_mesh;

	// a jobb olvashatóság kedvéért
	void InitShaders();
	void InitCube();
	void InitSkyBox();
	void InitCone();

	void InitSphere();

	void InitQuad();

	static void SetTransfUniforms(ProgramObject& program, const glm::mat4& viewProj, const glm::mat4& world);

	const int N = 30, M = 30;

	struct CoralSettings
	{
		glm::vec3 position;
		float rotation;
	} m_coralSettings[4];

	std::array<glm::vec3, 3> m_bubblePositions;

	void DrawAquariumBox(glm::mat4 viewProj, glm::mat4 aquaWorld, bool needTop = false);
};

