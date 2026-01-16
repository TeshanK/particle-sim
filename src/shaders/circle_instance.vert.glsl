#version 430 core
layout (location = 0) in vec3 aPos;

layout (std430, binding = 0) buffer PositionBlock {
    vec2 positions[];
};

layout (std430, binding = 1) buffer ColorBlock {
    vec4 colors[];
};

flat out vec3 fragColor;
out vec2 vUV;   // local coordinates in [-1, 1]

uniform mat4 projection;
uniform float radius;

void main()
{
    vec2 instancePos = positions[gl_InstanceID];

    vec3 worldPos = vec3(aPos.xy * radius * 2.0 + instancePos, 0.0);

    gl_Position = projection * vec4(worldPos, 1.0);

    // quad vertices are in [-0.5, 0.5], map to [-1, 1]
    vUV = aPos.xy * 2.0;

    fragColor = colors[gl_InstanceID].xyz;
}