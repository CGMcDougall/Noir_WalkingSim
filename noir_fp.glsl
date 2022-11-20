// Illumination using the physically-based model

#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec3 light_pos;
in vec2 vertex_uv;

// Uniform (global) buffer
uniform sampler2D texture_map;
vec3 lightSources[5];


// General constants
float pi = 3.1415926535897932;

// Material attributes (constants)

// Blue plastic
vec3 diffuse_color = vec3(0.0, 0.0, 0.5);
vec3 specular_color = vec3(0.1, 0.1, 0.1);
vec3 ambient_color = vec3(0, 0, 0);

float roughness = 0.5;
float light_intensity = 1.0*pi;


vec3 lightCalc(vec3 lightPos){



  vec3 N, // Interpolated normal for fragment
         L, // Light-source direction
         V, // View direction
         H; // Half-way vector

    // Initialize input quantities
    N = normalize(normal_interp);

    L = (lightPos - position_interp);
    L = normalize(L);

    // V = (eye_position - position_interp);
    V = - position_interp; // Eye position is (0, 0, 0) in view coordinates
    V = normalize(V);

    //H = 0.5*(V + L); // Halfway vector
    H = (V + L); // Halfway vector
    H = normalize(H);

    // Compute useful quantities
    float NH = max(dot(N, H), 0.0);
    float LH = max(dot(L, H), 0.0);
    float NL = max(dot(N, L), 0.0);

    // Compute shading model
    // Diffuse term (subsurface scattering)
    vec3 diffuse = diffuse_color / pi;

    // Microfacet geometry
    // Fresnel term
    //vec3 fresnel = (vec3(1.0, 1.0, 1.0) - specular_color)*pow(1 - LH, 5.0);
    vec3 fresnel = specular_color + (vec3(1.0, 1.0, 1.0) - specular_color)*pow(1 - LH, 5.0);


    // GGX distribution function
    float denom = NH*NH*(roughness*roughness - 1.0) + 1.0;
    float ndist = roughness / (pi*denom*denom);

    // Microfacet term: assume implicit geometry function
    vec3 mfacet = fresnel * (ndist / 4.0);

    // Full illumination
    vec3 illum = ambient_color + (diffuse + mfacet)*light_intensity*NL;
    return illum;

}

vec3 lightingCal(vec3 lightPos, vec3 norm, vec3 viewDir){

      vec3 lightDir = normalize(lightPos - position_interp);
      
      float diff = max(dot(norm,lightDir),0.0);

      vec3 reflectDir = reflect(-lightDir,norm);
      float spec = pow(max(dot(viewDir,reflectDir),0.0),10);

      float dist = length(lightPos - position_interp);
      
      //float attenuation = 1.0/(l)

      vec3 ambi = vec3(0.2,0.2,0.2);
      //vec3 diffuse = 


      return vec3(0,0,0);
}




void main() 
{

    vec3 illum= vec3(0,0,0);
       
    
    vec4 pixel = texture(texture_map, vertex_uv);
    //vec3 pixel = text

  
    
	illum = lightCalc(light_pos);
    for(int i = 0; i < 5; i++)illum += lightCalc(lightSources[i]);

    // Assign illumination to the fragment
    gl_FragColor = vec4(illum, 1.0);
    //gl_FragColor = pixel * illum;

}



