#include <cstdio>
#include <string>

#include "wav_header.h"
#include "wav_core.h"


// TODO: Remove all 'magic' numbers
// TODO: Make the code more secure. Get rid of pointers (after creating a class, of course).

WavDecoder::WavDecoder(const string& fileName) {
	_fileName = fileName;
}
WavDecoder::WavDecoder(const char* fileName) {
	_fileName = string(fileName);
}

WavHeader& WavDecoder::readHeader(FILE* f) {
	WavHeader tmp;

	if (!f) {
		throw EWavError(EWavError::WavErrorCodes::IO_ERROR);
	}

	size_t blocks_read = fread(&_header, sizeof(WavHeader), 1, f);
	if (blocks_read != 1) {
		// can't read header, because the file is too small.
		throw EWavError(EWavError::WavErrorCodes::BAD_FORMAT);
	}

	fseek(f, 0, SEEK_END); // seek to the end of the file
	size_t file_size = ftell(f); // current position is a file size!

	checkHeader(file_size);

	return tmp;
}


const WavHeader& WavDecoder::getHeader() {
	return _header;
}
const vector<vector<short>>& WavDecoder::getSamples() {
	return _samples;
}

const string& WavDecoder::getFiename() {
	return _fileName;
}

vector<vector<short>>& WavDecoder::extractData() {

	WavHeader header;
	FILE* f = fopen(_fileName.data(), "rb");

	header = readHeader(f);
	
	if (header.bitsPerSample != 16) {
		// Only 16-bit samples is supported.
		throw EWavError(EWavError::WavErrorCodes::UNSUPPORTED_FORMAT);
	}

	if (!f) {
		throw EWavError(EWavError::WavErrorCodes::IO_ERROR);
	}
	fseek(f, 44, SEEK_SET); // Seek to the begining of PCM data.

	int chan_count = header.numChannels;
	int samples_per_chan = (header.subchunk2Size / sizeof(short)) / chan_count;

	// 1. Reading all PCM data from file to a single vector.
	std::vector<short> all_channels;
	all_channels.resize(chan_count * samples_per_chan);
	size_t read_bytes = fread(all_channels.data(), 1, header.subchunk2Size, f);
	if (read_bytes != header.subchunk2Size) {
		throw EWavError(EWavError::WavErrorCodes::IO_ERROR);
	}
	fclose(f);

	vector<vector<short>> channelsData;
	// 2. Put all channels to its own vector.
	channelsData.resize(chan_count);
	for (size_t ch = 0; ch < channelsData.size(); ch++) {
		channelsData[ch].resize(samples_per_chan);
	}

	for (int ch = 0; ch < chan_count; ch++) {
		std::vector<short>& chdata = channelsData[ch];
		for (size_t i = 0; i < samples_per_chan; i++) {
			chdata[i] = all_channels[chan_count * i + ch];
		}
	}

	_samples = channelsData;
	return channelsData;
}
void WavDecoder::checkHeader(const size_t fileSize) {
	// Go to wav_header.h for details

	if (_header.chunkId[0] != 0x52 ||
		_header.chunkId[1] != 0x49 ||
		_header.chunkId[2] != 0x46 ||
		_header.chunkId[3] != 0x46)
	{
		throw EHeaderError(EHeaderError::WavHeaderCodes::HEADER_RIFF_ERROR);
	}

	if (_header.chunkSize != fileSize - 8) {
		throw EHeaderError(EHeaderError::WavHeaderCodes::HEADER_FILE_SIZE_ERROR);
	}

	if (_header.format[0] != 0x57 ||
		_header.format[1] != 0x41 ||
		_header.format[2] != 0x56 ||
		_header.format[3] != 0x45)
	{
		throw EHeaderError(EHeaderError::WavHeaderCodes::HEADER_WAVE_ERROR);
	}

	if (_header.subchunk1Id[0] != 0x66 ||
		_header.subchunk1Id[1] != 0x6d ||
		_header.subchunk1Id[2] != 0x74 ||
		_header.subchunk1Id[3] != 0x20)
	{
		throw EHeaderError(EHeaderError::WavHeaderCodes::HEADER_RIFF_ERROR);
	}

	if (_header.audioFormat != 1) {
		throw EHeaderError(EHeaderError::WavHeaderCodes::HEADER_RIFF_ERROR);
	}

	if (_header.subchunk1Size != 16) {
		throw EHeaderError(EHeaderError::WavHeaderCodes::HEADER_RIFF_ERROR);
	}

	if (_header.byteRate != _header.sampleRate * _header.numChannels * _header.bitsPerSample / 8) {
		throw EHeaderError(EHeaderError::WavHeaderCodes::HEADER_RIFF_ERROR);
	}

	if (_header.blockAlign != _header.numChannels * _header.bitsPerSample / 8) {
		throw EHeaderError(EHeaderError::WavHeaderCodes::HEADER_RIFF_ERROR);
	}

	if (_header.subchunk2Id[0] != 0x64 ||
		_header.subchunk2Id[1] != 0x61 ||
		_header.subchunk2Id[2] != 0x74 ||
		_header.subchunk2Id[3] != 0x61)
	{
		throw EHeaderError(EHeaderError::WavHeaderCodes::HEADER_RIFF_ERROR);
	}

	if (_header.subchunk2Size != fileSize - 44)
	{
		throw EHeaderError(EHeaderError::WavHeaderCodes::HEADER_RIFF_ERROR);
	}
}


void WavCoder::createWav(const string& filename, WavHeader & header, const vector<vector<short>>& samples, int sampleRate)
{
	int chan_count = (int)samples.size();

	if (chan_count < 1) {
		throw EWavError(EWavError::WavErrorCodes::BAD_PARAMS);
	}

	int samples_count_per_chan = (int)samples[0].size();

	// Verify that all channels have the same number of samples.
	for (size_t ch = 0; ch < chan_count; ch++) {
		if (samples[ch].size() != (size_t)samples_count_per_chan) {
			throw EWavError(EWavError::WavErrorCodes::BAD_PARAMS);
			
		}
	}

	fillHeader(header, chan_count, 16, sampleRate, samples_count_per_chan);

	std::vector<short> all_channels;
	all_channels.resize(chan_count * samples_count_per_chan);

	for (int ch = 0; ch < chan_count; ch++) {
		const std::vector<short>& chdata = samples[ch];
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			all_channels[chan_count * i + ch] = chdata[i];
		}
	}

	FILE* f = fopen(filename.data(), "wb");
	fwrite(&header, sizeof(wav_header_s), 1, f);
	fwrite(all_channels.data(), sizeof(short), all_channels.size(), f);
	if (!f) {
		throw EWavError(EWavError::WavErrorCodes::IO_ERROR);
	}

	fclose(f);
}

void WavCoder::fillHeader(WavHeader & header, int chanCount, int bitsPerSample, int sampleRate, int samples_count_per_chan)
{
	if (bitsPerSample != 16) {
		throw EWavError(EWavError::WavErrorCodes::UNSUPPORTED_FORMAT);
	}

	if (chanCount < 1) {
		throw EWavError(EWavError::WavErrorCodes::BAD_PARAMS);
	}

	int file_size_bytes = 44 + chanCount * (bitsPerSample / 8) * samples_count_per_chan;

	header.sampleRate = sampleRate;
	header.numChannels = chanCount;
	header.bitsPerSample = 16;

	header.chunkSize = file_size_bytes - 8;
	header.subchunk2Size = file_size_bytes - 44;

	header.byteRate = header.sampleRate * header.numChannels * header.bitsPerSample / 8;
	header.blockAlign = header.numChannels * header.bitsPerSample / 8;

}

WavEffects::WavEffects(WavHeader & header, vector<vector<short>>& initData)
{
	_header = header;
	_samples = initData;
}

void WavEffects::toMono()
{
	vector<vector<short>> dest_mono;
	int chan_count = (int)_samples.size();

	if (chan_count != 2) {
		throw EWavError(EWavError::WavErrorCodes::BAD_PARAMS);
	}

	int samples_count_per_chan = (int)_samples[0].size();

	// Verify that all channels have the same number of samples.
	for (size_t ch = 0; ch < chan_count; ch++) {
		if (_samples[ch].size() != (size_t)samples_count_per_chan) {
			throw EWavError(EWavError::WavErrorCodes::BAD_PARAMS);
		}
	}

	dest_mono.resize(1);
	std::vector<short>& mono = dest_mono[0];
	mono.resize(samples_count_per_chan);

	// Mono channel is an arithmetic mean of all (two) channels.
	for (size_t i = 0; i < samples_count_per_chan; i++) {
		mono[i] = (_samples[0][i] + _samples[1][i]) / 2;
	}
	_samples = dest_mono;
}

void WavEffects::addReverb(double delay, float decay)
{
	int chan_count = (int)_samples.size();

	if (chan_count < 1) {
		throw EWavError(EWavError::WavErrorCodes::BAD_PARAMS);
	}

	int samples_count_per_chan = (int)_samples[0].size();

	// Verify that all channels have the same number of samples.
	for (size_t ch = 0; ch < chan_count; ch++) {
		if (_samples[ch].size() != (size_t)samples_count_per_chan) {
			throw EWavError(EWavError::WavErrorCodes::BAD_PARAMS);
		}
	}

	int delay_samples = (int)(delay * _header.sampleRate);


	for (size_t ch = 0; ch < chan_count; ch++) {
		std::vector<float> tmp;
		tmp.resize(_samples[ch].size());

		// Convert signal from short to float
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			tmp[i] = _samples[ch][i];
		}

		// Add a reverb
		for (size_t i = 0; i < samples_count_per_chan - delay_samples; i++) {
			tmp[i + delay_samples] += decay * tmp[i];
		}

		// Find maximum signal's magnitude
		float max_magnitude = 0.0f;
		for (size_t i = 0; i < samples_count_per_chan - delay_samples; i++) {
			if (abs(tmp[i]) > max_magnitude) {
				max_magnitude = abs(tmp[i]);
			}
		}

		// Signed short can keep values from -32768 to +32767,
		// After reverb, usually there are values large 32000.
		// So we must scale all values back to [ -32768 ... 32768 ]
		float norm_coef = 30000.0f / max_magnitude;
		// Scale back and transform floats to shorts.
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			_samples[ch][i] = (short)(norm_coef * tmp[i]);
		}
	}
}
