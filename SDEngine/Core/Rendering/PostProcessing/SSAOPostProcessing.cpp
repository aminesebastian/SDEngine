#include "SSAOPostProcessing.h"
#include "Core/Engine/Engine.h"
#include "Core/Utilities/Math/MathLibrary.h"

SSAOPostProcessing::SSAOPostProcessing(RenderViewport* OwningViewport) : PostProcessingLayer("Screen Space Ambient Occlusion", OwningViewport) {
	SSAOShader = new Shader("Res/Shaders/PostProcessing/SSAO", false);

	OcclusionBuffer = new RenderTarget(OwningViewport->GetOwningWindow()->GetDimensions());
	OcclusionBuffer->AddTextureIndex(new FRenderTargetTextureEntry("SSAO", GL_RG8, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RG, GL_FLOAT));
	OcclusionBuffer->FinalizeRenderTarget();

	Power        = 5.0f;
	Bias         = 0.015f;
	Radius       = 5.0f;
	NoiseSize  = 4;
	KernelSize = 16;
	BlurRadius   = 2;

	GenerateNoise();
	GenerateKernel();
}
SSAOPostProcessing::~SSAOPostProcessing() {
	delete OcclusionBuffer;
	glDeleteTextures(1, &NoiseTexture);
}

void SSAOPostProcessing::RenderLayer(const DefferedCompositor* Compositor, const Camera* RenderCamera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) {
	RenderOcclusion(Compositor, RenderCamera, ReadBuffer, OcclusionBuffer);
	Blur(Compositor, RenderCamera, PreviousOutput, OutputBuffer);
}
void SSAOPostProcessing::RenderOcclusion(const DefferedCompositor* Compositor, const Camera* RenderCamera, GBuffer* GBufferIn, RenderTarget* OutputBuffer) {
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

	for (int i = 0; i < KernelSize; i++) {
		SSAOShader->SetShaderVector3("Samples[" + std::to_string(i) + "]", SampleKernel[i]);
	}
	mat4 projection = RenderCamera->GetProjectionMatrix();
	mat4 viewMatrix = RenderCamera->GetViewMatrix();

	SSAOShader->SetShaderMatrix4("PROJECTION_MATRIX", projection);

	SSAOShader->SetShaderMatrix4("VIEW_MATRIX", viewMatrix);
	SSAOShader->SetShaderMatrix4("TRANS_INV_VIEW_MATRIX", glm::transpose(glm::inverse(viewMatrix)));

	SSAOShader->SetShaderInteger("PASS", 0);

	SSAOShader->SetShaderInteger("SAMPLE_COUNT", KernelSize);
	SSAOShader->SetShaderVector2("NOISE_SCALE", NoiseScale);

	SSAOShader->SetShaderInteger("BLUR_RADIUS", BlurRadius);

	SSAOShader->SetShaderFloat("BIAS", Bias);
	SSAOShader->SetShaderFloat("POWER", Power);
	SSAOShader->SetShaderFloat("RADIUS", Radius);

	Compositor->DrawScreenQuad();
}
void SSAOPostProcessing::Blur(const DefferedCompositor* Compositor, const Camera* RenderCamera, RenderTarget* ReadBuffer, RenderTarget* OutputBuffer) {
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
	for (int i = 0; i < KernelSize; i++) {
		glm::vec3 sample(
			MathLibrary::RandRange<float>(-1.0f, 1.0f),
			MathLibrary::RandRange<float>(-1.0f, 1.0f),
			MathLibrary::RandRange<float>(0.0f, 1.0f)
		);
		sample = glm::normalize(sample);
		sample *= MathLibrary::RandRange(0.0f, 1.0f);
		scale = (float)(i / KernelSize);
		scale = MathLibrary::Lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;

		SampleKernel.Add(sample);
	}
}
void SSAOPostProcessing::GenerateNoise() {
	glDeleteTextures(1, &NoiseTexture);

	for (int i = 0; i < NoiseSize * NoiseSize; ++i) {
		Noise.Add(vec3(
			MathLibrary::RandRange<float>(-1.0f, 1.0f),
			MathLibrary::RandRange<float>(-1.0f, 1.0f),
			0.0f
		));
	}

	NoiseScale = vec2(GetOwningViewport()->GetOwningWindow()->GetDimensions().x / NoiseSize, GetOwningViewport()->GetOwningWindow()->GetDimensions().y / NoiseSize);

	glGenTextures(1, &NoiseTexture);
	glBindTexture(GL_TEXTURE_2D, NoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, NoiseSize, NoiseSize, 0, GL_RGB, GL_FLOAT, &Noise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
void SSAOPostProcessing::RecompileShaders() {
	SSAOShader->RecompileShader();
}

void SSAOPostProcessing::SetBias(const float& Bias) {
	this->Bias = Bias;
}
const float& SSAOPostProcessing::GetBias() const {
	return Bias;
}

void SSAOPostProcessing::SetPower(const float& Power) {
	this->Power = Power;
}
const float& SSAOPostProcessing::GetPower() const {
	return Power;
}

void SSAOPostProcessing::SetRadius(const float& Radius) {
	this->Radius = Radius;
}
const float& SSAOPostProcessing::GetRadius() const {
	return Radius;
}
void SSAOPostProcessing::SetSampleCount(const int& Samples) {
	if (Samples != KernelSize) {
		KernelSize = Samples;
		GenerateKernel();
	}
}
const int& SSAOPostProcessing::GetSampleCount() const {
	return KernelSize;
}
void SSAOPostProcessing::SetNoiseSize(const int& Size) {
	if (Size != NoiseSize) {
		NoiseSize = Size;
		GenerateNoise();
	}
}
const int& SSAOPostProcessing::GetNoiseSize() const {
	return NoiseSize;
}
void SSAOPostProcessing::SetBlurRadius(const int& Radius) {
	BlurRadius = Radius;
}
const int& SSAOPostProcessing::GetBlurRadius() const {
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