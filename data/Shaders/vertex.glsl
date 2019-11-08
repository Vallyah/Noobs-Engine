layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inormal;
layout (location = 2) in vec2 iuv;
layout (location = 3) in vec3 itan;
layout (location = 4) in vec3 ibitan;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec2 uv;
out vec3 tangent;
out vec3 bitan;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    normal = inormal;
    uv = iuv;
    tangent = itan;
    bitan = ibitan;
}
