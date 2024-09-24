cbuffer ColorBuffer : register(b5)
{
	float4 gColor;
}

struct VertexOut
{
	float4 PosWorld : SV_POSITION;
	float4 Color : COLOR;
};



struct PS_OUTPUT
{
    float4 ObjectMask : SV_Target0;
    float4 RimLightMask : SV_Target1;
};



PS_OUTPUT main(VertexOut input) : SV_TARGET
{
	
    PS_OUTPUT output;
	if(gColor.x > 0)
    {
        output.ObjectMask = gColor;
    }
    output.ObjectMask = 0;
    
    output.RimLightMask = gColor + 10;
	
	//return gColor;
        return output;
    }
