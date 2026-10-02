// Samuel Sutton
#pragma once
#include <filesystem>

class WAV {
public:
    // constructor
    WAV(const std::filesystem::path& file_name);

    void read_file();

private:
    const std::filesystem::path file_name;

    // RIFF chunk descriptor
    uint32_t chunk_id; // 4 bytes - 0 offset - big endian
    uint32_t chunk_size; // 4 bytes - 4 offset - little endian
    uint32_t format; // 4 bytes - 8 offset - big endian

    // fmt subchunk
    uint32_t subchunk_1_id; // 4 bytes - 12 offset - big endian
    uint32_t subchunk_1_size; // 4 bytes - 16 offset - little endian
    uint16_t audio_format; // 2 bytes - 20 offset - little endian
    uint16_t num_channels; // 2 bytes - 22 offset - little endian
    uint32_t sample_rate; // 4 bytes - 24 offset - little endian
    uint32_t byte_rate; // 4 bytes - 28 offset - little endian
    uint16_t block_align; // 2 bytes - 32 offset - little endian
    uint16_t bits_per_sample; // 2 bytes - 34 offset - little endian

    // data info
    uint32_t subchunk_2_id; // 4 bytes - 36 offset - big endian
    uint32_t subchunk_2_size; // 4 bytes - 40 offset - little endian
    
    // DATA
    uint8_t data; // 44 offset - little endian


    // helper functions
    uint32_t read_u32_bigend(std::ifstream& file);
    uint32_t read_u32_lilend(std::ifstream& file);
    uint16_t read_u16_lilend(std::ifstream& file);
};