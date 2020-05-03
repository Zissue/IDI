#version 330 core

out vec4 FragColor;

#define glfc gl_FragCoord

void main() {
    if (glfc.x > 354. && glfc.y > 354.)
        FragColor = vec4(0, 1, 0, 1);
    else if (glfc.x > 354. && glfc.y < 354.)
        FragColor = vec4(1, 0, 0, 1);
    else if (glfc.x < 354. && glfc.y > 354.)
        FragColor = vec4(0, 0, 1, 1);
    else if (glfc.x < 354. && glfc.y < 354.)
        FragColor = vec4(1, 1, 0, 1);
}

