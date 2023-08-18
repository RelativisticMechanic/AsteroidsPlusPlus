in vec4 color;
in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D tex;
uniform float time;
uniform float particle_size;

void main(void)
{
    fragColor = vec4(0.75, 0.25, 0.0, 1.0);
}
