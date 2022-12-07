// Illumination using the physically-based model

#version 130

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec2 vertex_uv;
in vec3 lamp_light_interp[1];

// Uniform (global) buffer
uniform sampler2D texture_map;
vec3 lightSources[5];

// General constants
float pi = 3.1415926535897932;

// Material attributes (constants)

// Grayscale lighting with a slight blue ambient hue (nighttime is never pure black)
vec3 diffuse_color = vec3(0.2, 0.2, 0.2);
vec3 specular_color = vec3(1.0, 0.8, 0.2);
vec3 ambient_color = vec3(0.05, 0.05, 0.06);

float roughness = 0.8;
float light_intensity = 1.0*pi;


vec3 lightCalc(vec3 lightPos){

    vec3 N, // Interpolated normal for fragment
            L, // Light-source direction
            V, // View direction
            H; // Half-way vector

    vec3 illum = vec3(0,0,0);
    
    // Initialize input quantities
    N = normalize(normal_interp);

    L = (lightPos - position_interp);
    L = normalize(L);

    vec3 downVector = vec3(0,-1,0);
    float theta = dot(L, normalize(-downVector));

    if (theta > cos(radians(70.0f))) {
        //V = (eye_position - position_interp);
        V = - position_interp; // Eye position is (0, 0, 0) in view coordinates
        V = normalize(V);

        H = 0.5*(V + L); // Halfway vector
        //H = (V + L); // Halfway vector
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
        //vec3 fresnel = (vec3(1.0, 1.0, 1.0) - specular_color)*pow(1 - LH, 2.0);
        vec3 fresnel = specular_color + (vec3(1.0, 1.0, 1.0) - specular_color)*pow(1 - LH, 2.0);


        // GGX distribution function
        float denom = NH*NH*(roughness*roughness - 1.0) + 1.0;
        float ndist = roughness / (pi*denom*denom);

        // Microfacet term: assume implicit geometry function
        vec3 mfacet = fresnel * (ndist / 4.0);

        float epsilon = (cos(radians(30.0f)) - cos(radians(70.0f)));
        float intensity = light_intensity * clamp((theta - cos(radians(70.0f))) / epsilon, 0.0, 1.0);

        // Full illumination
        illum = ambient_color + (diffuse + mfacet)*intensity*NL;        

    } else {
        illum = ambient_color;
    }

    return illum;

}

void main() 
{

    
    vec3 illum = vec3(0,0,0);
    vec4 pixel = texture(texture_map, vertex_uv*10);

    for(int i = 0; i < lamp_light_interp.length(); i++)illum += lightCalc(lamp_light_interp[i]);

    gl_FragColor = vec4(pixel) * vec4(illum, 1.0);
    
}



