#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Objects/CoreTypes/Texture2D.h"

class ImageWidget : public PictorumWidget {
public:
	ImageWidget(const TString& Name);
	virtual  ~ImageWidget();
	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;

	virtual void OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& Event) override;
	virtual void OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& Event) override;
	void SetTint(const FColor& NewTint);
	void SetImage(Texture2D* ImageIn);
	void SetSize(const vec2& SizeIn);
private:
	float BorderRadius;
	FColor Tint;
	Texture2D* Image;
	vec2 Size;
	bool bWasMouseDownInWidget;

	GLuint quadVAO;
	GLuint quadVBO;
};

