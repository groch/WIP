#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
#include "inOutVar_inc.glsl"
} vs_out;

//out vec3 ourColor;
//out vec2 TexCoords;
//out vec3 FragPos;
//out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoord;

    mat3 nm = transpose(inverse(mat3(model)));
    vs_out.Normal = nm * aNormal;
    vec3 T = normalize(nm * aTangent);
    vec3 N = normalize(vs_out.Normal);
    //vec3 N = normalize(nm * aNormal);
//    vec3 B = normalize(nm * aBitangent);

    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(T, N);

    // TBN must form a right handed coord system.
    // Some models have symetric UVs. Check and fix.
    if (dot(cross(N, T), B) < 0.0)
        T = T * -1.0;

    vs_out.TBN = transpose(mat3(T, B, N));
    //vs_out.TBN = mat3(T, B, N);

    vs_out.TangentViewPos  = vs_out.TBN * viewPos;
    vs_out.TangentFragPos  = vs_out.TBN * vs_out.FragPos;


    //ourColor = aColor;
//    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
