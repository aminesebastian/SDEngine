#include "Prefab.h"



Prefab::Prefab() {
}
Prefab::~Prefab() {
}

void Prefab::Tick(float DeltaTime) {
	for (int i = 0; i < Children.size(); i++) {
		Children[i]->SetTransform(GetTransform());
	}
}
void Prefab::Draw(Shader& shader) {
	for (int i = 0; i < Children.size(); i++) {
		Children[i]->Draw(shader);
	}
}
