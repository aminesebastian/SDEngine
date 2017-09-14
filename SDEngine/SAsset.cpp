#include "SAsset.h"
#include "AssetManager.h"

SAsset::SAsset(TString FilePath) {
	S_FilePath = FilePath;
	ifstream file(FilePath);
	TString curr = "";
	S_Data = *new SArray<TString>();
	while(getline(file, curr)) {
		S_Data.push_back(curr);
	}
	file.close();
}
SAsset::~SAsset() {}

Material* SAsset::GetAsMaterial() {
	Material* newMaterial;
	SArray<TString> texturePaths;
	AssetManager* manager = Engine::GetInstance()->GetAssetManager();

	/*Shader Stage*/
	for (int i = 0; i < S_Data.size(); i++) {
		TString currLine = S_Data[i];
		if (getFucntionName(currLine) == "Shader") {
			newMaterial = new Material(getParameters(currLine)[0]);
			break;
		}
	}

	if(newMaterial == nullptr) {
		return nullptr;
	}

	/*Texture Stage*/
	for(int i=0; i<S_Data.size(); i++) {
		TString currLine = S_Data[i];
		if(getFucntionName(currLine) == "Texture") {
			texturePaths.push_back(getParameters(currLine)[0]);
		}
	}
	/*Load textures as assets if needed*/
	for (int i = 0; i < texturePaths.size(); i++) {
		manager->GetAsset(texturePaths[i]);
	}

	/*Parameters*/
	for (int i = 0; i < S_Data.size(); i++) {
		TString currLine = S_Data[i];
		if (getFucntionName(currLine) == "TextureParam") {
			int index = atoi(getParameters(currLine)[0].c_str());
			Texture2D* tex = new Texture2D(texturePaths[index]);
			newMaterial->SetTextureParameter(getParameters(currLine)[1], tex);
		}
	}
	return newMaterial;
}
StaticMesh* SAsset::GetAsStaticMesh() {
	StaticMesh* newMesh;
	Material* newMaterial;
	Transform transform;

	for (int i = 0; i < S_Data.size(); i++) {
		TString currLine = S_Data[i];
		if (getFucntionName(currLine) == "Material") {
			newMaterial = Engine::GetInstance()->GetAssetManager()->GetAsset(getParameters(currLine)[0])->GetAsMaterial();
			break;
		}
	}

	for (int i = 0; i < S_Data.size(); i++) {
		TString currLine = S_Data[i];
		if (getFucntionName(currLine) == "Mesh") {
			newMesh = new StaticMesh(transform, newMaterial, getParameters(currLine)[0]);
			return newMesh;
		}
	}
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
	int firstSkip = getFucntionName(Line).length() + 1;
	TString parameterString = Line.substr(firstSkip, Line.length() - firstSkip);
	for (int i = 0; i < parameterString.length(); i++) {
		if(curr == "" && parameterString[i] == ' ') { //Skip leading spaces
			continue;
		}
		if(parameterString[i] == ',') {
			parameters.push_back(curr);
			curr = "";
		}else if(parameterString[i] == ')') {
			parameters.push_back(curr);
			curr = "";
			break;
		}else{
			curr += parameterString[i];
		}
	}
	return parameters;
}

