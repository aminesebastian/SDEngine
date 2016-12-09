#include "Light.h"

Light::~Light() {}

void Light::Tick(float DeltaTime) {

}
void Light::Draw(Shader& shader) {
	if (S_Probe != NULL) {
//		S_Probe->Draw(shader);
	}
}
