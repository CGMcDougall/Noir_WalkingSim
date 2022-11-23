#version 400

// Definition of the geometry shader
layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

// Attributes passed from the vertex shader
in vec3 vertex_color[];
in float timestep[];
in vec2 uv_interp[];

// Uniform (global) buffer
uniform mat4 projection_mat;

// Simulation parameters (constants)
uniform float particle_size = 0.01;

// Attributes passed to the fragment shader
out vec4 frag_color;
out float time;
out vec2 uv;

// add any other attributes you want here

void main(void){

    time = timestep[0];
    // Get the position of the particle
    vec4 position = gl_in[0].gl_Position;

    // Define particle size
    float p_size = particle_size*0.5; //

    // Define the positions of the four vertices that will form a quad 
    // The positions are based on the position of the particle and its size
    // We simply add offsets to the position (we can think of it as the center of the particle),
    // since we are already in camera space
 
    vec4 v[6];
    
    v[0] = vec4(position.x, position.y - p_size, position.z, 1.0);  //new
    v[1] = vec4(position.x - 0.5*p_size, position.y - 0.5*p_size, position.z, 1.0);
    v[2] = vec4(position.x + 0.5*p_size, position.y - 0.5*p_size, position.z, 1.0);

    v[3] = vec4(position.x - 0.5*p_size, position.y + 0.5*p_size, position.z, 1.0);

    v[4] = vec4(position.x + 0.5*p_size, position.y + 0.5*p_size, position.z, 1.0);
    v[5] = vec4(position.x, position.y + p_size, position.z, 1.0);  //new

   //v[4] = vec4(position.x + 0.5*p_size, position.y + 0.5*p_size, position.z, 1.0);
    //v[5] = vec4(position.x, position.y + p_size, position.z, 1.0);  //new
    
    //v[4] = vec4(position.x + p_size, position.y, position.z, 1.0);  //new
    //v[5] = vec4(position.x - p_size, position.y, position.z, 1.0);  //new
  
    
	

    // Create the new geometry: a quad with four vertices from the vector v
    for (int i = 0; i < 6; i++){
	
        gl_Position = projection_mat * v[i];
        frag_color = vec4(vertex_color[0], 1.0);
        //uv = uv_interp[0];
        //vec2 cord = vec2(max(1,v[i].x),max(1,v[i].y));

        //float x = mod(v[i].x/(position.x + p_size),1);
        //float y = mod(v[i].y/(position.y + p_size),1);

        float x = ((v[i].x - position.x)/p_size)*2;
        float y = ((v[i].x - position.x)/p_size);
        
        vec2 cord = vec2(x,y);
        uv = cord;
        EmitVertex();
     }

     EndPrimitive();
}
