#include "Downsampler.h"
#include "Rendering/RenderTarget.h"
#include "Engine/Engine.h"



Downsampler::Downsampler() {

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
