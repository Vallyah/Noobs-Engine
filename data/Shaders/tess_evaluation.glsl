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

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(triangles, fractional_odd_spacing, ccw) in;

in vec3 tc_normal[3];
in Coefs tc_coefs[3];

out vec3 te_normal;
out vec3 world_pos;

void main()
{
    vec3 b300 = gl_in[0].gl_Position.xyz;
    vec3 b030 = gl_in[1].gl_Position.xyz;
    vec3 b003 = gl_in[2].gl_Position.xyz;
    vec3 n200 = tc_normal[0];
    vec3 n020 = tc_normal[1];
    vec3 n002 = tc_normal[2];
    vec3 uvw =  gl_TessCoord;
    vec3 uvw2 = uvw * uvw; // square
    vec3 uvw3 = uvw2 * uvw; // cube
    
    // geometry coefficients
    vec3 b210 = vec3(tc_coefs[0].b210, tc_coefs[1].b210, tc_coefs[2].b210);
    vec3 b120 = vec3(tc_coefs[0].b120, tc_coefs[1].b120, tc_coefs[2].b120);
    vec3 b021 = vec3(tc_coefs[0].b021, tc_coefs[1].b021, tc_coefs[2].b021);
    vec3 b012 = vec3(tc_coefs[0].b012, tc_coefs[1].b012, tc_coefs[2].b012);
    vec3 b102 = vec3(tc_coefs[0].b102, tc_coefs[1].b102, tc_coefs[2].b102);
    vec3 b201 = vec3(tc_coefs[0].b201, tc_coefs[1].b201, tc_coefs[2].b201);
    vec3 b111 = vec3(tc_coefs[0].b111, tc_coefs[1].b111, tc_coefs[2].b111);
    
    // normal coefficients
    vec3 n110 = normalize(vec3(tc_coefs[0].n110, tc_coefs[1].n110, tc_coefs[2].n110));
    vec3 n011 = normalize(vec3(tc_coefs[0].n011, tc_coefs[1].n011, tc_coefs[2].n011));
    vec3 n101 = normalize(vec3(tc_coefs[0].n101, tc_coefs[1].n101, tc_coefs[2].n101));
    
    // compute normal
    te_normal = n200 * uvw2[2] + n020 * uvw2[0] + n002 * uvw2[1] +
                n110 * uvw[2] * uvw[0] + n011 * uvw[0] * uvw[1] + n101 * uvw[2] * uvw[1];
    
    // compute position
    uvw2 *= 3.0f;
    vec3 buvw = b300 * uvw3[2] + b030 * uvw3[0] + b003 * uvw3[1] +
                b210 * uvw2[2] * uvw[0] + b120 * uvw2[0] * uvw[2] +
                b201 * uvw2[2] * uvw[1] + b021 * uvw2[0] * uvw[1] +
                b102 * uvw2[1] * uvw[2] + b012 * uvw2[1] * uvw[0] +
                6.0f * b111 * uvw;
    gl_Position = projection * view * model * vec4(buvw,1.0);
    world_pos = (model * vec4(buvw, 1.0f)).xyz;
}
