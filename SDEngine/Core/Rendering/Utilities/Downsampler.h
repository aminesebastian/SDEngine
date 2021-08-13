#pragma once
#include "Core/Engine/Engine.h"

class RenderTarget;

class Downsampler {
public:
	Downsampler();
	~Downsampler();

	void Downsample(float DownsampleFactor, RenderTarget* Input, RenderTarget* Output);
};

