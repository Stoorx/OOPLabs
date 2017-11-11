#ifndef WAV_CORE_H
#define WAV_CORE_H

#include <vector>
#include <string>
#include "wav_header.h"


// TODO: Implement all this in the form of a class.
// TODO: Use an exception system to control errors.
// TODO: Make support for 8-bit PSM, not just 16-bit.
// TODO: Write a function to change the tone of the voice. (Interestingly, it's not too difficult?)


// *********************************************************************
// * Error handling
// *********************************************************************
using namespace std;

class EWavError {
public:
	enum class WavErrorCodes {
		WAV_OK = 0,
		IO_ERROR,
		BAD_FORMAT,
		UNSUPPORTED_FORMAT,
		BAD_PARAMS,
		DATA_SIZE_ERROR
	};
	EWavError(const EWavError::WavErrorCodes eCode) {
		errorCode = eCode;
	}
	EWavError::WavErrorCodes errorCode;
};
class EHeaderError {
public:
	enum class WavHeaderCodes {
		HEADER_OK = 0,
		HEADER_RIFF_ERROR,
		HEADER_FILE_SIZE_ERROR,
		HEADER_WAVE_ERROR,
		HEADER_FMT_ERROR,
		HEADER_NOT_PCM,
		HEADER_SUBCHUNK1_ERROR,
		HEADER_BYTES_RATE_ERROR,
		HEADER_BLOCK_ALIGN_ERROR,
		HEADER_SUBCHUNK2_SIZE_ERROR
	};
	EHeaderError(const EHeaderError::WavHeaderCodes eCode) {
		errorCode = eCode;
	}
	EHeaderError::WavHeaderCodes errorCode;
};

//Done
// Possible errors
enum wav_errors_e {
    WAV_OK = 0,
    IO_ERROR,
    BAD_FORMAT,
    UNSUPPORTED_FORMAT,
    BAD_PARAMS,
    DATA_SIZE_ERROR
};
//Done
// Possible header's errors
enum wav_headers_errors_e {
    HEADER_OK = 0,
    HEADER_RIFF_ERROR,
    HEADER_FILE_SIZE_ERROR,
    HEADER_WAVE_ERROR,
    HEADER_FMT_ERROR,
    HEADER_NOT_PCM,
    HEADER_SUBCHUNK1_ERROR,
    HEADER_BYTES_RATE_ERROR,
    HEADER_BLOCK_ALIGN_ERROR,
    HEADER_SUBCHUNK2_SIZE_ERROR
};

class WavDecoder {
public:
	WavDecoder(const string& fileName);
	WavDecoder(const char* fileName);
	const WavHeader& getHeader();
	const vector<vector<short>>& getSamples();
	const string& getFiename();
private:
	vector<vector<short>>& extractData();
	WavHeader& readHeader(FILE* f);
	void checkHeader(const size_t fileSize);
	string _fileName;
	vector<vector<short>> _samples;
	WavHeader _header;
};
class WavCoder {
public:
	WavCoder() {}
	static void createWav(const string& filename, WavHeader& header, const vector<vector<short>>& samples, int sampleRate);
private:
	static void fillHeader(WavHeader& header, int chanCount, int bitsPerSample, int sampleRate, int samples_count_per_chan);
};
class WavEffects {
public:
	WavEffects(WavHeader& header, vector<vector<short>> &initData);
	void toMono();
	void addReverb(double delay, float decay);
private:
	WavHeader _header;
	vector<vector<short>> _samples;
};

#endif // WAV_CORE_H
