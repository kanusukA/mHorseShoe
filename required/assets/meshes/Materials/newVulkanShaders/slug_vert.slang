struct VSInput {
    uint inIndex;
    float3 inPosition;
    float3 inColor;
    float2 inTexCoord;
};



struct UniformBuffer {
    float4x4 model;
    float4x4 view;
    float4x4 proj;
};
ConstantBuffer<UniformBuffer> ubo;

struct SlugBuffer {
    float2 viewport;
    float4 pos;
    float4 tex;
    float4 jac;
    float4 band;
    float4 col;
};
[[vk::binding(1, 0)]]
ConstantBuffer<SlugBuffer> slugBuffer;

// pos.xy = object-space vertex coordinates.
// pos.zw = object-space normal vector.

// tex.xy = em-space sample coordinates.

// tex.z = location of glyph data in band texture (interpreted as integer):

// | 31                         24 | 23                         16 | 15                          8 | 7                           0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |           y coordinate of glyph data in band texture          |           x coordinate of glyph data in band texture          |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+

// tex.w = max band indexes and flags (interpreted as integer):

// | 31                         24 | 23                         16 | 15                          8 | 7                           0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// | 0   0   0 | E | 0   0   0   0 |           band max y          | 0   0   0   0   0   0   0   0 |           band max x          |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+

// jac = inverse Jacobian matrix entries (00, 01, 10, 11).
// bnd = (band scale x, band scale y, band offset x, band offset y).
// col = vertex color (red, green, blue, alpha).


void SlugUnpack(float4 tex, float4 bnd, out float4 vbnd, out int4 vgly)
{
    uint2 g = asuint(tex.zw);
    vgly = int4(g.x & 0xFFFFU, g.x >> 16U, g.y & 0xFFFFU, g.y >> 16U);
    vbnd = bnd;
}

float2 SlugDilate(float4 pos, float4 tex, float4 jac, float4 m0, float4 m1, float4 m3, float2 dim, out float2 vpos)
{
    float2 n = normalize(pos.zw);
    float s = dot(m3.xy, pos.xy) + m3.w;
    float t = dot(m3.xy, n);

    float u = (s * dot(m0.xy, n) - t * (dot(m0.xy, pos.xy) + m0.w)) * dim.x;
    float v = (s * dot(m1.xy, n) - t * (dot(m1.xy, pos.xy) + m1.w)) * dim.y;

    float s2 = s * s;
    float st = s * t;
    float uv = u * u + v * v;
    float2 d = pos.zw * (s2 * (st + sqrt(uv)) / (uv - st * st));

    vpos = pos.xy + d;
    return (float2(tex.x + dot(d, jac.xy), tex.y + dot(d, jac.zw)));
}

struct VertexOutput {
    float4 sv_position : SV_Position;
    float4 color;
    float2 texCoord;
    nointerpolation float4 banding; // Band scale and offset, constant over glyph.
    nointerpolation int4 glyph;
};

[shader("vertex")]
VertexOutput vertMain(VSInput input) {

    float2 p;
    VertexOutput vresult;

    float4x4 matrax = ubo.proj * ubo.view * ubo.model;
    // Apply dynamic dilation to vertex position. Returns new em-space sample position.

    vresult.texCoord = SlugDilate(
        slugBuffer.pos, 
        slugBuffer.tex, 
        slugBuffer.jac, 
        matrax[0], 
        matrax[1], 
        matrax[2], 
        slugBuffer.viewport, 
        p
    );

    // Apply MVP matrix to dilated vertex position.
    vresult.sv_position.x = p.x * matrax[0].x + p.y * matrax[0].y + matrax[0].w;
    vresult.sv_position.y = p.x * matrax[1].x + p.y * matrax[1].y + matrax[1].w;
    vresult.sv_position.z = p.x * matrax[2].x + p.y * matrax[2].y + matrax[2].w;
    vresult.sv_position.w = p.x * matrax[3].x + p.y * matrax[3].y + matrax[3].w;

    // Unpack or pass through remaining vertex data.

    SlugUnpack(slugBuffer.tex, slugBuffer.band, vresult.banding, vresult.glyph);
    vresult.color = slugBuffer.col;
    return (vresult);
}