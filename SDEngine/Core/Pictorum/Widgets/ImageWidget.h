#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Objects/CoreTypes/Texture2D.h"
#include "Core/Rendering/OpenGL/GPUVertexBufferArray.h"

class ImageWidget : public PictorumWidget {
public:
	ImageWidget(const TString& Name);
	virtual  ~ImageWidget();
	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;

	virtual void OnMouseEnter(const vec2& MousePosition, FUserInterfaceEvent& Event) override;
	virtual void OnMouseExit(const vec2& MousePosition, FUserInterfaceEvent& Event) override;
	void SetTint(const FColor& NewTint);
	void SetImage(Texture2D* ImageIn, const bool& UpdateSize = false);
	void SetSize(const vec2& SizeIn);
private:
	FImageDrawInstruction* DrawInstruction;
	Vector2D Size;
};

