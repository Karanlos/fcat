[[vk::binding(0, 0)]]
RWTexture2D<float4> destTex : register(u0);

[numthreads(16, 16, 1)]
void main(uint3 gl_GlobalInvocationID : SV_DispatchThreadID)
{
    destTex[gl_GlobalInvocationID.xy] = float4(1.0);
}
