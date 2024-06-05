struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 posWorld : WORLDPOSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 bitangent : BITANGENT;
    float2 tex : TEXCOORD;
};



float4 main(PS_INPUT input) : SV_TARGET
{
    return input.color;
    //return float4(0.f, 1.0f, 0.f, 0.f);
}