////////////////////////////////////////////////////////////////////////////////
// Filename: shader.ps
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer PixelBuffer
{
	float4 pixelColor;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader(PixelInputType input) : SV_TARGET
{
    float4 color;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    color = shaderTexture.Sample(SampleType, input.tex);

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    if(color.r==1.0f && color.g==0.0f && color.b==1.0f)
	{
		color.a = 0.0f;
	}
	else
	{
		color.a = 1.0f;
		color = color*pixelColor;
	}

    return color;
}