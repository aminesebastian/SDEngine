#pragma once
#include "Actor.h"
#include "Entities/StaticMesh.h"

class EyeActor : public Actor {

public:
	EyeActor(TString Name, const Transform SpawnTransform);
	~EyeActor();

	virtual void Draw(Camera* Camera, bool bCallerProvidesShader = false) override;

private:
	StaticMesh* S_InnerEye;
	StaticMesh* S_OuterEye;

	Material*	S_InnerEyeMaterial;
	Material*	S_OuterEyeMaterial;

	Texture2D* S_InnerEyeAlbedo;
	Texture2D*	S_InnerEyeRMAO;
	Texture2D*	S_InnerEyeNormal;
};

