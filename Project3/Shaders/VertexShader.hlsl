struct Out
{
    float2 tc : TextureCoord;
    float4 pos : SV_Position;
};

Out main(float2 pos : Position, float2 tc : TextureCoord)
{
    Out out_;
    out_.pos = float4(pos.x, pos.y, 0.f, 1.f);
    out_.tc = tc;
    return out_;
}