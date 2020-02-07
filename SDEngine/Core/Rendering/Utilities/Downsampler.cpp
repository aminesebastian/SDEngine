#include "Downsampler.h"
#include "Core/Objects/CoreTypes/RenderTarget.h"
#include "Core/Engine/Engine.h"



Downsampler::Downsampler() {
	DownsamplingShader = new Shader("Res/Shaders/PostProcessing/Downsample", false);
}
Downsampler::~Downsampler() {
}

void Downsampler::Downsample(float DownsampleFactor, RenderTarget* Input, RenderTarget* Output) {
	vec2 SourceSize = Input->GetRenderTargetResolution();
	vec2 DestSize   = Output->GetRenderTargetResolution();

	Input->BindForReading();
	Output->BindForWriting();

	float downsampleDivisor  = DownsampleFactor * DownsampleFactor;
	vec2 downsampledDestSize = DestSize / downsampleDivisor;

	glBlitFramebuffer(0, 0,  (GLint)SourceSize.x,  (GLint)SourceSize.y, 0, 0,  (GLint)downsampledDestSize.x,  (GLint)downsampledDestSize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}
void Downsampler::RecompileShaders() {
	DownsamplingShader->RecompileShader();
}
