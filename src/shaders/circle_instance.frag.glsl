#version 430 core
in vec2 vUV;
flat in vec3 fragColor;
out vec4 FragColor;

void main()
{
    float fade = 0.02;
    vec2 uv = vUV;
    
    float invertedDistance = 1.0 - dot(uv, uv);
    float alpha = smoothstep(0.0, fade, invertedDistance);

    if (alpha <= 0.0)
        discard;

    FragColor = vec4(fragColor, alpha);
}

