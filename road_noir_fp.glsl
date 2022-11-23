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
uniform vec3 lampLightPos[1];


// General constants
float pi = 3.1415926535897932;

// Material attributes (constants)

// Grayscale lighting with a slight blue ambient hue (nighttime is never pure black)
vec3 diffuse_color = vec3(0.2, 0.2, 0.2);
vec3 specular_color = vec3(1, 1, 1);
vec3 ambient_color = vec3(0.1, 0.1, 0.12);

float roughness = 0.5;
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

    if (theta > cos(radians(15.0f))) {
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

        // Full illumination
        illum = ambient_color + (diffuse + mfacet)*light_intensity*NL;        


    } else {
        illum = ambient_color;
    }

    return illum;

}

void main() 
{

    /*
    vec3 illum = vec3(0,0,0);
    vec4 pixel = texture(texture_map, vertex_uv);

    for(int i = 0; i < 1; i++)illum += lightCalc(vec3(0.5,10,10));

    // How "grey" it is
    float greyFactor = 1.0;

    // Modify the texture lookups to be greyer
    float grey = 0.21 * pixel.r + 0.71 * pixel.g + 0.07 * pixel.b;
    gl_FragColor = vec4(pixel.rgb * (1.0 - greyFactor) + (grey * greyFactor), pixel.a) * vec4(illum, 1.0);
    */


    // ambient
    vec3 ambient = ambient_color * texture(texture_map, vertex_uv).rgb;
    
    // diffuse 
    vec3 norm = normalize(normal_interp);
    vec3 lightDir = normalize(vec3(0.5, 5, 10) - position_interp);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuse_color * diff * texture(texture_map, vertex_uv).rgb;  
    
    // specular
    vec3 viewDir = normalize(-position_interp);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1/roughness);
    vec3 specular = specular_color * spec * texture(texture_map, vertex_uv).rgb;  
    
    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-vec3(0, -1, 0))); 
    float epsilon = (cos(radians(12.5f)) - cos(radians(17.5f)));
    float intensity = clamp((theta - cos(radians(17.5f))) / epsilon, 0.0, 1.0);
    diffuse  *= light_intensity;
    specular *= light_intensity;
        
    vec3 result = ambient + diffuse + specular;

    if (theta > cos(radians(70.5f))) {
        gl_FragColor = vec4(result, 1.0);
    } else {
        gl_FragColor = vec4(ambient, 1.0);
    }

}



