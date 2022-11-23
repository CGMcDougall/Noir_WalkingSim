#version 400

// Attributes passed from the geometry shader
in vec4 frag_color;
in float time;
in vec2 uv;

uniform sampler2D texture_map;

void main (void)
{
    // Very simple fragment shader, but we can do anything we want here
    // We could apply a texture to the particle, illumination, etc.

    vec4 pixel = texture(texture_map,uv);
    
    
    float alpha = 1;
    alpha = (1.0f/((time+0.01f)/4.0f));

   
    
    //gl_FragColor = frag_color;
    gl_FragColor = pixel;
   
}
