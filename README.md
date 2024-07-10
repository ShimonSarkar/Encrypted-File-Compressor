# Huffman Compressor with Encryption

Welcome to the Huffman Compressor. This application allows you to compress and decompress files using Huffman coding, combined with a simple XOR encryption for added security.

## What does it do?

This application provides a way to compress and decompress text files using Huffman coding. The main features include:

- 📉 Compress files to reduce their size
- 🔄 Decompress files to restore original content
- 🔐 Encrypt and decrypt data using a simple XOR encryption

## Why I made it

I wanted to create a project that utilizes Huffman coding to understand data compression techniques better. This project offers a hands-on way to learn about Huffman trees, bit manipulation, and encryption in C++.

## How to Use

To use this Huffman Compressor, follow these steps:

1. **Make sure you have a C++ compiler installed** (like g++).
2. **Build the project** by executing the following command in your terminal:
   ```sh
   make all
   ```
3. **Run the Huffman Compressor**. The basic usage is:

   ```sh
   ./hfc <operation> <input_file> <output_file> <key>
   ```

   - `<operation>`: `compress` or `decompress`
   - `<input_file>`: The file to compress or decompress
   - `<output_file>`: The file to write the output
   - `<key>`: The key used for XOR encryption/decryption

   For example, to compress a file:

   ```sh
   ./hfc compress input.txt output.hfc 123
   ```

   To decompress a file:

   ```sh
   ./hfc decompress output.hfc decompressed.txt 123
   ```

## Command-Line Arguments

- `compress <input_file> <output_file> <key>`: Compress the input file and save the output to the specified file, using the provided key for encryption.
- `decompress <input_file> <output_file> <key>`: Decompress the input file and save the output to the specified file, using the provided key for decryption.

## Example

Compressing a file:

```sh
./hfc compress example.txt example.hfc 123
```

Decompressing a file:

```sh
./hfc decompress example.hfc example_decompressed.txt 123
```

## Notes

- Ensure that the input file exists before running the compression or decompression commands.
- The XOR encryption key should be the same for both compression and decompression to ensure data integrity.
- The program prints various details, such as the size of the input file, compressed data, and the number of Huffman codes, to the console for informational purposes.
