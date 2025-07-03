// main.cpp
// This file provides the command-line interface for the Huffman compressor/decompressor.

#include "Huffman.h"
#include <iostream>
#include <string>
#include <vector> // Required for processing command-line arguments

// Main function to handle program execution from the command line.
// It expects arguments like:
// ./huffman_app compress input.txt compressed.huf
// ./huffman_app decompress compressed.huf decompressed.txt
int main(int argc, char* argv[]) {
    // Check if the correct number of arguments are provided.
    // Expected arguments: [program_name] [mode] [input_file] [output_file]
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " [compress|decompress] <input_file> <output_file>" << std::endl;
        return 1; // Indicate an error
    }

    std::string mode = argv[1];         // "compress" or "decompress"
    std::string inputFile = argv[2];    // Path to the input file
    std::string outputFile = argv[3];   // Path for the output file

    Huffman huffman; // Create an instance of the Huffman class

    // Determine the operation based on the mode argument.
    if (mode == "compress") {
        std::cout << "Compressing '" << inputFile << "' to '" << outputFile << "'..." << std::endl;
        huffman.compress(inputFile, outputFile); // Call compress
        // Since compress calls exit(0) on error, if we reach here, it implies the operation started.
        // It's good practice to check if the output file actually exists and has content for better user feedback.
        // For simplicity, we'll assume the internal exit(0) handles hard failures.
        std::cout << "Compression operation requested. Check output file for result." << std::endl;
    } else if (mode == "decompress") {
        std::cout << "Decompressing '" << inputFile << "' to '" << outputFile << "'..." << std::endl;
        huffman.decompress(inputFile, outputFile); // Call decompress
        // Similar to compress, assuming internal exit(0) handles hard failures.
        std::cout << "Decompression operation requested. Check output file for result." << std::endl;
    } else {
        std::cerr << "Invalid mode. Use 'compress' or 'decompress'." << std::endl;
        return 1; // Indicate an error
    }

    return 0; // Indicate success (assuming the operations internally handle errors or exit)
}
