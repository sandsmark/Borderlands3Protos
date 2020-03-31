#include <iostream>
#include <fstream>
#include <vector>

#define HEADER_SIZE 0x400
#define COMPRESSED_SIZE 0x2A000

int main(int argc, char *argv[]) try
{
    if (argc < 2) {
        std::cerr << "Please pass a file" << std::endl;
        return 1;
    }
    std::ifstream in(argv[1], std::ios_base::in | std::ios_base::binary);
    if (!in.is_open()) {
        std::cerr << "Couldn't open " << argv[1] << std::endl;
        return 1;
    }

    in.seekg(0, std::ifstream::end);
    const std::streamsize size = in.tellg();
    in.seekg(0);
    std::cout << "0x" << std::hex << size << std::dec << " bytes total" << std::endl;
    if (HEADER_SIZE + COMPRESSED_SIZE > size) {
        std::cerr << "File not large enough" << std::endl;
        std::cerr << "Minimum size: " << (HEADER_SIZE + COMPRESSED_SIZE) << std::endl;
        return 1;
    }
    const std::streamsize encryptedSize = size - (HEADER_SIZE + COMPRESSED_SIZE);
    std::cout << "0x" << std::hex << encryptedSize << std::dec << " bytes encrypted" << std::endl;

    std::vector<char> buf(0x400);
    in.read(buf.data(), buf.size());
    if (in.gcount() != HEADER_SIZE) {
        std::cerr << "Failed to read header from " << argv[1] << std::endl;
        return 1;
    }

    {
        std::ofstream out("header.bin", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        if (!out.is_open()) {
            std::cerr << "Couldn't open header.bin for writing" << std::endl;
            return 1;
        }
        out.write(buf.data(), buf.size());
    }

    buf.resize(COMPRESSED_SIZE);
    in.read(buf.data(), buf.size());
    if (in.gcount() != COMPRESSED_SIZE) {
        std::cerr << "Failed to read compressed from " << argv[1] << std::endl;
        return 1;
    }

    {
        std::ofstream out("compressed.bin", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        if (!out.is_open()) {
            std::cerr << "Couldn't open compressed.bin for writing" << std::endl;
            return 1;
        }
        out.write(buf.data(), buf.size());
    }

    buf.resize(encryptedSize);
    in.read(buf.data(), buf.size());
    if (in.gcount() != encryptedSize) {
        std::cerr << "Failed to read encrypted from " << argv[1] << std::endl;
        return 1;
    }

    {
        std::ofstream out("encrypted.bin", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        if (!out.is_open()) {
            std::cerr << "Couldn't open encrypted.bin for writing" << std::endl;
            return 1;
        }
        out.write(buf.data(), buf.size());
    }

    return 0;
} catch (const std::exception &e) {
    std::cerr << "uncatched exception '" << e.what() << "', terminating uncleanly" << std::endl;
    return 1;
}

