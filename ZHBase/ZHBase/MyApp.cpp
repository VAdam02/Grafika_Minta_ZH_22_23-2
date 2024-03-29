﻿#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_mesh = nullptr;
}

CMyApp::~CMyApp(void)
{
}

void CMyApp::InitCube()
{
	//struct Vertex{ glm::vec3 position; glm::vec3 normals; glm::vec2 texture; };
	std::vector<Vertex>vertices;
	
	//front									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(1, 1) });
	//back
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 1) });
	//right									 
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 1) });
	//left									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 1) });
	//top									 
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(0, 1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(1, 1) });
	//bottom								 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(0, -1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, -1, 0), glm::vec2(1, 1) });

	std::vector<int> indices(36);
	int index = 0;
	for (int i = 0; i < 6 * 4; i += 4)
	{
		indices[index + 0] = i + 0;
		indices[index + 1] = i + 1;
		indices[index + 2] = i + 2;
		indices[index + 3] = i + 1;
		indices[index + 4] = i + 3;
		indices[index + 5] = i + 2;
		index += 6;
	}

	//
	// geometria definiálása (std::vector<...>) és GPU pufferekbe való feltöltése BufferData-val
	//

	// vertexek pozíciói:
	/*
	Az m_CubeVertexBuffer konstruktora már létrehozott egy GPU puffer azonosítót és a most következő BufferData hívás ezt
	1. bind-olni fogja GL_ARRAY_BUFFER target-re (hiszen m_CubeVertexBuffer típusa ArrayBuffer) és
	2. glBufferData segítségével áttölti a GPU-ra az argumentumban adott tároló értékeit

	*/

	m_CubeVertexBuffer.BufferData(vertices);

	// és a primitíveket alkotó csúcspontok indexei (az előző tömbökből) - triangle list-el való kirajzolásra felkészülve
	m_CubeIndices.BufferData(indices);

	// geometria VAO-ban való regisztrálása
	m_CubeVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétől vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következő csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_CubeVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_CubeVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_CubeVertexBuffer },
		},
		m_CubeIndices
	);
}

void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{
		// hátsó lap
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),
		// elülső lap
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1),
	}
	);

	// és a primitíveket alkotó csúcspontok indexei (az előző tömbökből) - triangle list-el való kirajzolásra felkészülve
	m_SkyboxIndices.BufferData(
		std::vector<int>{
			// hátsó lap
			0, 1, 2,
			2, 3, 0,
			// elülső lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felső
			3, 2, 6,
			3, 6, 7,
	}
	);

	// geometria VAO-ban való regisztrálása
	m_SkyboxVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_SkyboxPos },
		}, m_SkyboxIndices
	);

	// skybox texture
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	m_skyboxTexture.AttachFromFile("assets/xpos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	m_skyboxTexture.AttachFromFile("assets/xneg.png", false, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	m_skyboxTexture.AttachFromFile("assets/ypos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	m_skyboxTexture.AttachFromFile("assets/yneg.png", false, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	m_skyboxTexture.AttachFromFile("assets/zpos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	m_skyboxTexture.AttachFromFile("assets/zneg.png", true, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	// a GL_TEXTURE_MAG_FILTER-t és a GL_TEXTURE_MIN_FILTER-t beállítja az AttachFromFile
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CMyApp::InitCone() {
	std::vector<Vertex> vertices;
	for (int i = 0; i <= N; ++i) {
		for (int j = 0; j <= M; ++j) {
			const float u = i / (float)N;
			const float v = j / (float)M;

			Vertex vert;
			vert.p = glm::vec3(
				(1.0f - v) * cos(u * 2.0f * M_PI),
				v,
				(1.0f - v) * sin(u * 2.0f * M_PI)
			);
			vert.n = glm::normalize(glm::vec3(
				cos(u * 2.0f * M_PI),
				1.0f,
				sin(u * 2.0f * M_PI)
			));
			vert.t = glm::vec2(u, v);

			vertices.push_back(vert);
		}
	}

	std::vector<int> indices;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			const int topLeft = i * (M + 1) + j;
			const int topRight = (i + 1) * (M + 1) + j;
			const int bottomLeft = i * (M + 1) + j + 1;
			const int bottomRight = (i + 1) * (M + 1) + j + 1;

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);

			indices.push_back(topLeft);
			indices.push_back(bottomRight);
			indices.push_back(topRight);
		}
	}


	m_ConeVertexBuffer.BufferData(vertices);

	// és a primitíveket alkotó csúcspontok indexei (az előző tömbökből) - triangle list-el való kirajzolásra felkészülve
	m_ConeIndices.BufferData(indices);

	// geometria VAO-ban való regisztrálása
	m_ConeVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_ConeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétől vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következő csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_ConeVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_ConeVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_ConeVertexBuffer },
		},
		m_ConeIndices
	);
}

void CMyApp::InitSphere() {
	std::vector<Vertex> vertices;
	for (int i = 0; i <= N; ++i) {
		for (int j = 0; j <= M; ++j) {
			float u = i / (float)N;
			float v = j / (float)M;

			Vertex vert;
			vert.p = //GetPos(u, v);
				glm::vec3(
					sin(v * glm::pi<float>() * cos(u * glm::pi<float>() * 2.0f)),
					cos(v * glm::pi<float>()),
					sin(v * glm::pi<float>()) * sin(u * glm::pi<float>() * 2.0f)
				);
			vert.n = //GetNorm(u, v);
				glm::vec3(
					sin(v * glm::pi<float>() * cos(u * glm::pi<float>() * 2.0f)),
					cos(v * glm::pi<float>()),
					sin(v * glm::pi<float>()) * sin(u * glm::pi<float>() * 2.0f)
				);
			vert.t = //GetTex(u, v);
				glm::vec2(1 - u, 1 - v);

			vertices.push_back(vert);
		}
	}

	std::vector<int> indices;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			const int topLeft = i * (M + 1) + j;
			const int topRight = (i + 1) * (M + 1) + j;
			const int bottomLeft = i * (M + 1) + j + 1;
			const int bottomRight = (i + 1) * (M + 1) + j + 1;

			indices.push_back(bottomLeft);
			indices.push_back(topLeft);
			indices.push_back(bottomRight);

			indices.push_back(bottomRight);
			indices.push_back(topLeft);
			indices.push_back(topRight);
		}
	}
	
	m_SphereIndexNum = indices.size();


	m_SphereVertexBuffer.BufferData(vertices);

	// és a primitíveket alkotó csúcspontok indexei (az előző tömbökből) - triangle list-el való kirajzolásra felkészülve
	m_SphereIndices.BufferData(indices);

	// geometria VAO-ban való regisztrálása
	m_SphereVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_ConeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétől vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következő csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_SphereVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_SphereVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_SphereVertexBuffer },
		},
		m_SphereIndices
	);
}

void CMyApp::InitQuad()
{
	//struct Vertex{ glm::vec3 position; glm::vec3 normals; glm::vec2 texture; };
	std::vector<Vertex> vertices =
	{
		{ glm::vec3(-0.5, -0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(0, 0) },
		{ glm::vec3(+0.5, -0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(1, 0) },
		{ glm::vec3(-0.5, +0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(0, 1) },
		{ glm::vec3(+0.5, +0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(1, 1) }
	};

	std::vector<int> indices =
	{
		0,1,2,
		2,1,3
	};

	//
	// geometria definiálása (std::vector<...>) és GPU pufferekbe való feltöltése BufferData-val
	//

	// vertexek pozíciói:
	/*
	Az m_ShieldVertexBuffer konstruktora már létrehozott egy GPU puffer azonosítót és a most következő BufferData hívás ezt
	1. bind-olni fogja GL_ARRAY_BUFFER target-re (hiszen m_ShieldVertexBuffer típusa ArrayBuffer) és
	2. glBufferData segítségével áttölti a GPU-ra az argumentumban adott tároló értékeit

	*/

	m_QuadVertexBuffer.BufferData(vertices);

	// és a primitíveket alkotó csúcspontok indexei (az előző tömbökből) - triangle list-el való kirajzolásra felkészülve
	m_QuadIndices.BufferData(indices);

	// geometria VAO-ban való regisztrálása
	m_QuadVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_ShieldVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétől vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következő csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_QuadVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)),     sizeof(Vertex)>, m_QuadVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_QuadVertexBuffer },
		},
		m_QuadIndices
	);
}

void CMyApp::InitFish()
{
	Vertex verticles[] =
	{
		{ glm::vec3(-0.75, 0.15, 1.0), glm::vec3(sqrtf(2.0f) / 2.0, sqrtf(2.0f) / 2.0, 0.0), glm::vec2(0.0) },
		{ glm::vec3(-0.25, -0.35, 0.0), glm::vec3(sqrtf(2.0f) / 2.0, sqrtf(2.0f) / 2.0, 0.0), glm::vec2(0.0) },
		{ glm::vec3(-0.75, 0.15, -1.0), glm::vec3(sqrtf(2.0f) / 2.0, sqrtf(2.0f) / 2.0, 0.0), glm::vec2(0.0) },

		{ glm::vec3(-0.55, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0)},
		{ glm::vec3(-0.55, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0)},
		{ glm::vec3(-0.25, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0)},

		{ glm::vec3(-0.75, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0)},
		{ glm::vec3(-1.25, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0)},
		{ glm::vec3(-1.25, -1.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0)},
	};

	GLuint indices[] =
	{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8
	};

	m_FishFinsVertexBuffer.BufferData(verticles);
	m_FishFinsIndices.BufferData(indices);

	m_FishFinsVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(Vertex)>, m_FishFinsVertexBuffer },
			{ CreateAttribute<1, glm::vec3, sizeof(glm::vec3), sizeof(Vertex)>, m_FishFinsVertexBuffer },
			{ CreateAttribute<2, glm::vec2, sizeof(glm::vec3) * 2, sizeof(Vertex)>, m_FishFinsVertexBuffer },
		},
		m_FishFinsIndices
		);
}

void CMyApp::InitShaders()
{
	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
	});

	// attributomok osszerendelese a VAO es shader kozt
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
	});

	m_program.LinkProgram();

	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_programSimpleColor.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "simpleColor.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	m_programSimpleColor.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
		});

	m_programSimpleColor.LinkProgram();


	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_programFish.AttachShaders({
		{ GL_VERTEX_SHADER, "fish.vert"},
		{ GL_FRAGMENT_SHADER, "fish.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	m_programFish.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
		});

	m_programFish.LinkProgram();


	// shader program rövid létrehozása, egyetlen függvényhívással a fenti három:
	m_programSkybox.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		}
	);
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	InitShaders();
	InitCube();
	InitSkyBox();
	InitCone();

	InitSphere();

	InitQuad();
	InitFish();

	// egyéb textúrák betöltése
	m_pebblesTexture.FromFile("assets/pebbles.jpg");
	m_coralTexture.FromFile("assets/coral.png");

	// mesh betöltése
	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/coral.obj"));
	m_mesh->initBuffers();
	
	// kamera
	m_camera.SetProj(glm::radians(60.0f), 640.0f / 480.0f, 0.01f, 1000.0f);
	m_camera.SetView(glm::vec3(0.0f, 10.0f, 30.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// korall
	m_coralSettings[0].position = glm::vec3(5.0f, 0.0f, -1.5f);
	m_coralSettings[0].rotation = rand() / (float)RAND_MAX * glm::pi<float>();
	m_coralSettings[1].position = glm::vec3(3.0f, 0.0f, 1.5f);
	m_coralSettings[1].rotation = rand() / (float)RAND_MAX * glm::pi<float>();
	m_coralSettings[2].position = glm::vec3(3.0f, 0.0f, -1.5f);
	m_coralSettings[2].rotation = rand() / (float)RAND_MAX * glm::pi<float>();
	m_coralSettings[3].position = glm::vec3(-5.0f, 0.0f, -3.0f);
	m_coralSettings[3].rotation = rand() / (float)RAND_MAX * glm::pi<float>();

	//buborekok
	for (int i = 0; i < m_bubblePositions.size(); ++i)
	{
		m_bubblePositions[i] = glm::vec3(
			3.0f,
			(rand() / (float)RAND_MAX * 5.0f + 1.0f),
			2.0f);
	}

	//halak
	fishStateArray.push_back(FishState(0.3, 5.0, 0.1));
	fishStateArray.push_back(FishState(0.4, 6.0, 0.2));
	fishStateArray.push_back(FishState(0.8, 2.0, 0.1));

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	m_ElapsedTime += delta_time;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();

	for (auto& fishState : fishStateArray)
	{
		fishState.Param += delta_time * fishState.SizeFactor;
	}
}

void CMyApp::SetTransfUniforms(ProgramObject& program, const glm::mat4& world, const glm::mat4& viewProj)
{
	program.SetUniform("MVP", viewProj * world);
	program.SetUniform("world", world);
	program.SetUniform("worldIT", glm::transpose(glm::inverse(world)));
}

void CMyApp::DrawAquariumBox(glm::mat4 viewProj, glm::mat4 aquaWorld, bool needTop)
{
	static const glm::mat4 faceMatArray[] =
	{
		glm::translate(glm::vec3(0.0, 4.0, 4.0)) * glm::scale(glm::vec3(15.0, 8.0, 1.0)),
		glm::rotate(glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0)) * glm::translate(glm::vec3(0.0, 4.0, 4.0)) * glm::scale(glm::vec3(15.0, 8.0, 1.0)),
		glm::rotate(glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0)) * glm::translate(glm::vec3(0.0, 4.0, 7.5)) * glm::scale(glm::vec3(8.0, 8.0, 1.0)),
		glm::rotate(glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0)) * glm::translate(glm::vec3(0.0, 4.0, 7.5)) * glm::scale(glm::vec3(8.0, 8.0, 1.0)),
		glm::translate(glm::vec3(0.0, 8.0, 0.0)) * glm::rotate(glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0)) * glm::scale(glm::vec3(15.0, 8.0, 1.0))
	};

	int faceNum = (needTop ? 5 : 4);

	glDisable(GL_CULL_FACE);
	m_QuadVao.Bind();
	for (int i = 0; i < faceNum; ++i)
	{
		glm::mat4 tmpWorld = aquaWorld * faceMatArray[i];
		SetTransfUniforms(m_programSimpleColor, tmpWorld, viewProj);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
	m_QuadVao.Unbind();
	glEnable(GL_CULL_FACE);
}

static glm::vec3 GetFishPath(float t)
{
	return glm::vec3(
		6.0f * cosf(t),
		0.0f,
		2.0f * sinf(2.0f * t)
	);
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProj = m_camera.GetViewProj();

	// Shader program bekapcsolása
	m_program.Use();

	//akvárium alja
	glm::mat4 pebblesWorld = glm::rotate(-glm::pi<float>() / 2.0f, glm::vec3(1.0, 0.0, 0.0))
		* glm::scale(glm::vec3(15.0f, 8.0f, 1.0f));

	m_QuadVao.Bind();
	m_program.SetTexture("texImage", 0, m_pebblesTexture);
	SetTransfUniforms(m_program, pebblesWorld, viewProj);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


	// korall
	m_program.SetTexture("texImage", 0, m_coralTexture);

	for (int i = 0; i < 4; i++)
	{
		glm::mat4 coralWorld = glm::translate(m_coralSettings[i].position) *
			glm::rotate(m_coralSettings[i].rotation, glm::vec3(0.0, 1.0, 0.0));
		SetTransfUniforms(m_program, coralWorld, viewProj);
		m_mesh->draw();
	}

	m_QuadVao.Unbind();

	// Shader kikapcs
	m_program.Unuse();


	// fish

	for (auto& fishState : fishStateArray)
	{

		m_programFish.Use();

		m_programFish.SetUniform("color", glm::vec4(1.0, 0.5, 0.0, 1.0));
		m_programFish.SetUniform("ElapsedTime", m_ElapsedTime);

		glm::vec3 fishPos = GetFishPath(fishState.Param);
		glm::vec3 fishForward = glm::normalize(
			GetFishPath(fishState.Param + 0.01f) - fishPos
		);
		glm::vec3 fishRight = glm::rotate(-glm::pi<float>() / 2.0f, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(fishForward, 0.0);

		fishPos.y = (sinf(fishState.Height * m_ElapsedTime * 0.1) * 0.5 + 0.5) * 4.0 + 1.0;

		glm::mat4 fishRot(1.0);
		fishRot[0] = glm::vec4(fishForward, 0.0);
		fishRot[2] = glm::vec4(fishRight, 0.0);


		glm::mat4 fishWorld = glm::translate(fishPos) * fishRot * glm::scale(glm::vec3(fishState.SizeFactor));

		m_FishFinsVao.Bind();

		SetTransfUniforms(m_programFish, fishWorld, viewProj);

		glDisable(GL_CULL_FACE);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr);
		glEnable(GL_CULL_FACE);

		m_FishFinsVao.Unbind();

		m_SphereVao.Bind();

		glm::mat4 fishBodyWorld = fishWorld * glm::scale(glm::vec3(1.0, 0.4, 0.4));
		SetTransfUniforms(m_programFish, fishBodyWorld, viewProj);

		glDrawElements(GL_TRIANGLES, m_SphereIndexNum, GL_UNSIGNED_INT, nullptr);

		m_programFish.Unuse();

		m_programSimpleColor.Use();
		m_programSimpleColor.SetUniform("color", glm::vec4(0.5, 0.5, 0.5, 1.0));

		float EyeOffsetAlongZ = 0.05f * sinf(0.9 + m_ElapsedTime) * glm::pi<float>();

		glm::mat4 fishEyeWorld = fishWorld * glm::translate(glm::vec3(0.9, 0.12, 0.1 + EyeOffsetAlongZ)) * glm::scale(glm::vec3(0.06, 0.06, 0.06));
		SetTransfUniforms(m_programSimpleColor, fishEyeWorld, viewProj);
		glDrawElements(GL_TRIANGLES, m_SphereIndexNum, GL_UNSIGNED_INT, nullptr);

		fishEyeWorld = fishWorld * glm::translate(glm::vec3(0.9, 0.12, -0.1 + EyeOffsetAlongZ)) * glm::scale(glm::vec3(0.06, 0.06, 0.06));
		SetTransfUniforms(m_programSimpleColor, fishEyeWorld, viewProj);
		glDrawElements(GL_TRIANGLES, m_SphereIndexNum, GL_UNSIGNED_INT, nullptr);

		m_programSimpleColor.Unuse();

		m_SphereVao.Unbind();
	}


	// -----------------------------------------------------------------------------------------


	// skybox
	// mentsük el az előző Z-test eredményt, azaz azt a relációt, ami alapján update-eljük a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenlőt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);

	m_SkyboxVao.Bind();
	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", viewProj * glm::translate( m_camera.GetEye()) );
	
	// cube map textúra beállítása 0-ás mintavételezőre és annak a shaderre beállítása
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);
	// az előző három sor <=> m_programSkybox.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	m_programSkybox.Unuse();

	// végül állítsuk vissza
	glDepthFunc(prevDepthFnc);

	//blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_programSimpleColor.Use();

	m_SphereVao.Bind();
	m_programSimpleColor.SetUniform("color", glm::vec4(0.5));

	for (int i = 0; i < m_bubblePositions.size(); ++i)
	{
		glm::mat4 bubbleWorld = glm::translate(m_bubblePositions[i]) *
			glm::scale(glm::vec3(0.1f));

		SetTransfUniforms(m_programSimpleColor, bubbleWorld, viewProj);

		glDrawElements(GL_TRIANGLES, m_SphereIndexNum, GL_UNSIGNED_INT, nullptr);
	}
	m_SphereVao.Unbind();

	//water
	m_programSimpleColor.SetUniform("color", glm::vec4(0.0, 0.1, 0.6, 0.3));
	DrawAquariumBox(viewProj, glm::scale(glm::vec3(0.99, m_waterHeightAspect, 0.99)), true);

	//glass
	m_programSimpleColor.SetUniform("color", glm::vec4(1.0, 1.0, 1.0, 0.1));
	DrawAquariumBox(viewProj, glm::mat4(1.0), false);

	m_programSimpleColor.Unuse();

	glDisable(GL_BLEND);

	//ImGui Testwindow
	ImGui::ShowTestWindow();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}