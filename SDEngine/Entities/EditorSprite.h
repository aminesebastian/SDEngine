#pragma once
#include "Entities/Entity.h"

class Texture2D;

class EditorSprite : public Entity {
public:
	EditorSprite(TString TexturePath, vec3 Color);
	EditorSprite(Texture2D* Sprite, vec3 Color);
	~EditorSprite();

	virtual void Draw(Camera* Camera, bool bCallerProvidesShader = false) override;
	virtual bool TraceAgainstRay(vec3 Origin, vec3 Direction, vec3& HitPoint, float& Distance, ECollisionChannel Channel = VISIBILITY) override;

	void SetTint(vec3 NewTint);
	vec3 GetTint();

protected:
	EditorSprite();

private:
	Texture2D* Sprite;
	vec3 Tint;
	GLuint SpriteVertexArray;
	GLuint SpriteVertexBuffer;
};

