uniform float lod;

layout(vertices=3) out;

in vec3 v_normal[];

out vec3 tc_normal[3];
out vec3 pi_sum[3];

vec3 pi(int i, vec3 q)
{
    vec3 Pi = gl_in[i].gl_Position.xyz;
    vec3 Ni = v_normal[i];
    return (q - dot(q - Pi, Ni) * Ni);
}

void main()
{
    // Pi, Pj and Pk
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    // Ni, Nj and Nk
    tc_normal[gl_InvocationID] = v_normal[gl_InvocationID];

    int next = (gl_InvocationID + 1) % 3;

    // compute coefs
    pi_sum[gl_InvocationID] = pi(gl_InvocationID, gl_in[next].gl_Position.xyz) +
                              pi(next, gl_in[gl_InvocationID].gl_Position.xyz);

    // set tesselation levels (~ level of detail)
    gl_TessLevelOuter[gl_InvocationID] = lod + 1.0f;
    gl_TessLevelInner[0] = lod + 1.0f;
}
