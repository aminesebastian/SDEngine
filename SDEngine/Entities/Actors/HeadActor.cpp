#include "HeadActor.h"
#include <Engine\SAsset.h>
#include "Engine/AssetManager.h"

HeadActor::HeadActor(const TString& Name) : StaticMeshActor(Name) {
	SAsset* headAsset = Engine::GetInstance()->GetAssetManager()->GetAsset("./Res/Assets/Head.sasset");
	StaticMesh* head = headAsset->GetAsStaticMesh("Head");
	SetStaticMesh(head);
}
HeadActor::~HeadActor() {

}