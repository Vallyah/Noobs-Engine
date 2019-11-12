layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inormal;
layout (location = 2) in vec2 iuv;
layout (location = 3) in vec3 itan;
layout (location = 4) in vec3 ibitan;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 dirlight_spacematrix;

out vec3 world_pos;
out vec3 normal;
out vec2 uv;
out vec3 tangent;
out vec3 bitan;
out vec4 pos_dirlightspace;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    world_pos = (model * vec4(position, 1.0f)).xyz;
    pos_dirlightspace = dirlight_spacematrix * vec4(world_pos, 1.0f);
    normal = inormal;
    uv = iuv;
    tangent = itan;
    bitan = ibitan;
}
