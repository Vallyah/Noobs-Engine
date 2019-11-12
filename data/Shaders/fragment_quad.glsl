in vec2 uv;

out vec4 color;

uniform sampler2D depthMap;

void main()
{             
    float depth = texture(depthMap, uv).x;
    color = vec4(vec3(depth), 1.0);
} 
