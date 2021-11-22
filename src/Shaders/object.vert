#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    FragPos = (modelMat * vec4(aPos, 1.0)).xyz;
    TexCoords = aTexCoords;    
    Normal = mat3(transpose(inverse(modelMat))) * aNormal;  
    gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);
}