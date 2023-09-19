﻿#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

out vec3 normal;
out vec3 fragPosition;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 uvScale;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    normal = mat3(transpose(inverse(model))) * inNormal;
    fragPosition = vec3(model * vec4(inPosition, 1.0f));
    texCoords = inTexCoords * uvScale;
}