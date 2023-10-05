#include "PostProcess.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t4> gHighIntensityTexture : register(t1);
Texture2D<float32_t4> gHighIntensityBlurTexture : register(t2);
SamplerState gSampler : register(s0);

ConstantBuffer<Bloom> gBloomParameter : register(b0);
ConstantBuffer<LensDistortion> gLensDistortionParameter : register(b1);
ConstantBuffer<Vignette> gVignetteParameter : register(b2);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
	//通常テクスチャ
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
	//高輝度テクスチャ
    float32_t4 highIntensityColor = gHighIntensityTexture.Sample(gSampler, input.texcoord);
	//高輝度ぼかしテクスチャ
    float32_t4 highIntensityBlurColor = gHighIntensityBlurTexture.Sample(gSampler, input.texcoord);

	//レンズディストーション
    if (gLensDistortionParameter.enable == true)
    {
        const float2 uvNormalized = input.texcoord * 2 - 1;
        const float distortionMagnitude = abs(uvNormalized.x * uvNormalized.y);
        const float smoothDistortionMagnitude = pow(distortionMagnitude, gLensDistortionParameter.tightness);
		//const float smoothDistortionMagnitude = 1 - sqrt(1 - pow(distortionMagnitude, gLensDistortionParameter.tightness));
		//const float smoothDistortionMagnitude = pow(sin(1.57079632679f * distortionMagnitude), gLensDistortionParameter.tightness);
        float2 uvDistorted = input.texcoord + uvNormalized * smoothDistortionMagnitude * gLensDistortionParameter.strength;
        if (uvDistorted[0] < 0 || uvDistorted[0] > 1 || uvDistorted[1] < 0 || uvDistorted[1] > 1)
        {
            textureColor = 0.0f;
            highIntensityColor = 0.0f;
            highIntensityBlurColor = 0.0f;
        }
        else
        {
            textureColor = gTexture.Sample(gSampler, uvDistorted);
            highIntensityColor = gHighIntensityTexture.Sample(gSampler, uvDistorted);
            highIntensityBlurColor = gHighIntensityBlurTexture.Sample(gSampler, uvDistorted);
        }
    }


	//Bloom
    if (gBloomParameter.enable == true)
    {
        float4 color = textureColor;
        textureColor = color + highIntensityColor + highIntensityBlurColor;
    }


	//ビネット
    if (gVignetteParameter.enable)
    {
        float2 uv = input.texcoord;
        uv = gVignetteParameter.intensity * uv - gVignetteParameter.intensity / 2;
        textureColor *= 1.0 - dot(uv, uv);
    }

    output.color = textureColor;
    
    return output;
}