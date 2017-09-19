#include "SSAOPostProcessing.h"
#include "Engine.h"
#include "SMath.h"

SSAOPostProcessing::SSAOPostProcessing() {
	S_SSAOShader = new Shader("Res/Shaders/PostProcessing/SSAO", false);

	S_SSAOBuffer = new FrontBufferObject();
	S_SSAOBuffer->AddTextureIndex("ssaoRough");
	S_SSAOBuffer->Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	GenerateNoise();
	GenerateKernel();
}
SSAOPostProcessing::~SSAOPostProcessing() {}

void SSAOPostProcessing::RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer) {
	RenderOcclusion(Compositor, Camera, ReadBuffer, OutputBuffer);
	//Blur(Compositor, Camera, ReadBuffer, OutputBuffer);
}
void SSAOPostProcessing::RenderOcclusion(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer) {
	ReadBuffer->BindForReading();
	OutputBuffer->BindForWriting();
	//S_SSAOBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_SSAOShader->Bind();
	ReadBuffer->BindTextures(S_SSAOShader, true);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + 9);
	glBindTexture(GL_TEXTURE_2D, S_NoiseTexture);
	glUniform1i(glGetUniformLocation(S_SSAOShader->GetProgram(), "noiseTexture"), 9);

	for (int i = 0; i < S_KernelSize; i++) {
		S_SSAOShader->SetShaderVector3("Samples[" + std::to_string(i) + "]", S_Kernel[i]);
	}
	mat4 projection = Camera->GetProjectionMatrix();
	S_SSAOShader->SetShaderMatrix4("PROJECTION_MATRIX", projection);

	mat4 viewMatrix = Camera->GetViewMatrix();
	S_SSAOShader->SetShaderMatrix4("VIEW_MATRIX", viewMatrix);

	mat4 P = Camera->GetProjectionMatrix();
	P = glm::inverse(P);
	S_SSAOShader->SetShaderMatrix4("INV_PROJECTION_MATRIX", P);

	mat4 V = Camera->GetViewMatrix();
	V = glm::inverse(V);
	S_SSAOShader->SetShaderMatrix4("INV_VIEW_MATRIX", V);

	float thfov = Camera->GetThetaFOV();
	S_SSAOShader->SetShaderFloat("THETA_FOV", thfov);

	float aspect = Camera->GetAspect();
	S_SSAOShader->SetShaderFloat("ASPECT", aspect);

	S_SSAOShader->Update(Transform(), Camera);
	
	S_SSAOShader->SetShaderInteger("PASS", 0);

	Compositor->DrawScreenQuad();
}
void SSAOPostProcessing::Blur(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer) {
	ReadBuffer->BindForReading();
	S_SSAOBuffer->BindForReading();
	OutputBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_SSAOShader->Bind();
	ReadBuffer->BindTextures(S_SSAOShader, true);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + 9);
	glBindTexture(GL_TEXTURE_2D, S_SSAOBuffer->GetTexture(0));
	glUniform1i(glGetUniformLocation(S_SSAOShader->GetProgram(), S_SSAOBuffer->GetTextureName(0).c_str()), 9);

	S_SSAOShader->SetShaderInteger("PASS", 1);

	Compositor->DrawScreenQuad();
}

void SSAOPostProcessing::GenerateKernel() {
	for (int i = 0; i < S_KernelSize; i++) {
		glm::vec3 sample(
			SMath::RandRange<float>(-1.0f, 1.0f), 
			SMath::RandRange<float>(-1.0f, 1.0f), 
			SMath::RandRange<float>(0.0f, 1.0f)
		);
		sample = glm::normalize(sample);
		sample *= SMath::RandRange<float>(0.0f, 1.0f);
		float scale = float(i) / 64.0;

		scale = SMath::Lerp<float>(0.1f, 1.0f, scale * scale);
		sample *= scale;
		S_Kernel.push_back(sample);
	}
}
void SSAOPostProcessing::GenerateNoise() {
	for (int i = 0; i < S_NoiseSize; ++i) {
		S_Noise.push_back(vec3(
			SMath::RandRange<float>(-1.0f, 1.0f),
			SMath::RandRange<float>(-1.0f, 1.0f),
			0.0f
		));
		glm::normalize(S_Noise[i]);
	}
	glGenTextures(1, &S_NoiseTexture);
	glBindTexture(GL_TEXTURE_2D, S_NoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &S_Noise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
void SSAOPostProcessing::RecompileShaders() {
	S_SSAOShader->RecompileShader();
}