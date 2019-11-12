layout (location = 0) in vec2 position;
layout (location = 1) in vec2 iuv;

out vec2 uv;

void main()
{
    gl_Position = vec4(position, 0.0f, 1.0f);
    uv = iuv;
}
