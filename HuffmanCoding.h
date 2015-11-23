#ifndef HUFFMAN_CODING_H
#define HUFFMAN_CODING_H

#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <cstdlib>
#include "HuffmanTree.h"
#include "Timer.h"

using std::string;
using std::cout;

const int nAsciis = 128;             //number of standard ASCII characters
const string fileType = ".hfmc";     //suffix for a compressed file

enum openMode {
	c, d
};                                   //two modes to open files: c for compressing while d for decompressing


class Buffer {
public:
	Buffer():buf(0), beg(-1), end(-1), binary(0){}
	unsigned long buf;
	int beg;
	int end;
	bitset<sizeof(unsigned long) * 8> binary;
};                                   //The buffer Class used to encode and decode

class HuffmanCoding{
public:
	HuffmanCoding();
	~HuffmanCoding();
	void open(string path, openMode mode);
	void compress();                 //compress the file which is opened
	void decompress();               //decompress the file which is opened
	void showPerformance();          //show the compressing ratio and running time
	void close();                    //unbind the file with HuffmanCoding object
private:
	streampos wtf;                   //this is a magic value which is used to deal with counting characters in a file
	openMode mod;
	HuffmanTree tree;                //a huffamn tree used for encoding
	map<char, string> codeDict;      //a dictionary for encoding
	map<string, char> decompressDict;//a dictionary for decoding
	Buffer fileBuf;                  //a buffer used to encode and decode
	string primitiveFilePath;        //path of the primitive file 
	string targetFilePath;           //path of the target file 
	ifstream ifs;
	ofstream ofs;
	double* freqSpace;               //space for storing the frequence of each charcater
	int64_t timeUsedToCompress;      
	int64_t timeUsedToDecompress;
	unsigned long long primitiveSizeKB;
	unsigned long long primitiveSizeByte;
	unsigned long long compressedSizeKB;
	unsigned long long compressedSizeByte;
	unsigned long long calFileSize(string file);         //calculate the size of a file
	void calFreq();                                      //calculate frequence of each charcater
	void encode();                                       //dummy function
	void recursiveEncode(TreeNode* p, string code);      
	void writeHeader();                                  //write file header into a compressed file
	void writeBody();                                    //translate ascii characters into the Huffman codes
	void readHeader();                                   //read file header from a compressed file
	void readBody();                                     //translate the Huffman codes into ascii characters
};
#endif
