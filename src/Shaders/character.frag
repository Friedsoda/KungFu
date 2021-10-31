#version 330 core

struct DirLight {
    vec3 direction;
	
    float ambient;
    float diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform DirLight dirLight;

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    FragColor = CalcDirLight(dirLight, norm, viewDir);
}

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse 
    float diff = max(dot(normal, lightDir), 0.0);
    // specular 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    vec4 ambient = light.ambient * texture(texture_diffuse1, TexCoords);
    vec4 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords);
    vec3 specular = light.specular * spec;
    return (ambient + diffuse + vec4(specular, 1.0));
}
