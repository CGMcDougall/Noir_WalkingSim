#version 400

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 normal_mat;
uniform float timer;

// Attributes forwarded to the geometry shader
out vec3 vertex_color;
out float timestep;

// Simulation parameters (constants)
uniform vec3 up_vec = vec3(0.0, 1.0, 0.0);
uniform vec3 object_color = vec3(0.1, 0.1, 0.15);

void main()
{
    // Let time cycle every 5 seconds (modified by falling speed)
    float speedOffset = normal.x;
    float timeOffset = normal.y;
    float circtime = timer * speedOffset + timeOffset - 5.0 * floor(timer*speedOffset / 5);
    float t = circtime; // Our time parameter
    

    // Constant used in determining the falling speed coefficient
    float a = 10.0;

    // Vec3 holding the path calculations
    vec3 path = vec3(0.0, 0.0, 0.0);

    // Let's first work in model space (apply only world matrix)
    vec4 originalPosition = world_mat * vec4(vertex, 1.0);
    vec4 position;


	// Parametric equation for a downward spiral 
    path.x = 0;
    path.y = -a*t;
    path.z = 0;

    position.x = path.x + originalPosition.x;
    position.y = path.y + originalPosition.y;
    position.z = path.z + originalPosition.z;
    position.w = 1.0;
    

    // Now apply view transformation
    gl_Position = view_mat * position;
        
    // Define outputs
    // Define color of vertex
    //vertex_color = color.rgb; // Color defined during the construction of the particles
    vertex_color = object_color; // Uniform color 


    // Forward time step to geometry shader
    timestep = t;
}