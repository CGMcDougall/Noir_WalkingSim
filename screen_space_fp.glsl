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
	vec2 blurTextureCoords[11];

	// Intensity is determined by distance from the center of the screen divided by max possible distance
	float intensity = distance(uv0, vec2(0.5,0.5)) / distance(vec2(0.0,0.0), vec2(0.5,0.5));

	// Intensity to the power of a sinusoidal function of time determine a pixel's "size" to fade the effect in and out
	float pixelSize = 0.01 * pow(intensity, sin(timer) + 1.5);


	// Gaussian blur, which is faster by using fewer samples
	for(int i = 0; i <= 10; i++) {
		blurTextureCoords[i] = pos + vec2(pixelSize * (i-5), 0.0);
	}

	// Adjusts the pixel by each of the samples, using samples from a Gaussian kernel calculator
	pixel += texture(texture_map, blurTextureCoords[0]) * 0.0093;
	pixel += texture(texture_map, blurTextureCoords[1]) * 0.0280;
	pixel += texture(texture_map, blurTextureCoords[2]) * 0.0659;
	pixel += texture(texture_map, blurTextureCoords[3]) * 0.1217;
	pixel += texture(texture_map, blurTextureCoords[4]) * 0.1757;
	pixel += texture(texture_map, blurTextureCoords[5]) * 0.1985;
	pixel += texture(texture_map, blurTextureCoords[6]) * 0.1757;
	pixel += texture(texture_map, blurTextureCoords[7]) * 0.1217;
	pixel += texture(texture_map, blurTextureCoords[8]) * 0.0659;
	pixel += texture(texture_map, blurTextureCoords[9]) * 0.0280;
	pixel += texture(texture_map, blurTextureCoords[10]) * 0.0093;

    gl_FragColor = pixel;
}
