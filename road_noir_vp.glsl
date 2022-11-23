// Illumination using the physically-based model

#version 130

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;
in vec2 uv;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;
uniform mat4 normal_mat;

// Uniform (global) buffer
//uniform sampler2D texture_map;
vec3 lightSources[5];
uniform vec3 lampLightPos[1];


// Attributes forwarded to the fragment shader
out vec3 position_interp;
out vec3 normal_interp;
out vec2 vertex_uv;
out vec3 lamp_light_interp[1];


void main()
{
    // Transform vertex position
    gl_Position = projection_mat * view_mat * world_mat * vec4(vertex, 1.0);

    // Transform vertex position without including projection
    position_interp = vec3(view_mat * world_mat * vec4(vertex, 1.0));

    // Transform normal
    normal_interp = vec3(normal_mat * vec4(normal, 0.0));

    // Transform light position to align with view
    for (int i = 0; i < lampLightPos.length(); i++) {
        lamp_light_interp[i] = vec3(view_mat * vec4(lampLightPos[i], 1.0));
    }
    

    vertex_uv = uv;
}
