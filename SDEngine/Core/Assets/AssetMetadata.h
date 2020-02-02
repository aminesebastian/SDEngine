#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Utilities/Serialization/DeserializationStream.h"
#include "Core/Utilities/Serialization/SerializationStream.h"

class AssetMetadata {
public:
	/**
	 * Deserializes the asset metadata into this object.
	 * Ensure that the version number has already been deserialized, otherwise this process will fail.
	 * 
	 * @param VersioNumber The version number that was deserialized from the asset data.
	 * @param Stream The stream to deserialize the data from.
	 */
	AssetMetadata(DeserializationStream& Stream);
	/**
	 * Serializes the asset metadata to the stream.
	 * Ensure that the stream is at the head currently.
	 * 
	 * @param AssetType The type of the Asset.
	 * @param Stream The stream to write the data to.
	 */
	AssetMetadata(TString AssetType, int32 VersionNumber, SerializationStream& Stream);
	/**
	 * Deleted copy constructor.
	 */
	AssetMetadata(AssetMetadata&) = delete;
	virtual ~AssetMetadata();

	/**
	 * Gets the version of the asset this metadata is associated with.
	 */
	const int32& GetVersion() const;
	/**
	 * Gets the type of the asset this metadata is associated with.
	 */
	const TString& GetAssetType() const;

protected:
	virtual bool ParseMetadataValues(DeserializationStream& Stream);
	virtual bool PopulateMetadataValues(SerializationStream& Stream);
private:
	int32 Version;
	TString AssetType;
};

