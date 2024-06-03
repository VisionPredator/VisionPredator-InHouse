
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};



float3 main(PS_INPUT input) : SV_TARGET
{
    return input.color;
    //return float4(0.f, 1.0f, 0.f, 0.f);
}