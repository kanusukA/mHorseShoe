
struct VertexOutput {
    float4 sv_position : SV_Position;
};

[shader("fragment")]
float4 fragMain(VertexOutput inVert) : SV_Target
{
    return float4(1.0, 0.0, 0.0, 1.0);
}