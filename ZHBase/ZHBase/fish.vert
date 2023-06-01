#version 330 core

// VBO-ból érkező változók
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader külső paraméterei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

uniform float ElapsedTime = 0.0;

void main()
{
	vec3 pos = vs_in_pos + vec3(0.0, 0.0,
		0.05 * sin(vs_in_pos.x + ElapsedTime) * 3.14159);
	gl_Position = MVP * vec4( pos, 1 );
	
	vs_out_pos = (world * vec4(vs_in_pos, 1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = vs_in_pos.xx;
}