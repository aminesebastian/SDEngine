#include "HeadActor.h"
#include "Core/Assets/AssetManager.h"


HeadActor::HeadActor(const TString& Name) : StaticMeshActor(Name) {
	StaticMesh* head = Engine::Get()->GetAssetManager()->GetAsset<StaticMesh>("./Res/Assets/Head.sasset");
	SetStaticMesh(head);
}
HeadActor::~HeadActor() {

}