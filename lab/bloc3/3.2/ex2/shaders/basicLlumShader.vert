#version 330 core

in vec3 vertex;
out vec3 vertexFrag;
in vec3 normal;
out vec3 normalFrag;

in vec3 matamb;
out vec3 matAmbFrag;
in vec3 matdiff;
out vec3 matDiffFrag;
in vec3 matspec;
out vec3 matSpecFrag;

in float matshin;
out float matShinFrag;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;


void main()
{	
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);

    // Passem el vector normal a SCO
    mat3 NormalMat = inverse(transpose(mat3(view * TG)));
    normalFrag = NormalMat * normal;

    // Passem la pos dels vertex a SCO
    vec4 vertexSCO = view * TG * vec4(vertex, 1.0);
    vertexFrag = vertexSCO.xyz;

    matAmbFrag = matamb;
    matDiffFrag = matdiff;
    matSpecFrag = matspec;

    matShinFrag = matshin;

    // fcolor = Phong(NORM, L, vertexSCO);
}
