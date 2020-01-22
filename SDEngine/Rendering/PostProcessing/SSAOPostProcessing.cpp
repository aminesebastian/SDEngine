#include "SSAOPostProcessing.h"
#include "Engine/Engine.h"
#include "Utilities/Math/MathLibrary.h"

SSAOPostProcessing::SSAOPostProcessing(vec2 FinalOutputDimensions) : PostProcessingLayer("Screen Space Ambient Occlusion", FinalOutputDimensions) {
	SSAOShader = new Shader("Res/Shaders/PostProcessing/SSAO", false);

	OcclusionBuffer = new RenderTarget(FinalOutputDimensions);
	OcclusionBuffer->AddTextureIndex(new FRenderTargetTextureEntry("SSAO", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	OcclusionBuffer->FinalizeRenderTarget();

	Power        = 5.0f;
	Bias         = 0.015f;
	Radius       = 5.0f;
	S_NoiseSize  = 4;
	S_KernelSize = 16;
	BlurRadius   = 2;

	GenerateNoise();
	GenerateKernel();
}
SSAOPostProcessing::~SSAOPostProcessing() {}

void SSAOPostProcessing::RenderLayer(DefferedCompositor* Compositor, Camera* Camera, GBuffer* GBufferIn, RenderTarget* CurrentLitFrame, RenderTarget* OutputBuffer) {
	RenderOcclusion(Compositor, Camera, GBufferIn, OcclusionBuffer);
	Blur(Compositor, Camera, CurrentLitFrame, OutputBuffer);
}
void SSAOPostProcessing::RenderOcclusion(DefferedCompositor* Compositor, Camera* Camera, GBuffer* GBufferIn, RenderTarget* OutputBuffer) {
	GBufferIn->BindForReading();
	OutputBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SSAOShader->Bind();

	GBufferIn->BindTexture(SSAOShader, GBuffer::GBUFFER_TEXTURE_TYPE_POSITION, 0, "WorldPosition");
	GBufferIn->BindTexture(SSAOShader, GBuffer::GBUFFER_TEXTURE_TYPE_ALBEDO, 1, "Albedo");
	GBufferIn->BindTexture(SSAOShader, GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL, 2, "WorldNormal");

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, NoiseTexture);
	glUniform1i(glGetUniformLocation(SSAOShader->GetProgram(), "noiseTexture"), 3);

	for (int i = 0; i < S_KernelSize; i++) {
		SSAOShader->SetShaderVector3("Samples[" + std::to_string(i) + "]", SampleKernel[i]);
	}
	mat4 projection = Camera->GetProjectionMatrix();
	mat4 viewMatrix = Camera->GetViewMatrix();

	SSAOShader->SetShaderMatrix4("PROJECTION_MATRIX", projection);

	SSAOShader->SetShaderMatrix4("VIEW_MATRIX", viewMatrix);
	SSAOShader->SetShaderMatrix4("TRANS_INV_VIEW_MATRIX", glm::transpose(glm::inverse(viewMatrix)));

	SSAOShader->SetShaderInteger("PASS", 0);

	SSAOShader->SetShaderInteger("SAMPLE_COUNT", S_KernelSize);
	SSAOShader->SetShaderVector2("NOISE_SCALE", NoiseScale);

	SSAOShader->SetShaderInteger("BLUR_RADIUS", BlurRadius);

	SSAOShader->SetShaderFloat("BIAS", Bias);
	SSAOShader->SetShaderFloat("POWER", Power);
	SSAOShader->SetShaderFloat("RADIUS", Radius);

	Compositor->DrawScreenQuad();
}
void SSAOPostProcessing::Blur(DefferedCompositor* Compositor, Camera* Camera, RenderTarget* ReadBuffer, RenderTarget* OutputBuffer) {
	ReadBuffer->BindForReading();
	OcclusionBuffer->BindForReading();
	OutputBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SSAOShader->Bind();

	ReadBuffer->BindTexture(SSAOShader, 0, 0, "Input");
	OcclusionBuffer->BindTexture(SSAOShader, 0, 1, "SSAO");

SSAOShader->SetShaderInteger("PASS", 1);

Compositor->DrawScreenQuad();
}

void SSAOPostProcessing::GenerateKernel() {
	float scale = 0.0f;
	for (int i = 0; i < S_KernelSize; i++) {
		glm::vec3 sample(
			MathLibrary::RandRange<float>(-1.0f, 1.0f),
			MathLibrary::RandRange<float>(-1.0f, 1.0f),
			MathLibrary::RandRange<float>(0.0f, 1.0f)
		);
		sample = glm::normalize(sample);
		sample *= MathLibrary::RandRange(0.0f, 1.0f);
		scale = (float)(i / S_KernelSize);
		scale = MathLibrary::Lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;

		SampleKernel.push_back(sample);
	}
}
void SSAOPostProcessing::GenerateNoise() {
	glDeleteTextures(1, &NoiseTexture);

	for (int i = 0; i < S_NoiseSize * S_NoiseSize; ++i) {
		Noise.push_back(vec3(
			MathLibrary::RandRange<float>(-1.0f, 1.0f),
			MathLibrary::RandRange<float>(-1.0f, 1.0f),
			0.0f
		));
	}

	NoiseScale = vec2(GetRenderTargetDimensions().x / S_NoiseSize, GetRenderTargetDimensions().y / S_NoiseSize);

	glGenTextures(1, &NoiseTexture);
	glBindTexture(GL_TEXTURE_2D, NoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, S_NoiseSize, S_NoiseSize, 0, GL_RGB, GL_FLOAT, &Noise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
void SSAOPostProcessing::RecompileShaders() {
	SSAOShader->RecompileShader();
}

void SSAOPostProcessing::SetBias(float Bias) {
	this->Bias = Bias;
}
float SSAOPostProcessing::GetBias() {
	return Bias;
}

void SSAOPostProcessing::SetPower(float Power) {
	this->Power = Power;
}
float SSAOPostProcessing::GetPower() {
	return Power;
}

void SSAOPostProcessing::SetRadius(float Radius) {
	this->Radius = Radius;
}
float SSAOPostProcessing::GetRadius() {
	return Radius;
}
void SSAOPostProcessing::SetSampleCount(int Samples) {
	if (Samples != S_KernelSize) {
		S_KernelSize = Samples;
		GenerateKernel();
	}
}
int SSAOPostProcessing::GetSampleCount() {
	return S_KernelSize;
}
void SSAOPostProcessing::SetNoiseSize(int Size) {
	if (Size != S_NoiseSize) {
		S_NoiseSize = Size;
		GenerateNoise();
	}
}
int SSAOPostProcessing::GetNoiseSize() {
	return S_NoiseSize;
}
void SSAOPostProcessing::SetBlurRadius(int Radius) {
	BlurRadius = Radius;
}
int SSAOPostProcessing::GetBlurRadius() {
	return BlurRadius;
}


bool SSAOPostProcessing::PopulatePostProcessingDetailsPanel() {
	ImGui::SliderFloat("Radius", &Radius, 0.0f, 50.0f);
	ImGui::SliderFloat("Power", &Power, 0.0f, 20.0f);
	ImGui::SliderFloat("Bias", &Bias, 0.0f, 0.05f);

	int noiseSize = GetNoiseSize();
	int samples = GetSampleCount();

	if(ImGui::SliderInt("Samples", &samples, 1, 255)) {
		SetSampleCount(samples);
	}
	if (ImGui::SliderInt("Noise Size", &noiseSize, 4, 32)) {
		SetNoiseSize(noiseSize);
	}

	return true;
}