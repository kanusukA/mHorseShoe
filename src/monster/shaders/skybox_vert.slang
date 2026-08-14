struct VSInput {
    float3 inPosition;
    float3 inColor;
    float2 inTexCoord;
};

struct VertexOutput {
    float4 sv_position : SV_Position;
    float4 color;
    float2 texCoord;
};

struct UniformBuffer {
    float4x4 model;
    float4x4 view;
    float4x4 proj;
};
ConstantBuffer<UniformBuffer> ubo;

[shader("vertex")]
VertexOutput main(
    VSInput input,    
)
{
    VertexOutput output = (VertexOutput)0;
    float4 mvpPosition = mul(ubo.proj, (mul(ubo.view, mul(ubo.model, float4(input.inPosition, 1.0f)))));
    output.sv_position = mvpPosition;
    output.color = float4(input.inColor, 1.0f);
    output.texCoord = input.inTexCoord;
    
    return output;

}