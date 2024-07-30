#include "Common.hlsli"
    



float4 main(VS_OUTPUT input) : SV_TARGET
{    
    float d = input.pos.z / input.pos.w;
    d *= 10;
        
    return input.color;
    //return float4(0.f, 1.0f, 0.f, 0.f);
}