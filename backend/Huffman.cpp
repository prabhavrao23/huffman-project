#include "Huffman.h"

Huffman::Huffman() {
    root = nullptr;
}

Huffman::~Huffman() {
    deleteTree(root);
}

void Huffman::deleteTree(Node* node) {
    if (!node) return;
    deleteTree(node->zero);
    deleteTree(node->one);
    delete node;
}

std::string Huffman::inFiles(const std::string& filename) {
    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    if (content.empty()) {
        std::cerr << "File is empty: " << filename << std::endl;
    }
    return content;
}

void Huffman::frequencyTable(const std::string& text, std::unordered_map<char, int>& freqTable) {
    for (char c : text) {
        freqTable[c]++;
    }
}

void Huffman::buildTree(const std::unordered_map<char, int>& freqTable) {
    auto compare = [](Node* a, Node* b) {
        return a->weight > b->weight;
    };
    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> pq(compare);

    for (const auto& pair : freqTable) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* parent = new Node('\0', left->weight + right->weight, left, right);
        pq.push(parent);
    }

    root = pq.top();
}

void Huffman::making_codes(Node* node, const std::string& code, std::unordered_map<char, std::string>& huffmanCodes) {
    if (!node) return;
    if (!node->zero && !node->one) {
        huffmanCodes[node->letter] = code;
    }
    making_codes(node->zero, code + "0", huffmanCodes);
    making_codes(node->one, code + "1", huffmanCodes);
}

std::string Huffman::encodeTree(Node* node) {
    if (!node) return "0";
    if (!node->zero && !node->one) return "1" + std::string(1, node->letter);
    return "0" + encodeTree(node->zero) + encodeTree(node->one);
}

Node* Huffman::decodeTree(const std::string& data, size_t& index) {
    if (index >= data.size()) return nullptr;
    char current = data[index++];
    if (current == '1') {
        return new Node(data[index++], 0);
    }
    Node* left = decodeTree(data, index);
    Node* right = decodeTree(data, index);
    return new Node('\0', 0, left, right);
}

void Huffman::reconstruct_tree(const std::string& header) {
    size_t index = 0;
    root = decodeTree(header, index);
}

void Huffman::ReverseCodes(Node* node, const std::string& code, std::unordered_map<std::string, char>& code_Char) {
    if (!node) return;
    if (!node->zero && !node->one) {
        code_Char[code] = node->letter;
    }
    ReverseCodes(node->zero, code + "0", code_Char);
    ReverseCodes(node->one, code + "1", code_Char);
}

void Huffman::compress(const std::string& inputFile, const std::string& outputFile) {
    std::string content = inFiles(inputFile);
    if (content.empty()) return;

    std::unordered_map<char, int> freqTable;
    frequencyTable(content, freqTable);

    buildTree(freqTable);

    std::unordered_map<char, std::string> huffmanCodes;
    making_codes(root, "", huffmanCodes);

    std::string header = encodeTree(root) + "\n";

    Storage storage;
    if (!storage.open(outputFile, "write")) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        return;
    }

    storage.setHeader(header);
    for (char c : content) {
        storage.insert(huffmanCodes[c]);
    }

    std::cout << "File compressed successfully!" << std::endl;
    deleteTree(root);
    root = nullptr;
    storage.close();
}

/*
void Huffman::decompress(const std::string &inFilePath, const std::string &outFilePath) {
    std::ifstream inputFile(inFilePath, std::ios::binary);
    std::string headerData;
    std::getline(inputFile, headerData);

    std::unordered_map<std::string, char> reverseHuffmanCodes;
    std::stringstream headerStream(headerData);
    std::string codePair;
    while (std::getline(headerStream, codePair, '|')) {
        size_t pos = codePair.find(':');
        char character = codePair[0];
        std::string code = codePair.substr(pos + 1);
        reverseHuffmanCodes[code] = character;
    }

    std::string encodedText;
    char byte;
    while (inputFile.get(byte)) {
        encodedText += std::bitset<8>(byte).to_string();
    }

    std::string decodedText;
    std::string currentCode;
    size_t i = 0;
    while (i < encodedText.size()) {
        currentCode += encodedText[i];
        if (reverseHuffmanCodes.find(currentCode) != reverseHuffmanCodes.end()) {
            decodedText += reverseHuffmanCodes[currentCode];
            currentCode.clear();
        }
        i++;
    }

    std::ofstream outputFile(outFilePath);
    outputFile << decodedText;
    inputFile.close();
    outputFile.close();
}
*/

bool Huffman::decompress(const std::string& inputFile, const std::string& outputFile) {
    Storage storage;
    if (!storage.open(inputFile, "read")) {
        std::cerr << "Failed to open input file: " << inputFile << std::endl;
        return false;
    }

    reconstruct_tree(storage.getHeader());

    std::unordered_map<std::string, char> reverseCodes;
    ReverseCodes(root, "", reverseCodes);

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        return false;
    }

    std::string binaryData;
    std::string currentCode;

    while (storage.extract(binaryData)) {
        for (char bit : binaryData) {
            currentCode += bit;
            if (reverseCodes.find(currentCode) != reverseCodes.end()) {
                outFile.put(reverseCodes[currentCode]);
                currentCode.clear();
            }
        }
    }

    storage.close();
    outFile.close();

    std::cout << "File decompressed successfully!" << std::endl;
    return true;
}