

struct VSInput {
    float2 inPosition;
    float3 inColor;
};

struct VertexOutput {
    float4 sv_position : SV_Position;
    float4 color;
};

struct UniformBuffer {
    float4x4 model;
    float4x4 view;
    float4x4 proj;
};
ConstantBuffer<UniformBuffer> ubo;


[shader("vertex")]
VertexOutput vertMain(VSInput input){

    VertexOutput output;
    float4 mvpPosition = mul(ubo.proj, (mul(ubo.view, mul(ubo.model, float4(input.inPosition, 0.0f, 1.0f)))));
    output.sv_position = mvpPosition;
    output.color = float4(input.inColor,1.0f);
    return output;

}

[shader("fragment")]
float4 fragMain(VertexOutput inVert) : SV_Target {
    return inVert.color;
}