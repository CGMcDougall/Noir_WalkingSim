#version 400

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;
in vec2 uv;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 normal_mat;
uniform float timer;

// Attributes forwarded to the geometry shader
out vec3 vertex_color;
out float timestep;
out vec2 uv_interp;

// Simulation parameters (constants)
uniform vec3 up_vec = vec3(0.0, 1.0, 0.0);
uniform vec3 object_color = vec3(0.8, 0.8, 0.8);
float grav = -0.15; // Gravity
float speed = 2.0; // Controls the speed of the explosion
float upward = 0.0; // additional y velocity for all particles

void main()
{
    //lets slow down time
    float time = timer/4;

    // Let time cycle every four seconds
    float circtime = mod((time+color.x),1);
    float t = circtime; // Our time parameter
    float halfT = (4+color.x)/2;
    
    // Let's first work in model space (apply only world matrix)
    vec4 position = world_mat * vec4(vertex, 1.4f);
    vec4 norm = normal_mat * vec4(normal, 0.0);

    // Move point along normal and down with t*t (acceleration under gravity)
    

     //position.x = (sin(t));
    //if(color.y > 0.5)position.x += log(t) * cos(t)/3 * (t*(color.x));
    //else position.x += log(t) * cos(t) * (t*abs(color.y));
    position.x += log(t) * sin(t)/3 * (t*color.x);
	
	// add "upward" to y speed to launch the particles vertically -- can easily omit
    //position.y += (upward+norm.y)*t*speed - grav*speed*up_vec.y*t*t;
    //position.y += -grav*speed*up_vec.y*t*t*(min(0.5,color.y));

    //position.y += abs((norm.y*t*speed - grav*speed*up_vec.x*t*t));

    position.y += t/2;


    position.z += 0;
    
    // Now apply view transformation
    gl_Position = view_mat * position;
        
    // Define outputs
    // Define color of vertex
    //vertex_color = color.rgb; // Color defined during the construction of the particles
    //vertex_color = object_color; // Uniform color 
    //vertex_color = vec3(t, 0.0, 1-t); // red-purple dynamic color
    float cts =  t/(4+color.x);
    vertex_color = vec3(.5+cts, .5+cts, .5+cts); // red-yellow dynamic color

    // Forward time step to geometry shader
    timestep = t;
    
    uv_interp = uv;
    
}
