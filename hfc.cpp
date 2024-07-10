#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <algorithm>
#include <sstream>
#include <cstdint>

struct Node
{
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f, Node *l = nullptr, Node *r = nullptr)
        : ch(c), freq(f), left(l), right(r) {}

    ~Node()
    {
        delete left;
        delete right;
    }
};

struct Compare
{
    bool operator()(Node *l, Node *r)
    {
        return l->freq > r->freq;
    }
};

class HuffmanCompressor
{
private:
    std::unordered_map<char, std::string> huffmanCodes;

    void generateCodes(Node *root, std::string str)
    {
        if (!root)
            return;
        if (root->ch != '\0')
            huffmanCodes[root->ch] = str;
        generateCodes(root->left, str + "0");
        generateCodes(root->right, str + "1");
    }

    Node *buildHuffmanTree(const std::string &text)
    {
        std::unordered_map<char, int> freq;
        for (char c : text)
            freq[c]++;

        std::priority_queue<Node *, std::vector<Node *>, Compare> pq;
        for (auto &pair : freq)
            pq.push(new Node(pair.first, pair.second));

        while (pq.size() > 1)
        {
            Node *left = pq.top();
            pq.pop();
            Node *right = pq.top();
            pq.pop();
            pq.push(new Node('\0', left->freq + right->freq, left, right));
        }

        return pq.top();
    }

    std::string encryptData(const std::string &data, int key)
    {
        std::string encrypted = data;
        for (char &c : encrypted)
            c = c ^ key;
        return encrypted;
    }

    std::string decryptData(const std::string &data, int key)
    {
        return encryptData(data, key);
    }

    std::string bitsToString(const std::string &bits)
    {
        std::string result;
        for (size_t i = 0; i < bits.length(); i += 8)
        {
            result += static_cast<char>(std::bitset<8>(bits.substr(i, 8)).to_ulong());
        }
        return result;
    }

    std::string stringToBits(const std::string &str)
    {
        std::string result;
        for (char c : str)
        {
            result += std::bitset<8>(c).to_string();
        }
        return result;
    }

public:
    void compressFile(const std::string &inputFile, const std::string &outputFile, int key)
    {
        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile)
        {
            std::cerr << "Error opening input file." << std::endl;
            return;
        }
        std::string text((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();

        std::cout << "Input file size: " << text.length() << " bytes" << std::endl;

        std::string compressed = compress(text, key);

        std::ofstream outFile(outputFile, std::ios::binary);
        if (!outFile)
        {
            std::cerr << "Error opening output file." << std::endl;
            return;
        }

        uint32_t codeCount = huffmanCodes.size();
        outFile.write(reinterpret_cast<const char *>(&codeCount), sizeof(codeCount));
        for (const auto &pair : huffmanCodes)
        {
            outFile.put(pair.first);
            uint16_t codeLength = pair.second.length();
            outFile.write(reinterpret_cast<const char *>(&codeLength), sizeof(codeLength));
            outFile.write(pair.second.c_str(), codeLength);
        }

        outFile.write(compressed.c_str(), compressed.length());
        outFile.close();

        std::cout << "File compressed successfully." << std::endl;
        std::cout << "Compressed size: " << compressed.length() << " bytes" << std::endl;
        std::cout << "Number of Huffman codes: " << huffmanCodes.size() << std::endl;
    }

    void decompressFile(const std::string &inputFile, const std::string &outputFile, int key)
    {
        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile)
        {
            std::cerr << "Error opening input file." << std::endl;
            return;
        }

        std::unordered_map<char, std::string> codes;
        uint32_t codeCount;
        inFile.read(reinterpret_cast<char *>(&codeCount), sizeof(codeCount));
        for (uint32_t i = 0; i < codeCount; ++i)
        {
            char ch;
            uint16_t codeLength;
            inFile.get(ch);
            inFile.read(reinterpret_cast<char *>(&codeLength), sizeof(codeLength));
            std::string code(codeLength, ' ');
            inFile.read(&code[0], codeLength);
            codes[ch] = code;
        }

        std::string compressed((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();

        std::cout << "Compressed data size: " << compressed.length() << " bytes" << std::endl;

        std::string decompressed = decompress(compressed, key, codes);

        std::ofstream outFile(outputFile, std::ios::binary);
        if (!outFile)
        {
            std::cerr << "Error opening output file." << std::endl;
            return;
        }
        outFile.write(decompressed.c_str(), decompressed.length());
        outFile.close();

        std::cout << "Decompressed size: " << decompressed.length() << " bytes" << std::endl;
        std::cout << "File decompressed successfully." << std::endl;
    }

    std::string compress(const std::string &text, int key)
    {
        if (text.empty())
        {
            std::cerr << "Warning: Empty input text" << std::endl;
            return "";
        }

        Node *root = buildHuffmanTree(text);
        generateCodes(root, "");
        delete root;

        std::string compressed;
        for (char c : text)
            compressed += huffmanCodes[c];

        uint8_t paddingLength = 8 - (compressed.length() % 8);
        if (paddingLength < 8)
        {
            compressed += std::string(paddingLength, '0');
        }
        else
        {
            paddingLength = 0;
        }

        std::string result;
        result += static_cast<char>(paddingLength);
        result += static_cast<char>((text.length() >> 24) & 0xFF);
        result += static_cast<char>((text.length() >> 16) & 0xFF);
        result += static_cast<char>((text.length() >> 8) & 0xFF);
        result += static_cast<char>(text.length() & 0xFF);
        result += bitsToString(compressed);

        return encryptData(result, key);
    }

    std::string decompress(const std::string &compressed, int key, const std::unordered_map<char, std::string> &codes)
    {
        std::string decrypted = decryptData(compressed, key);

        if (decrypted.length() < 5)
        {
            std::cerr << "Error: Compressed data is too short" << std::endl;
            return "";
        }

        uint8_t paddingLength = static_cast<uint8_t>(decrypted[0]);
        uint32_t originalLength =
            (static_cast<unsigned char>(decrypted[1]) << 24) |
            (static_cast<unsigned char>(decrypted[2]) << 16) |
            (static_cast<unsigned char>(decrypted[3]) << 8) |
            static_cast<unsigned char>(decrypted[4]);

        std::cout << "Original text length: " << originalLength << " bytes" << std::endl;

        std::string bits = stringToBits(decrypted.substr(5));
        if (paddingLength > 0)
        {
            bits = bits.substr(0, bits.length() - paddingLength);
        }

        std::string decompressed;
        std::string currentCode;
        std::unordered_map<std::string, char> reverseMap;
        for (const auto &pair : codes)
            reverseMap[pair.second] = pair.first;

        for (char bit : bits)
        {
            currentCode += bit;
            if (reverseMap.find(currentCode) != reverseMap.end())
            {
                decompressed += reverseMap[currentCode];
                currentCode.clear();
                if (decompressed.length() == originalLength)
                {
                    break;
                }
            }
        }

        if (decompressed.length() != originalLength)
        {
            std::cerr << "Warning: Decompressed length doesn't match original length" << std::endl;
        }

        return decompressed;
    }
};

void printUsage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "  Compress:   ./hfc compress <input_file> <output_file> <key>" << std::endl;
    std::cout << "  Decompress: ./hfc decompress <input_file> <output_file> <key>" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printUsage();
        return 1;
    }

    std::string operation = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];
    int key = std::stoi(argv[4]);

    HuffmanCompressor compressor;

    if (operation == "compress")
    {
        std::cout << "Starting compression..." << std::endl;
        compressor.compressFile(inputFile, outputFile, key);
    }
    else if (operation == "decompress")
    {
        std::cout << "Starting decompression..." << std::endl;
        compressor.decompressFile(inputFile, outputFile, key);
    }
    else
    {
        std::cout << "Invalid operation. Use 'compress' or 'decompress'." << std::endl;
        printUsage();
        return 1;
    }

    std::cout << "Operation completed." << std::endl;
    return 0;
}