#version 330 core
in vec3 Normal;
in vec2 TexCoord;
uniform vec3 u_LightDir;
uniform vec3 u_LightColor;
uniform sampler2D u_Texture;
out vec4 FragColor;
void main() {
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, u_LightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;
    vec3 ambient = 0.1 * u_LightColor; // 10% ambient light
    vec3 lighting = ambient + diffuse;
    vec4 texColor = texture(u_Texture, TexCoord);
    FragColor = vec4(lighting, 1.0) * texColor;
}