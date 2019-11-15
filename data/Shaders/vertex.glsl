layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inormal;

out vec3 v_normal;

void main()
{
    v_normal = inormal;
}
