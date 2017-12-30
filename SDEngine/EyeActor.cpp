#include "EyeActor.h"
#include "Engine.h"



EyeActor::EyeActor(TString Name, const Transform SpawnTransform) :
	Actor(Name, SpawnTransform) {
	S_InnerEyeAlbedo = new Texture2D("res/Eye/T_EyeBaseColor.tga");
	S_InnerEyeRMAO	 = new Texture2D("res/Eye/T_EyeRMAO.tga");
	S_InnerEyeNormal = new Texture2D("res/Eye/T_EyeNormal.tga");

	S_InnerEyeMaterial = new Material("./Res/Shaders/TestEyeShader");
	S_InnerEyeMaterial->SetTextureParameter("Albedo", S_InnerEyeAlbedo);
	S_InnerEyeMaterial->SetTextureParameter("RMAO", S_InnerEyeRMAO);
	S_InnerEyeMaterial->SetTextureParameter("Normal", S_InnerEyeNormal);
	S_InnerEye = new StaticMesh("Inner Eye", SpawnTransform, S_InnerEyeMaterial, "./res/Eye/Eye.fbx");

	S_OuterEyeMaterial = new Material("./Res/Shaders/TranslucentShader");
	S_OuterEyeMaterial->SetShaderModel(EShaderModel::TRANSLUCENT);
	S_OuterEyeMaterial->SetVec3Parameter("Albedo", vec3(1.0, 1.0, 1.0));
	S_OuterEyeMaterial->SetScalarParameter("Opacity", 0.2f);
	S_OuterEye = new StaticMesh("Outer Eye", SpawnTransform, S_OuterEyeMaterial, "./res/Eye/EyeClear.fbx");
}
EyeActor::~EyeActor() {
}

void EyeActor::Draw(Camera* Camera) {
	S_OuterEye->Draw(Camera);
	S_InnerEye->Draw(Camera);
}
