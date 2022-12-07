#version 130

// Passed from the vertex shader
in vec2 uv0;

// Passed from outside
uniform float timer;
uniform sampler2D texture_map;

void main() 
{	
	vec4 pixel;
	vec2 pos = uv0;
	pixel = vec4(0.0);

	pixel = texture(texture_map, pos);
	
    float greyFactor = 1.0;

    // Modify the texture lookups to be greyer
    float grey = 2.51 * pixel.r + 0.71 * pixel.g + 0.07 * pixel.b;
    gl_FragColor = vec4(pixel.rgb * (1.0 - greyFactor) + (grey * greyFactor), pixel.a);

}
