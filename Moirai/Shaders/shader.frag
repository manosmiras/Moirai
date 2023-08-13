#version 330 core
out vec4 fragColor;

in vec3 color;
in vec2 texCoords;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    fragColor = mix(texture(texture1, texCoords), texture(texture2, texCoords), 0.2);
}