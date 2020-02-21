#pragma once
#include "Core/Rendering/OpenGL/GPUVertexBufferArray.h"
#include "Core/Pictorum/PictorumDataTypes.h"
#include "Core/Objects/EngineObject.h"

struct FBoxDrawInstruction {
	Vector2D Location;
	Vector2D Size;
	float EdgeSoftness;
	FColor BackgroundColor;
	FBorderRadius BorderRadius;
	float BorderThickness;
	float BorderSoftness;
	FColor BorderColor;

	FBoxDrawInstruction() {
		Location = Vector2D(0.0f, 0.0f);
		Size = Vector2D(0.0f, 0.0f);
		EdgeSoftness = 1.0f;
		BackgroundColor = FColor(0.5f, 0.5f, 0.5f);

		BorderThickness   = 1.0f;
		BorderSoftness   = 1.0f;
		BorderColor = FColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
};
struct FImageDrawInstruction {
	FColor Tint;
	Vector2D Location;
	Vector2D Size;
	class Texture2D* Texture;

	FImageDrawInstruction() {
		Tint = FColor(1.0f, 1.0f, 1.0f);
		Texture = nullptr;
		Location = Vector2D(0.0f, 0.0f);
		Size = Vector2D(0.0f, 0.0f);
	}
};

class PictorumShapeDrawer  : public EngineObject {
public:
	PictorumShapeDrawer(const TString& Name);
	virtual ~PictorumShapeDrawer();

	void DrawQuad(const FRenderGeometry& Geometry, const FBoxDrawInstruction& Instruction) const;
	void DrawImage(const FRenderGeometry& Geometry, const FImageDrawInstruction& Instruction) const;
private:
	GPUVertexBufferArray* QuadVertexBuffer;
};

