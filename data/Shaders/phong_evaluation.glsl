struct Coefs {
    float b210;
    float b120;
    float b021;
    float b012;
    float b102;
    float b201;
    float b111;
    float n110;
    float n011;
    float n101;
};


uniform mat4 view;
uniform mat4 projection;

layout(triangles, equal_spacing, ccw) in;

in vec3 tc_normal[];
in vec3 pi_sum[];

out vec3 te_normal;
out vec3 te_world_pos;

void main()
{
    vec3 uvw =  gl_TessCoord;
    vec3 uvw2 = uvw * uvw; // square
    
    vec3 Pi = gl_in[0].gl_Position.xyz;
    vec3 Pj = gl_in[1].gl_Position.xyz;
    vec3 Pk = gl_in[2].gl_Position.xyz;
    
    float alpha = 0.75f;
    
    // compute normal
    te_normal = normalize(uvw[0] * tc_normal[0] +
                          uvw[1] * tc_normal[1] +
                          uvw[2] * tc_normal[2]);

    vec3 bar_pos = Pi * uvw[0] + Pj * uvw[1] + Pk * uvw[2];

    // compute pos
    vec3 puv = uvw2[0] * Pi + uvw2[1] * Pj + uvw2[2] * Pk +
               uvw[0] * uvw[1] * pi_sum[0] + uvw[1] * uvw[2] * pi_sum[1] +
               uvw[2] * uvw[0] * pi_sum[2];
    
    vec3 final_pos = (1 - alpha) * bar_pos + alpha * puv;

    gl_Position = projection * view * vec4(final_pos, 1.0f);
    te_world_pos = final_pos;
}
