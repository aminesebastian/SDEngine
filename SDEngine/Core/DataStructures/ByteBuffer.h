#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "ZLIB/zlib.h"

/**
 * Helper class to handle the serialization and deserialization of byte data.
 * 
 * @author Amine Hosseini
 */
class ByteBuffer {
public:
	/**
	 * Creates an empty byte buffer.
	 */
	ByteBuffer();
	/**
	 * Creates an empty byte buffer but preallocates memory.
	 */
	ByteBuffer(int32 PreallocateSize);
	/**
	 * Creates a byte buffer from a provided array of bytes. Optionally decompresses.
	 * 
	 * @param Data The byte array data.
	 * @param bShouldDecompressData If true, the data passed in will first be decompressed before being placed in the buffer. Otherwise, data is inserted directly into the buffer.
	 */
	ByteBuffer(const SArray<char>& Data, bool bShouldDecompressData);
	~ByteBuffer();

	/**
	 * Gets the internal data of the buffer.
	 * 
	 * @return A constant reference to the data.
	 */
	const SArray<char>& GetData() const;
	/**
	 * Adds a character to the buffer.
	 * 
	 * @param Data The character to add.
	 */
	void Add(char Data);
	/**
	* Adds a character to the buffer.
	* 
	* @param Data The character to add.
	*/
	void Add(TString Data);
	/**
	 * Gets the current size of the buffer.
	 * 
	 * @return The size of the buffer.
	 */
	int32 Size() const; 
	/* 
	* Compresses and writes this buffer to a file path.
	* 
	* @param FilePath The path of the file to read from.
	* 
	* @return True if the read and parse was successful, false otherwise.
	*/
	bool WriteToCompressedFile(TString FilePath);
	/* 
	 * Populates this buffer from a compressed file path.
	 * 
	 * @param FilePath The path of the file to read from.
	 * 
	 * @return True if the read and parse was successful, false otherwise.
	 */
	bool FromCompressedFile(TString FilePath);
	/* 
	* Populates this buffer from a compressed file path.
	* 
	* @param FilePath The path of the file to read from.
	* 
	* @return True if the read and parse was successful, false otherwise.
	*/
	bool FromFile(TString FilePath);

	char* begin() {
		return &Buffer[0];
	}
	char* end() {
		return &Buffer[Buffer.Count()-1];
	}

	char& operator[](int32 Index) {
		return Buffer[Index];
	}
protected:
	/**
	* Gets the compressed version of this buffer.
	*/
	TString GetCompressedString() const;
private:
	bool DecompressFromCompressedString(const SArray<char>& CompressedData);
	SArray<char> Buffer;
};

