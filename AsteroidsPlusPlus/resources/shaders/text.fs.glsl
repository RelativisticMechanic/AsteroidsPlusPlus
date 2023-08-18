in vec4 color;
in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D tex;
uniform vec3 text_color;

void main(void)
{
    float alpha = texture(tex, texCoord).a;
    fragColor = vec4(text_color, alpha);
}
