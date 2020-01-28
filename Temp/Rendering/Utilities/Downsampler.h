#pragma once
#include "Engine/Engine.h"

class Shader;
class RenderTarget;

class Downsampler {
public:
	Downsampler();
	~Downsampler();

	void Downsample(float DownsampleFactor, RenderTarget* Input, RenderTarget* Output);

};

