// Samuel Sutton
#include "wav.h"
#include <fstream>
#include <iostream>
#include <vector>

WAV::WAV(const std::filesystem::path& file_name)
    :file_name{file_name} {

        // open file
        std::ifstream file(file_name, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Could not open wav file: " + file_name.string());
        }

        // TODO: DELETE
        // char byte;
        // while (file.get(byte)) {
        //     std::cout << std::bitset<8>(static_cast<unsigned char>(byte)) << " ";
        // }
        // TODO: DELETE

        //--------------------------------------------------------------------------------
        //                                  HEADER
        //--------------------------------------------------------------------------------

        // ------------------------------ RIFF header ------------------------------
        chunk_id = read_u32_bigend(file);
        chunk_size = read_u32_lilend(file);
        format = read_u32_bigend(file);

        // TODO: change read_u32_bigend to return string to compare against strings?
        if (chunk_id != 0x52494646) { // "RIFF"
            throw std::runtime_error("Not a valid WAV file: missing RIFF tag: " + file_name.string());
        }
        if (format != 0x57415645) { // "WAVE"
            throw std::runtime_error("Not a valid WAV file: missing WAVE tag: " + file_name.string());
        }

        // ------------------------------ fmt subchunk ------------------------------
        subchunk_1_id = read_u32_bigend(file);
        subchunk_1_size = read_u32_lilend(file);

        // TODO: change read_u32_bigend to return string to compare against strings?
        if (subchunk_1_id != 0x666d7420) { // "fmt "
            throw std::runtime_error("Not a valid WAV file: missing fmt tag: " + file_name.string());
        }

        audio_format = read_u16_lilend(file);
        num_channels = read_u16_lilend(file);
        sample_rate = read_u32_lilend(file);
        byte_rate = read_u32_lilend(file);
        block_align = read_u16_lilend(file);
        bits_per_sample = read_u16_lilend(file);

        // TODO: might be more stuff in fmt depending?? (check size maybe?)
        
        // TODO: -SEEK- TO FIND WHERE DATA STARTS (SKIP EXTRA?)

        // ------------------------------ data subchunk ------------------------------
        subchunk_2_id = read_u32_bigend(file);
        subchunk_2_size = read_u32_lilend(file);

        // TODO: change read_u32_bigend to return string to compare against strings?
        if (subchunk_2_id != 0x64617461) { // "data"
            throw std::runtime_error("Not a valid WAV file: missing data tag: " + file_name.string());
        } 

        //--------------------------------------------------------------------------------
        //                                  DATA
        //--------------------------------------------------------------------------------
        // TODO: read audio data (make sure at "data" part)
}


//--------------------------------------------------------------------------------
//                                  HELPER FUNCTIONS
//--------------------------------------------------------------------------------
// 32 bits - big endian
uint32_t WAV::read_u32_bigend(std::ifstream& file) {
    uint8_t bytes[4];
    file.read(reinterpret_cast<char*>(bytes), 4);
    if (file.gcount() != 4) {
        throw std::runtime_error("WAV file shorter than expected uint32_t data: " + file_name.string());
    }
    return (static_cast<uint32_t>(bytes[0]) << 24) | // big endian
            (static_cast<uint32_t>(bytes[1]) << 16) |
            (static_cast<uint32_t>(bytes[2]) << 8)  |
            (static_cast<uint32_t>(bytes[3]));
}

// 32 bits - little endian
uint32_t WAV::read_u32_lilend(std::ifstream& file) {
    uint8_t bytes[4];
    file.read(reinterpret_cast<char*>(bytes), 4);
    if (file.gcount() != 4) {
        throw std::runtime_error("WAV file shorter than expected uint32_t data: " + file_name.string());
    }
    return (static_cast<uint32_t>(bytes[0])) | // little endian
            (static_cast<uint32_t>(bytes[1]) << 8) |
            (static_cast<uint32_t>(bytes[2]) << 16)  |
            (static_cast<uint32_t>(bytes[3]) << 24);
}

// 16 bits - little endian
uint16_t WAV::read_u16_lilend(std::ifstream& file) {
    uint8_t bytes[2];
    file.read(reinterpret_cast<char*>(bytes), 2);
    if (file.gcount() != 2) {
        throw std::runtime_error("WAV file shorter than expected uint16_t data: " + file_name.string());
    }
    return (static_cast<uint16_t>(bytes[0])) | // little endian
            (static_cast<uint16_t>(bytes[1]) << 8);
}

// TODO: move these functions elsewhere for using with diff file types too?



// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
int main() {
    WAV file{"Code/Core/Data_Types/doe_hunting.wav"};
}
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
// TODO: DELETE THIS!!! FOR TESTING ONLY!!!
