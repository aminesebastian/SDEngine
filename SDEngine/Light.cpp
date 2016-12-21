#include "Light.h"

Light::~Light() {}

void Light::BeginPlay() {
	Texture2D defaultAlbedo("./res/T_DefaultAlbedoMap.png", "tex_Normal");
	Texture2D defaultRMAO("./res/T_DefaultRMAOMap.png", "tex_Normal");
	Texture2D defaultNormal("./res/T_DefaultNormalMap.png", "tex_Normal");

	
	Transform spherelightTransform;
	StaticMesh lightProbe(S_World, spherelightTransform, "./res/Sphere.fbx");
	lightProbe.RegisterTexture(&defaultRMAO);
	lightProbe.RegisterTexture(&defaultAlbedo);
	lightProbe.RegisterTexture(&defaultNormal);
	
	S_Probe = &lightProbe;
}
void Light::Tick(float DeltaTime) {

}
void Light::Draw(Shader& shader) {

}
