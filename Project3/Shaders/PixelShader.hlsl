Texture2D tex;
SamplerState samplerthing;

float4 main(float2 tc : TextureCoord) : SV_Target
{
    return tex.Sample(samplerthing, tc);
}