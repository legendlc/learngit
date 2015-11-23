#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include"HuffmanCoding.h"
using namespace std;

HuffmanCoding::HuffmanCoding() {
	primitiveFilePath = "";
	targetFilePath = "";
	freqSpace = new double[nAsciis];
	for (int i = 0; i < nAsciis; i++) {
		freqSpace[i] = 0;
	}
	timeUsedToCompress = 0;
	timeUsedToDecompress = 0;
	primitiveSizeKB = 0;
	compressedSizeKB = 0;
}
HuffmanCoding::~HuffmanCoding() {
	delete freqSpace;
}
void HuffmanCoding::open(string path, openMode mode) {
	ifs.open(path.c_str(), ios::in);
	if (!ifs) {
		cout << "File: " << path << " does not exist." << endl;
		exit(0);
	}
	mod = mode;
	if (mode == d) {
		if (path.find(fileType) == path.size()) {
			cout << "The file to be decompressed is not a .hfmc file." << endl;
			ifs.close();
			exit(0);
		}
	}
	primitiveFilePath = path;
	ifs.close();
}
void HuffmanCoding::compress() {
	primitiveSizeByte = calFileSize(primitiveFilePath);
	primitiveSizeKB = primitiveSizeByte / 1024;
	calFreq();
	tree.create(freqSpace, nAsciis);
	encode();
	/*
	for (map<char, string>::iterator it = codeDict.begin(); it != codeDict.end(); it++) {
		cout << it->first << '\t' << it->second << endl;
	}
	*/
	for (int i = 0; i < nAsciis; i++) {
		if (freqSpace[i] == 0) {
			codeDict.erase(char(i));
		}
	}                                             //将文件中不存在的字符从字典中清除
	cout << "Please input the filename for the new file:" << endl;
	cin >> targetFilePath;
	targetFilePath = targetFilePath + fileType;
	ofs.open(targetFilePath.c_str(), ios::out);
	if (!ofs) {
		cout << "Creating file " << targetFilePath << "fails" << endl;
		exit(0);
	}
	ofs.close();
	Timer t;
	t.start();
	writeHeader();
	writeBody();
	t.end();
	timeUsedToCompress = t.getRunningTime_MS();
	compressedSizeByte = calFileSize(targetFilePath);
	compressedSizeKB = compressedSizeByte / 1024;
}
void HuffmanCoding::writeHeader() {
	ofs.open(targetFilePath.c_str(), ios::out | ios::binary);
	ofs.write(primitiveFilePath.c_str(), primitiveFilePath.size());
	char newline = '\n';
	char space = '\t';
	ofs.write(&newline, sizeof(char));
	wtf = ofs.tellp();
	ofs.write((char*)&primitiveSizeByte, sizeof(primitiveSizeByte));
	ofs.write(&newline, sizeof(char));
	int dictSize = codeDict.size();
	//map<char, string>::size_type dictSize = codeDict.size();
	ofs.write((char*)&dictSize, sizeof(dictSize));
	ofs.write(&newline, sizeof(char));
	//Write dictionary into the target file
	for (map<char, string>::iterator it = codeDict.begin(); it != codeDict.end(); it++) {
		ofs.write((char*)(&(it->first)), sizeof(it->first));
		ofs.write(&space, sizeof(char));
		ofs << it->second;
		ofs.write(&newline, sizeof(char));
	}
	ofs.close();
}
void HuffmanCoding::writeBody() {
	char ch;
	ifs.open(primitiveFilePath.c_str(), ios::in);
	ofs.open(targetFilePath.c_str(), ios::in | ios::out | ios::binary);
	ofs.seekp(0, ios::end);
	unsigned long long byteCount = 0;
	cout << "Progress ";
	while (ifs.peek() != EOF) {
		if (byteCount % 1000 == 0) {
			cout << setw(6) << right << setprecision(2) << fixed << byteCount * 100.0 / primitiveSizeByte << " %";
		}
		ch = ifs.get();
		byteCount++;
		string s = codeDict[ch];
		int bits = s.size();
		fileBuf.binary = bitset<sizeof(unsigned long) * 8>(s);
		unsigned long temp = fileBuf.binary.to_ulong();
		fileBuf.buf <<= bits;
		fileBuf.beg += bits;
		fileBuf.buf |= temp;
		while (fileBuf.beg - fileBuf.end >= 8) {
			unsigned long temp2 = fileBuf.buf;
			temp2 >>= (fileBuf.beg - fileBuf.end - 8);
			unsigned char tempChar = (unsigned char)temp2;
			ofs.put(tempChar);
			fileBuf.beg -= 8;
		}
		if (byteCount % 1000 == 0) {
			cout << "\b\b\b\b\b\b\b\b";
		}
	}
	if (fileBuf.beg > fileBuf.end) {
		fileBuf.buf <<= (8 - (fileBuf.beg - fileBuf.end));
		char tempChar = char(fileBuf.buf);
		ofs.write(&tempChar, sizeof(char));
	}
	primitiveSizeByte = byteCount;
	primitiveSizeKB = primitiveSizeByte / 1024;
	ofs.seekp(wtf, ios::beg);
	ofs.write((char*)&primitiveSizeByte, sizeof(primitiveSizeByte));
	ifs.close();
	ofs.close();
}
void HuffmanCoding::decompress() {
	readHeader();
	Timer t;
	t.start();
	readBody();
	t.end();
	timeUsedToDecompress = t.getRunningTime_MS();
	primitiveSizeKB = primitiveSizeByte / 1024;
}
void HuffmanCoding::readHeader() {
	ifs.open(primitiveFilePath.c_str(), ios::in | ios::binary);
	ifs >> targetFilePath;
	targetFilePath += "_new";
	ifs.get();
	//cout << targetFilePath << endl;
	ifs.read((char*)&primitiveSizeByte, sizeof(primitiveSizeByte));
	//cout << primitiveSizeByte << endl;
	ifs.get();
	int dictSize;
	//map<char, string>::size_type dictSize;
	ifs.read((char*)&dictSize, sizeof(dictSize));
	ifs.get();
	if(dictSize > nAsciis){
		cout << "Damaged file." << endl;
		exit(0);
	}
	//cout << dictSize << endl;
	for (map<char, string>::size_type i = 0; i < dictSize; i++) {
		char key;
		string value;
		ifs.read(&key, sizeof(key));
		ifs.get();
		ifs >> value;
		ifs.get();
		codeDict[key] = value;
		decompressDict[value] = key;
	}
	/*
	for (map<char, string>::iterator it = codeDict.begin(); it != codeDict.end(); it++) {
		cout << it->first << '\t' << it->second << endl;
	}
	*/
}
void HuffmanCoding::readBody() {
	ofs.open(targetFilePath.c_str(), ios::out);
	unsigned long long byteCount = 0;
	cout << "Progress ";
	while (ifs.peek() != EOF) {
		if (byteCount % 1000 == 0) {
			cout << setw(6) << right << setprecision(2) << fixed << byteCount * 100.0 / primitiveSizeByte << " %";
		}
		unsigned char tempChar;
		unsigned long tempUL;
		while (fileBuf.beg - fileBuf.end < 3 * sizeof(char) * 8) {
			ifs.read((char*)&tempChar, sizeof(tempChar));
			//if (ifs.peek() == EOF && byteCount >= primitiveSizeByte) {
			//	break;
			//}
			tempUL = tempChar;
			fileBuf.buf <<= sizeof(char) * 8;
			fileBuf.buf |= tempUL;
			fileBuf.beg += sizeof(char) * 8;
		}
		fileBuf.binary = bitset<sizeof(unsigned long) * 8>(fileBuf.buf);
		string patten("");
		while (decompressDict.count(patten) == 0) {
			patten += char(fileBuf.binary[fileBuf.beg] + '0');
			fileBuf.beg--;
		}
		ofs.put(decompressDict[patten]);
		byteCount++;
		if (byteCount % 1000 == 0) {
			cout << "\b\b\b\b\b\b\b\b";
		}
	}
	while (byteCount < primitiveSizeByte && fileBuf.beg > fileBuf.end) {
		string patten("");
		while (decompressDict.count(patten) == 0 && fileBuf.beg > fileBuf.end) {
			patten += char(fileBuf.binary[fileBuf.beg] + '0');
			fileBuf.beg--;
		}
		ofs.put(decompressDict[patten]);
		byteCount++;
	}
	ifs.close();
	ofs.close();
}
void HuffmanCoding::showPerformance() {
	if (mod == c) {
		cout << endl;
		cout << "******************************************" << endl;
		cout << "File Path :" << primitiveFilePath << endl;
		cout << "Primitive Size(KB): " << primitiveSizeKB << endl;
		cout << "After compressing(KB) : " << compressedSizeKB << endl;
		cout << "Compress ratio: " << 1.0 * compressedSizeKB / primitiveSizeKB * 100 << "%" << endl;
		cout << "Time for Compressing(ms) : " << timeUsedToCompress << endl;
		cout << "******************************************" << endl;
	}
	else if (mod == d) {
		cout << endl;
		cout << "******************************************" << endl;
		cout << "File Path :" << primitiveFilePath << endl;
		cout << "Size after being decompressed(KB) : " << primitiveSizeKB << endl;
		cout << "Time for Decompressing(ms) : " << timeUsedToDecompress << endl;
		cout << "******************************************" << endl;
	}
	/*
	for (int i = 0; i < nAsciis; i++) {
		cout << char(i) << '\t' << freqSpace[i];
		cout << endl;
	}
	*/
}
void HuffmanCoding::close() {
	primitiveFilePath = "";
	targetFilePath = "";
	for (int i = 0; i < nAsciis; i++) {
		freqSpace[i] = 0;
	}
	timeUsedToCompress = 0;
	timeUsedToDecompress = 0;
	primitiveSizeKB = 0;
	compressedSizeKB = 0;
}
unsigned long long HuffmanCoding::calFileSize(string file) {
	ifs.open(file.c_str(), ios::in | ios::binary);
	if (!ifs) {
		cout << "File: " << file << " does not exist." << endl;
		exit(0);
	}
	ifs.seekg(0, ios::end);
	unsigned long long temp = ifs.tellg();
	ifs.close();
	return temp;
}
void HuffmanCoding::calFreq() {
	ifs.open(primitiveFilePath.c_str(), ios::in);
	if (!ifs) {
		cout << "File: " << primitiveFilePath << " does not exist." << endl;
		exit(0);
	}
	char ch;
	while (!ifs.eof()) {
		ifs.read(&ch, sizeof(char));
		//
		if(ch < 0){
			ch = '?';
		}
		//
		freqSpace[ch]++;
	}
	for (int i = 0; i < nAsciis; i++) {
		freqSpace[i] /= primitiveSizeByte;
	}
	ifs.close();
}
void HuffmanCoding::encode() {
	TreeNode* p = tree.getHead();
	string code("");
	recursiveEncode(p, code);
}
void HuffmanCoding::recursiveEncode(TreeNode* p, string code) {
	if (p->value == -1) {
		recursiveEncode(p->lchild, code + "0");
		recursiveEncode(p->rchild, code + "1");
	}
	else {
		p->stringCode = code;
		codeDict[p->value] = code;
	}
}
