layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inormal;

out vec3 v_normal;
uniform mat4 model;

void main()
{
    gl_Position = model * vec4(position, 1.0f);
    v_normal = inormal;
}
