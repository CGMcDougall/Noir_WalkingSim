#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 projection_mat;
uniform mat4 view_mat;

void main() {
	vec4 pos = projection_mat * view_mat * vec4(aPos, 1.0f);
	gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
	texCoords = vec3(aPos.x, aPos.y, -aPos.z);
}