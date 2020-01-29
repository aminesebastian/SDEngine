#include "AssetMetadata.h"

AssetMetadata::AssetMetadata(DeserializationStream& Stream) {
	Version = -1;
	ParseMetadataValues(Stream);
}
AssetMetadata::AssetMetadata(TString AssetType, int32 VersionNumber, SerializationStream& Stream) {
	Version = VersionNumber;
	this->AssetType = AssetType;
	PopulateMetadataValues(Stream);
}
AssetMetadata::~AssetMetadata() {

}
bool AssetMetadata::ParseMetadataValues(DeserializationStream& Stream) {
	Stream.DeserializeInteger32(Version); // Serialize the version number.
	Stream.DeserializeString(AssetType); // Get Asset Type
	return true;
}	
bool AssetMetadata::PopulateMetadataValues(SerializationStream& Stream) {
	Stream.SerializeInteger32(Version); // Serialize the version number.
	Stream.SerializeString(AssetType); // Serialize Asset Type
	return true;
}

const int32& AssetMetadata::GetVersion() const {
	return Version;
}
const TString& AssetMetadata::GetAssetType() const {
	return AssetType;
}