/* Compute control points */

// As the control shader will be called 3 times (one for each vertex)
// we work on one composant of the coeficients each time.
struct Coefs {
    /* Geometry */
    float b210;
    float b120;
    float b021;
    float b012;
    float b102;
    float b201;
    float b111;
    // b003, b030, b300 are already known
    /* Normal */
    float n110;
    float n011;
    float n101;
    // n002, n020, n200 are already known
};

uniform float lod;

layout(vertices=3) out;

in vec3 v_normal[];

out vec3 tc_normal[3];
out Coefs tc_coefs[3];

float wij(int i, int j)
{
    return dot(gl_in[j].gl_Position.xyz - gl_in[i].gl_Position.xyz, v_normal[i]);
}

float vij(int i, int j)
{
    vec3 vec_PiPj = gl_in[j].gl_Position.xyz - gl_in[i].gl_Position.xyz;
    float num = dot(vec_PiPj, v_normal[i] + v_normal[j]);
    return 2.0f * num / dot(vec_PiPj, vec_PiPj);
}

void main()
{
    // b003, b030, b300
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    // n002, n020, n200
    tc_normal[gl_InvocationID] = v_normal[gl_InvocationID];
    
    // known data
    float P0 = gl_in[0].gl_Position[gl_InvocationID];
    float P1 = gl_in[1].gl_Position[gl_InvocationID];
    float P2 = gl_in[2].gl_Position[gl_InvocationID];
    float N0 = v_normal[0][gl_InvocationID];
    float N1 = v_normal[1][gl_InvocationID];
    float N2 = v_normal[2][gl_InvocationID];
    
    // compute coefs
    /* Geometry */
    tc_coefs[gl_InvocationID].b210 = ((2.0f * P0 + P1) - wij(0,1) * N0) / 3.0f;
    tc_coefs[gl_InvocationID].b120 = ((2.0f * P1 + P0) - wij(1,0) * N1) / 3.0f;
    tc_coefs[gl_InvocationID].b021 = ((2.0f * P1 + P2) - wij(1,2) * N1) / 3.0f;
    tc_coefs[gl_InvocationID].b012 = ((2.0f * P2 + P1) - wij(2,1) * N2) / 3.0f;
    tc_coefs[gl_InvocationID].b102 = ((2.0f * P2 + P0) - wij(2,0) * N2) / 3.0f;
    tc_coefs[gl_InvocationID].b201 = ((2.0f * P0 + P2) - wij(0,2) * N0) / 3.0f;
    
    float E = (tc_coefs[gl_InvocationID].b012 + tc_coefs[gl_InvocationID].b021 +
               tc_coefs[gl_InvocationID].b102 + tc_coefs[gl_InvocationID].b120 +
               tc_coefs[gl_InvocationID].b201 + tc_coefs[gl_InvocationID].b210) / 6.0f;
    float V = (P0 + P1 + P2) / 3.0f;
    
    tc_coefs[gl_InvocationID].b111 = E + (E - V) / 2.0f;
    
    /* Normal */
    // We can't normalize yet because it's only one composant of the vector
    // Therefore, we postpone it to the evaluation shader.
    tc_coefs[gl_InvocationID].n110 = N0 + N1 - vij(0,1) * (P1 - P0);
    tc_coefs[gl_InvocationID].n011 = N1 + N2 - vij(1,2) * (P2 - P1);
    tc_coefs[gl_InvocationID].n101 = N2 + N0 - vij(2,0) * (P0 - P2);
    
    // set tesselation levels (~ level of detail)
    gl_TessLevelOuter[gl_InvocationID] = lod + 1.0f;
    gl_TessLevelInner[0] = lod + 1.0f;
}
