
struct VertexOutput {
    float4 sv_position : SV_Position;
    float4 color;
    float2 texCoord;
};

Sampler2D texture;


[shader("fragment")]
float4 fragMain(VertexOutput inVert) : SV_Target {
    return float4(1.0,0.0,0.0,1.0);
}