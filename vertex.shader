#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
out vec3 Normal;
out vec2 TexCoord;
uniform mat4 u_MVP;
uniform mat4 u_Model;
void main() {
    gl_Position = u_MVP * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    TexCoord = aTexCoord;
}