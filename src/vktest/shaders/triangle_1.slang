

struct VSInput {
    float2 inPosition;
    float3 inColor;
};

struct VertexOutput {
    float4 sv_position : SV_Position;
    float4 color;
};


[shader("vertex")]
VertexOutput vertMain(VSInput input){

    VertexOutput output;
    output.sv_position = float4(input.inPosition, 0, 1);
    output.color = float4(input.inColor,1);
    return output;

}

[shader("fragment")]
float4 fragMain(VertexOutput inVert) : SV_Target {
    return inVert.color;
}