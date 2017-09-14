#include "SSAOPostProcessing.h"
#include "Engine.h"
#include "SMath.h"

SSAOPostProcessing::SSAOPostProcessing() {
	S_SSAOShader = new Shader("Res/Shaders/PostProcessing/SSAO");
	S_LowResBuffer = new FrontBufferObject();
	S_LowResBuffer->AddTextureIndex("ssaoRough");
	S_LowResBuffer->Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	GenerateNoise();
	GenerateKernel();
}
SSAOPostProcessing::~SSAOPostProcessing() {}

void SSAOPostProcessing::RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer) {
	ReadBuffer->BindForReading();
	OutputBuffer->BindForWriting();
	S_LowResBuffer->BindForReading();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_SSAOShader->Bind();
	ReadBuffer->BindTextures(S_SSAOShader);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + 8);
	glBindTexture(GL_TEXTURE_2D, S_NoiseTexture);
	glUniform1i(glGetUniformLocation(S_SSAOShader->GetProgram(), "noiseTexture"), 8);

	for (int i = 0; i < S_KernelSize; i++) {
		S_SSAOShader->SetShaderVector3("Samples[" + std::to_string(i) + "]", S_Kernel[i]);
	}
	S_SSAOShader->SetShaderMatrix4("PROJECTION", Camera->GetProjectionMatrix());
	S_SSAOShader->SetShaderVector3("VIEW_RAY", Camera->GetTransform().GetForwardVector());
	Compositor->DrawScreenQuad();

}
void SSAOPostProcessing::GenerateKernel() {
	for (int i = 0; i < S_KernelSize; i++) {
		S_Kernel.push_back(vec3(
			SMath::FRandRange(-1.0f, 1.0f),
			SMath::FRandRange(-1.0f, 1.0f),
			SMath::FRandRange(0.0f, 1.0f)
		));
		glm::normalize(S_Kernel[i]);
	}
}
void SSAOPostProcessing::GenerateNoise() {
	for (int i = 0; i < S_NoiseSize; ++i) {
		S_Noise.push_back(vec3(
			SMath::FRandRange(-1.0f, 1.0f),
			SMath::FRandRange(-1.0f, 1.0f),
			0.0f
		));
		glm::normalize(S_Noise[i]);
	}
	glGenTextures(1, &S_NoiseTexture);
	glBindTexture(GL_TEXTURE_2D, S_NoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &S_Noise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void SSAOPostProcessing::RecompileShaders() {
	S_SSAOShader->RecompileShader();
}