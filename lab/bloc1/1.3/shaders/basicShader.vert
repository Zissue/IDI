#version 330 core

in vec3 vertex;

in vec3 vcolor;
out vec3 fcolor;

uniform float scale;
uniform mat4 TG;

void main()  {
	fcolor = vcolor;
    gl_Position = TG * vec4 (scale * vertex, 1.0);
}
