#include "SAsset.h"
#include "AssetManager.h"
#include "EngineStatics.h"

SAsset::SAsset(TString FilePath) {
	S_FilePath = FilePath;
	ifstream file(FilePath);
	TString curr = "";
	S_Data = *new SArray<TString>();
	while(getline(file, curr)) {
		S_Data.Add(curr);
	}
	file.close();
}
SAsset::~SAsset() {}

Material* SAsset::GetAsMaterial() {
	Material* newMaterial;
	SArray<TString> texturePaths;
	AssetManager* manager = Engine::GetInstance()->GetAssetManager();

	/*Shader Stage*/
	bool useDefaultShader = true;
	for (int i = 0; i < S_Data.Count(); i++) {
		TString currLine = S_Data[i];
		if (getFucntionName(currLine) == "Shader") {
			newMaterial = new Material(getParameters(currLine)[0]);
			useDefaultShader = false;
			break;
		}
	}

	if(useDefaultShader) {
		newMaterial = new Material(EngineStatics::GetDefaultGeometryPassShader());
	}

	if(newMaterial == nullptr) {
		return nullptr;
	}

	/*Texture Stage*/
	for(int i=0; i<S_Data.Count(); i++) {
		TString currLine = S_Data[i];
		if(getFucntionName(currLine) == "Texture") {
			texturePaths.Add(getParameters(currLine)[0]);
		}
	}
	/*Load textures as assets if needed*/
	for (int i = 0; i < texturePaths.Count(); i++) {
		manager->GetAsset(texturePaths[i]);
	}

	/*Parameters*/
	for (int i = 0; i < S_Data.Count(); i++) {
		TString currLine = S_Data[i];
		if (getFucntionName(currLine) == "TextureParam") {
			int index = atoi(getParameters(currLine)[0].c_str());
			Texture2D* tex = new Texture2D(texturePaths[index]);
			newMaterial->SetTextureParameter(getParameters(currLine)[1], tex);
		}
	}
	return newMaterial;
}
StaticMesh* SAsset::GetAsStaticMesh(TString StaticMeshName) {
	StaticMesh* newMesh = nullptr;
	Material* newMaterial = nullptr;

	for (int i = 0; i < S_Data.Count(); i++) {
		TString currLine = S_Data[i];
		if (getFucntionName(currLine) == "Mesh") {
			newMesh = new StaticMesh(StaticMeshName, getParameters(currLine)[0]);
		}
	}

	if (!newMesh) {
		return nullptr;
	}

	int materialIndex = 0;
	for (int i = 0; i < S_Data.Count(); i++) {
		TString currLine = S_Data[i];
		if (getFucntionName(currLine) == "Material") {
			newMaterial = Engine::GetInstance()->GetAssetManager()->GetAsset(getParameters(currLine)[0])->GetAsMaterial();
			newMesh->GetSubMeshes()[materialIndex]->SubMeshMaterial = newMaterial;
			materialIndex++;
		}
	}

	return newMesh;
}

TString SAsset::getFucntionName(TString Line) {
	TString temp = "";
	for(int i=0; i<Line.length(); i++) {
		if(Line[i] != '(') {
			temp += Line[i];
		}else{
			break;
		}
	}
	return temp;
}
SArray<TString> SAsset::getParameters(TString Line) {
	SArray<TString> parameters;
	TString curr = "";
	int firstSkip = (int)getFucntionName(Line).length() + 1;
	TString parameterString = Line.substr(firstSkip, Line.length() - firstSkip);
	for (int i = 0; i < parameterString.length(); i++) {
		if(curr == "" && parameterString[i] == ' ') { //Skip leading spaces
			continue;
		}
		if(parameterString[i] == ',') {
			parameters.Add(curr);
			curr = "";
		}else if(parameterString[i] == ')') {
			parameters.Add(curr);
			curr = "";
			break;
		}else{
			curr += parameterString[i];
		}
	}
	return parameters;
}

