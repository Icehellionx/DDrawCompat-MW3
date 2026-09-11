sampler2D s_texture : register(s0);

// xy = one source texel; zw unused.
float4 g_texelSize : register(c200);

float4 main(float2 texCoord : TEXCOORD0) : COLOR0
{
	float4 color = tex2D(s_texture, texCoord);
	const float3 lumaWeights = float3(0.2126f, 0.7152f, 0.0722f);
	float centerLuma = dot(color.rgb, lumaWeights);
	float maxChannel = max(color.r, max(color.g, color.b));
	float minChannel = min(color.r, min(color.g, color.b));
	float chroma = maxChannel - minChannel;

	float3 right = tex2D(s_texture, texCoord + float2(g_texelSize.x, 0)).rgb;
	float3 left = tex2D(s_texture, texCoord - float2(g_texelSize.x, 0)).rgb;
	float3 down = tex2D(s_texture, texCoord + float2(0, g_texelSize.y)).rgb;
	float3 up = tex2D(s_texture, texCoord - float2(0, g_texelSize.y)).rgb;

	float4 neighborLuma = float4(dot(right, lumaWeights), dot(left, lumaWeights),
		dot(down, lumaWeights), dot(up, lumaWeights));
	float4 weights = saturate(1.0f - abs(neighborLuma - centerLuma) * (255.0f / 12.0f));
	float weightSum = 1.0f + dot(weights, float4(1, 1, 1, 1));
	float smoothLuma = (centerLuma + dot(weights, neighborLuma)) / weightSum;
	float3 smoothRgb = (color.rgb + right * weights.x + left * weights.y +
		down * weights.z + up * weights.w) / weightSum;

	// Smooth only chroma in dark portions of INTRO.AVI. The center pixel's
	// luminance is retained exactly, so this cannot crush clothing or shadow
	// contours into black.
	float3 centerChroma = color.rgb - centerLuma;
	float3 localChroma = smoothRgb - dot(smoothRgb, lumaWeights);
	float darkWeight = 1.0f - smoothstep(24.0f / 255.0f, 48.0f / 255.0f, centerLuma);
	float chromaWeight = smoothstep(4.0f / 255.0f, 10.0f / 255.0f, chroma);
	float3 denoised = centerLuma + lerp(centerChroma, localChroma, darkWeight * chromaWeight * 0.80f);

	// The legacy path turns source Y=16/U=128/V=128 black into a repeating
	// 0/8 red-blue signature. Neutralize that exact envelope without changing
	// its locally estimated brightness.
	float signature = (1.0f - step(8.5f / 255.0f, maxChannel)) *
		step(7.5f / 255.0f, chroma);
	float3 neutralSignature = float3(smoothLuma, smoothLuma, smoothLuma);
	return float4(lerp(denoised, neutralSignature, signature), color.a);
}
