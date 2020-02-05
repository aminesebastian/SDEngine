#include "ByteBuffer.h"
#include "Utilities/Logger.h"

ByteBuffer::ByteBuffer() {

}
ByteBuffer::ByteBuffer(int32 PreallocateSize) : ByteBuffer() {
	Buffer.PreAllocate(PreallocateSize);
}
ByteBuffer::ByteBuffer(const SArray<char>& Data, bool bShouldDecompressData) : ByteBuffer() {
	if (bShouldDecompressData) {
		DecompressFromCompressedString(Data);
	} else {
		Buffer.PreAllocate(Data.Count());
		for (char charToAdd : Data) {
			Buffer.Add(charToAdd);
		}
	}
}
ByteBuffer::~ByteBuffer() {}

const SArray<char>& ByteBuffer::GetData() const {
	return Buffer;
}
void ByteBuffer::Add(char Data) {
	Buffer.Add(Data);
}
void ByteBuffer::Add(TString Data) {
	for (int i = 0; i < Data.size(); i++) {
		Buffer.Add(Data[i]);
	}
}
int32 ByteBuffer::Size() const {
	return Buffer.Count();
}
bool ByteBuffer::WriteToCompressedFile(TString FilePath) {
	TString compressedData = GetCompressedString();
	std::ofstream outFile(FilePath.c_str(), std::ofstream::binary);
	if (!outFile) {
		SD_ENGINE_ERROR("An error occured when attempting to write compressed file: {0}.", FilePath);
		return false;
	}
	outFile.write(compressedData.c_str(), compressedData.size());
	outFile.close();
	return true;
}
bool ByteBuffer::FromCompressedFile(TString FilePath) {
	Buffer.Clear();

	std::ifstream file;
	file.open(FilePath.c_str(), std::ios::binary);
	if (!file) {
		SD_ENGINE_ERROR("An error occured when attempting to open compressed file: {0}.", FilePath);
		return nullptr;
	}
	file.seekg(0, std::ios::end);
	const int length = (int)file.tellg();
	file.seekg(0, std::ios::beg);
	SArray<char> data(length);
	data.Fill(0, 0, length);
	file.read(&data[0], length);
	file.close();
	DecompressFromCompressedString(data);
	return true;
}
bool ByteBuffer::FromFile(TString FilePath) {
	Buffer.Clear();

	std::ifstream file;
	file.open(FilePath.c_str(), std::ios::binary);
	if (!file) {
		SD_ENGINE_ERROR("An error occured when attempting to open file: {0}.", FilePath);
		return nullptr;
	}
	file.seekg(0, std::ios::end);
	const int length = (int)file.tellg();
	file.seekg(0, std::ios::beg);
	Buffer.Fill(0, length);
	file.read(&Buffer[0], length);
	file.close();
	return true;
}

bool ByteBuffer::DecompressFromCompressedString(const SArray<char>& CompressedData) {
	z_stream zs;                        // z_stream is zlib's control structure
	memset(&zs, 0, sizeof(zs));

	if (inflateInit(&zs) != Z_OK) {
		SD_ENGINE_ERROR("An error occured while attempting to initialize the decompression a byte buffer.");
		return false;
	}

	zs.next_in = (Bytef*)& CompressedData[0];
	zs.avail_in = CompressedData.Count();

	int ret;
	char outbuffer[32768];
	std::string decompressedString;

	// get the decompressed bytes blockwise using repeated calls to inflate
	do {
		zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
		zs.avail_out = sizeof(outbuffer);

		ret = inflate(&zs, 0);

		if (decompressedString.size() < (int32)zs.total_out) {
			decompressedString.append(outbuffer, zs.total_out - decompressedString.size());
		}

	} while (ret == Z_OK);

	inflateEnd(&zs);

	if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
		SD_ENGINE_ERROR("An error occured while attempting to decompress a byte buffer: {0} {1}.", ret, zs.msg);
		return false;
	}

	Buffer.Clear();
	Add(decompressedString);
	return true;
}

TString ByteBuffer::GetCompressedString() const {
	int compressionlevel = Z_BEST_COMPRESSION;
	z_stream zs;                        // z_stream is zlib's control structure
	memset(&zs, 0, sizeof(zs));

	if (deflateInit(&zs, compressionlevel) != Z_OK)
		throw(std::runtime_error("deflateInit failed while compressing."));

	zs.next_in = (Bytef*)& Buffer[0];
	zs.avail_in = Buffer.Count();           // set the z_stream's input

	int ret;
	SArray<char> output;
	output.PreAllocate(Buffer.Count());
	char outbuffer[32768];
	std::string outstring;

	// retrieve the compressed bytes blockwise
	do {
		zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
		zs.avail_out = sizeof(outbuffer);

		ret = deflate(&zs, Z_FINISH);

		if (outstring.size() < (int32)zs.total_out) {
			// append the block to the output string
			outstring.append(outbuffer, zs.total_out - outstring.size());
		}
	} while (ret == Z_OK);

	deflateEnd(&zs);

	if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
		SD_ENGINE_ERROR("An error occured while attempting to compress a byte buffer: {0} {1}.", ret, zs.msg);
	}

	return outstring;
}