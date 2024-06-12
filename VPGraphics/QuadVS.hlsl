#include "Common.hlsli"

VS_OUTPUT main(Quad input)
{
    VS_OUTPUT test;
        
    test.pos = input.pos;
    //test.pos = mul(input.pos, gWorldViewProj);
    //test.color = input.color;
    //test.normal = input.normal;
    //test.tangent = input.tangent;
    //test.bitangent = input.bitangent, test.pos;
    test.tex = input.tex;
    
    return test;
}
