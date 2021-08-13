#pragma once
#include "Core/Objects/Entities/Component.h"
#include "Core/Objects/CoreTypes/Texture2D.h"

class BillboardComponent : public Component {
public:
	BillboardComponent(const TString& Name, TString TexturePath, const FColor& Color);
	BillboardComponent(const TString& Name, Texture2D* Sprite, const FColor& Color);
	~BillboardComponent();

	virtual void DrawAdvanced(const Camera* RenderCamera, const EDrawType& DrawType) override;
	virtual bool TraceAgainstRay(const Vector3D& Origin, const Vector3D& Direction, Vector3D& HitPoint, float& Distance, ECollisionChannel Channel = VISIBILITY);

	void SetTint(const FColor& NewTint);
	const FColor& GetTint() const;

protected:
	BillboardComponent(const TString& Name);

private:
	Texture2D* Sprite;
	FColor Tint;
	GLuint SpriteVertexArray;
	GLuint SpriteVertexBuffer;
};

