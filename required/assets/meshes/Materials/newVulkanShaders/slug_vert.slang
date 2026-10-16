struct VSInput {
    float4 pos;
    float4 tex;
    float4 jac;
    float4 band;
    float4 col;
};

struct SlugUniformBuffer {
    float4 viewport;
};
[[vk::binding(1, 0)]]
ConstantBuffer<SlugUniformBuffer> slugBuffer;

struct UniformBuffer {
    float4x4 model;
    float4x4 view;
    float4x4 proj;
};
ConstantBuffer<UniformBuffer> ubo;

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

float4 SlugDilate(float4 pos, float4 tex, float4 jac, float4 m0, float4 m1, float4 m3, float2 dim)
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

    float2 vpos = pos.xy + d;
    return (float4(tex.x + dot(d, jac.xy), tex.y + dot(d, jac.zw), vpos));
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

    float4x4 matrax = mul(ubo.model,mul(ubo.view,ubo.proj));
    
    // Apply dynamic dilation to vertex position. Returns new em-space sample position.

    float4 dilate = SlugDilate(
        input.pos, 
        input.tex, 
        input.jac, 
        matrax[3], 
        matrax[1], 
        matrax[0],
        slugBuffer.viewport.xy
    );

    vresult.texCoord = dilate.xy;
    p = dilate.zw;

    // Apply MVP matrix to dilated vertex position.
    // vresult.sv_position.x = p.x * matrax[0].x + p.y * matrax[0].y + matrax[0].w;
    // vresult.sv_position.y = p.x * matrax[1].x + p.y * matrax[1].y + matrax[1].w;
    // vresult.sv_position.z = p.x * matrax[2].x + p.y * matrax[2].y + matrax[2].w;
    // vresult.sv_position.w = p.x * matrax[3].x + p.y * matrax[3].y + matrax[3].w;
    // vresult.sv_position = matrax[0];
    // vresult.sv_position = mul()
    vresult.sv_position = mul(ubo.proj, mul(ubo.view, mul(ubo.model, float4(input.pos.x,0, input.pos.y,1))));
    

    // Unpack or pass through remaining vertex data.

    SlugUnpack(input.tex, input.band, vresult.banding, vresult.glyph);

    vresult.color = input.col;
    return vresult;
}