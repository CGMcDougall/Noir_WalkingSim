#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 color_interp;
in vec2 uv_interp;
in vec3 light_pos;

// Uniform (global) buffer
uniform sampler2D texture_map;


void main() 
{
    // Retrieve texture value
	vec2 uv_use = 2*uv_interp;
    vec4 pixel = texture(texture_map, uv_use);
	//vec4 pixel = color_interp;

    vec4 lightcol = vec4(1,1,1,1);
	vec3 v = vec3(0,0,0);
	vec3 vv = normalize(v - position_interp);
	
	

	//l = normalize(light_pos - position_interp);
	vec3 l = normalize(vec3(100,0,0) - position_interp);

	vec3 lv = normalize(l - position_interp);
	vec3 h = normalize((vv+lv)/2);
	vec3 n = normalize(normal_interp);

	float diffuse = dot(n,lv);
	float spec = max(0.0,dot(n,h));
	
	//v = normalize(position_interp - normal_interp);
	vec3 r = (0,0,-l + 2 * n * (l*n));
	spec = max(0.0,dot(v,r));
	
	float amb = 5;
	spec = pow(spec,41);


    // Use texture in determining fragment colour

    gl_FragColor = pixel;
	
	//gl_FragColor = lightcol*pixel*diffuse +
	//lightcol*vec4(1,1,1,1)*spec +
	//lightcol*pixel*amb;

}
