#pragma once
#include "Engine.h"

class Shader;
class RenderTarget;

class Downsampler {
public:
	Downsampler();
	~Downsampler();

	void Downsample(float DownsampleFactor, RenderTarget* Input, RenderTarget* Output);
	void RecompileShaders();

private:
	Shader* DownsamplingShader;
};

