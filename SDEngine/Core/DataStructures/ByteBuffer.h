#pragma once
#include "Core/DataStructures/Array.h"
#include "Utilities/TypeDefenitions.h"
#include "ZLIB/zlib.h"

class ByteBuffer {
public:
	ByteBuffer();
	ByteBuffer(int32 PreallocateSize);
	ByteBuffer(const SArray<char>& Data, bool bShouldDecompressData);
	~ByteBuffer();
	const SArray<char>& GetData() const;
	void Add(char Data);
	void Add(TString Data);
	void AddLine(char Data);
	void AddLine(TString Data);
	int32 Size() const; 
	TString GetCompressedString() const;
	bool WriteToCompressedFile(TString FilePath);


	char* begin() {
		return &Buffer[0];
	}
	char* end() {
		return &Buffer[Buffer.Count()-1];
	}

	char& operator[](int32 Index) {
		return Buffer[Index];
	}


	static ByteBuffer* FromCompressedFile(TString FilePath);
	static ByteBuffer* FromFile(TString FilePath);
private:
	bool DecompressFromCompressedString(const SArray<char>& CompressedData);
	SArray<char> Buffer;
};

