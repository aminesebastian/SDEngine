#pragma once

class Camera;

class IRenderable {
public:
	virtual void Draw(Camera* Camera, float DeltaTime) = 0;
};

