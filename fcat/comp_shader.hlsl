[[vk::binding(0, 0)]]
RWTexture2D<float4> destTex : register(u0);

[[vk::push_constant]]
cbuffer Index {
	uint index;
};

static float4 clearColors[] = {
    float4( 1.0,          0.1,        1.0, 			1.0 ), //1.  White
    float4( 0.0,          1.0,        0.0,  		1.0 ), //2.  Lime
    float4( 0.0,          0.0,        1.0,  		1.0 ), //3.  Blue
    float4( 1.0,          0.0,        0.0,  		1.0 ), //4.  Red
    float4( 0.0,          0.5,        0.5,  		1.0 ), //5.  Teal
    float4( 0.0,          0.0,        0.5,  		1.0 ), //6.  Navy
    float4( 0.0,          0.5,        0.0,  		1.0 ), //7.  Green
    float4( 0.0,          1.0,        1.0,  		1.0 ), //8.  Aqua
    float4( 0.0,          0.2,        0.13,			1.0 ), //9.  Dark Green
    float4( 0.75,         0.75,       0.75,			1.0 ), //10. Silver
    float4( 0.5,          0.0,        0.5,  		1.0 ), //11. Purple
    float4( 0.5,          0.5,        0.0,  		1.0 ), //12. Olive
    float4( 0.5,          0.5,        0.5,  		1.0 ), //13. Gray
    float4( 1.0,          0.0,        1.0,  		1.0 ), //14. Fuchsia
    float4( 1.0,          1.0,        0.0,  		1.0 ), //15. Yellow
    float4( 1.0,          0.65,       0.0,  		1.0 )  //16. Orange
};

[numthreads(16, 16, 1)]
void main(uint3 gl_GlobalInvocationID : SV_DispatchThreadID)
{
    destTex[gl_GlobalInvocationID.xy] = clearColors[index];
}
