#include <iostream>
#include <stdio.h>
#include <queue>
#include <cstring>
#include <string>
#include<fstream>
#include <vector>
using namespace std;

class NodeData{
public:
    int frequency;
    char character;
    NodeData *left;
    NodeData *right;

    NodeData(){
        frequency=0;
        left= NULL;
        right= NULL;
    }
    NodeData(int freq, char character){
        this->frequency= freq;
        this->character= character;
        this->left= NULL;
        this->right= NULL;
    }
};

void heapify(vector <NodeData*> &heap, int size, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < size && heap[left]->frequency < heap[smallest]->frequency)
        smallest = left;
    if (right < size && heap[right]->frequency < heap[smallest]->frequency)
        smallest = right;
    if (smallest != i) {
        NodeData *temp = heap[smallest];
        heap[smallest] = heap[i];
        heap[i] = temp;
        heapify(heap, size, smallest);
    }
}

void createMinHeap(vector <NodeData*> &heap, int size) {
    for (int i = (size / 2) - 1; i >= 0; i--)
    heapify(heap, size, i);
}

NodeData * deleteRootNode(vector <NodeData*> &heap) {
    NodeData *temp= heap[0];
    heap[0] = heap[heap.size()-1];
    heap.pop_back();
    heapify(heap, heap.size(), 0);
    return temp;
}

void addTwo(vector <NodeData*> &heap){
    NodeData *node1= deleteRootNode(heap);
    NodeData *node2= deleteRootNode(heap);
    NodeData *newNode= new NodeData(node1->frequency+ node2->frequency, '?');
    newNode->left= node1;
    newNode->right= node2;
    heap.push_back(newNode);

    for (int i = (heap.size() - 2) / 2; i > 0; i = (i - 1) / 2)
        heapify(heap, heap.size(), i);
    heapify(heap, heap.size(), 0);
    
}
void setFreq(string text_str, int *freqArray){
    for(int i=0; i< text_str.length(); i++){
        freqArray[text_str[i]]++;
    }
}

void addFrequencyToHeap(vector <NodeData*> &heap, int *freqArray){
    for(int i=0; i<256; i++){
        if(freqArray[i]!=0)
            heap.push_back(new NodeData(freqArray[i], i));
    }
}

string removeExtraLines(string fileText){
    string newText="";
    for(int i=0; i<fileText.length(); i++){
        if(fileText[i]!=13)
            newText= newText + fileText[i];
    }
    return newText;
}

void createHuffmanTree(vector <NodeData*> &heap){
    while(heap.size()>1)
        addTwo(heap);
}

string decodeData(NodeData* root, string str){
    NodeData *temp= root;
    string decodeData="";
    for(int i=0; i<str.length(); i++){   
        if(temp!=NULL){
            if(temp->right==NULL && temp->left==NULL){
                decodeData+= temp->character; 
                temp=root;
            }
            if(str[i]=='0')
                temp=temp->left;
            else if(str[i]=='1')
            temp= temp->right;
        }
        else
            i--;
    }
    if(temp->right==NULL && temp->left==NULL){
        decodeData+= temp->character;  
        temp=root;
    }
    return decodeData;
}

void encodeData(NodeData* root, string HuffmanCodes[], string code=""){
    if(root==NULL)
        return;
    if(root->left ==NULL && root->right==NULL){
        HuffmanCodes[root->character]= code;
    }

    encodeData(root->left, HuffmanCodes, code+"0");
    encodeData(root->right, HuffmanCodes, code+"1");
}

string textToBinaryCode(string fileText, string HuffmanCodes[]){
    string binaryCode= "";  
    for(int i=0; i<fileText.length(); i++)
            binaryCode+=HuffmanCodes[fileText[i]];
        return binaryCode;
}

union binManipulate {
    unsigned char character; //8 bits
    struct {
        unsigned char b0 : 1;
        unsigned char b1 : 1;
        unsigned char b2 : 1;
        unsigned char b3 : 1;
        unsigned char b4 : 1;
        unsigned char b5 : 1;
        unsigned char b6 : 1;
        unsigned char b7 : 1;
    };
};

void setBits(binManipulate& d) {
    d.b0 = 0;
    d.b1 = 0;
    d.b2 = 0;
    d.b3 = 0;
    d.b4 = 0;
    d.b5 = 0;
    d.b6 = 0;
    d.b7 = 0;
}

void writeBinaryDataToFile(string dataToWrite, FILE* fPtr) {
    binManipulate d;
    size_t sizeOfData = dataToWrite.size();
    bool flag = false;
    if (fPtr == NULL) {
        cout << "Not opened" << endl;
    }
    for (int i = 0; i < sizeOfData; 1) {
        setBits(d);
        d.b0 = (dataToWrite[i++] == '1') ? 1 : 0;
        if (i == sizeOfData) { flag = true; break; }
        d.b1 = (dataToWrite[i++] == '1') ? 1 : 0;
        if (i == sizeOfData) { flag = true; break; }
        d.b2 = (dataToWrite[i++] == '1') ? 1 : 0;
        if (i == sizeOfData) { flag = true; break; }
        d.b3 = (dataToWrite[i++] == '1') ? 1 : 0;
        if (i == sizeOfData) { flag = true; break; }
        d.b4 = (dataToWrite[i++] == '1') ? 1 : 0;
        if (i == sizeOfData) { flag = true; break; }
        d.b5 = (dataToWrite[i++] == '1') ? 1 : 0;
        if (i == sizeOfData) { flag = true; break; }
        d.b6 = (dataToWrite[i++] == '1') ? 1 : 0;
        if (i == sizeOfData) { flag = true; break; }
        d.b7 = (dataToWrite[i++] == '1') ? 1 : 0;
        if (i == sizeOfData) { flag = true; break; }
        fwrite(&d.character, sizeof(char), 1, fPtr);
    }
    if (flag) {
        fwrite(&d.character, sizeof(char), 1, fPtr);
    }
    return;
}

string readBinaryDataFromFile(FILE* fPtr) {
    binManipulate d;
    setBits(d);
    char temp;
    string binaryData;
    if (fPtr == NULL)
        cout << "file not opening" << endl;
    else {
        while (fread(&temp, sizeof(char), 1, fPtr) != 0) {
            d.character = temp;
            binaryData += (d.b0 ? '1' : '0');
            binaryData += (d.b1 ? '1' : '0');
            binaryData += (d.b2 ? '1' : '0');
            binaryData += (d.b3 ? '1' : '0');
            binaryData += (d.b4 ? '1' : '0');
            binaryData += (d.b5 ? '1' : '0');
            binaryData += (d.b6 ? '1' : '0');
            binaryData += (d.b7 ? '1' : '0');
            setBits(d);
        }
    }
    return binaryData;
}

void readInformationAndBinaryFromFile(FILE* fPtr, int& size, int* freqArray, string& data) {
    fread(&size, sizeof(int), 1, fPtr);
    fread(freqArray, sizeof(int), 256, fPtr);
    data = readBinaryDataFromFile(fPtr);

    int sizeOfBinaryReadData = 0;
    for (int i = 0; i < data.size(); i++) {
        sizeOfBinaryReadData++;
    }
    int diff = sizeOfBinaryReadData - size;
    for (int i = sizeOfBinaryReadData - 1; i >= size; i--) {
        data.pop_back();
    }
}

void writeInformationAndBinaryToFile(FILE* fPtr, int& size, int* freqArray, const string& data) {
    fwrite(&size, sizeof(int), 1, fPtr);
    fwrite(freqArray, sizeof(int), 256, fPtr);
    writeBinaryDataToFile(data, fPtr);
}

void createDecompressedFile(string text){
    fstream FileName;        

    FileName.open("MyFiles/decompressedFile.txt", ios::out);    
    if (!FileName){                 
        cout<<"Error while creating the file";    
    }
    else{
        FileName << text;   
        FileName.close();           
    }
}

string compressFile(string fileText) {
  int freqArray[256]={0};
  fileText= removeExtraLines(fileText);
  setFreq(fileText, freqArray);

  vector <NodeData*> heap;
  addFrequencyToHeap(heap, freqArray);
  createMinHeap(heap, heap.size());
  createHuffmanTree(heap);

  string HuffmanCodes[256];
  NodeData* root= heap[0];
  encodeData(root, HuffmanCodes);
                    
  string binaryCodedData= textToBinaryCode(fileText, HuffmanCodes);
   
  FILE* writeFptr;
  errno_t err = fopen_s(&writeFptr, "MyFiles/compressedFile.txt", "wb");
  int sizeOfTextInFile = binaryCodedData.size();
  writeInformationAndBinaryToFile(writeFptr, sizeOfTextInFile, freqArray, binaryCodedData);
  fclose(writeFptr);
  return "Compressed file created: File Compressor\\build-dev\\bin\\MyFiles";
}

string decompressFile(string fileName){
    FILE* fptr2;
    fileName= "MyFiles/" +fileName;
    const int length = fileName.length();  
    char* fileNameCharArray = new char[length + 1]; 
    strcpy(fileNameCharArray, fileName.c_str());
    errno_t err2 = fopen_s(&fptr2, fileNameCharArray, "rb");
    int size;
    int freqArray2[256];
    string newData;
    readInformationAndBinaryFromFile(fptr2, size, freqArray2, newData);
    fclose(fptr2);

    vector <NodeData*> heap;
    addFrequencyToHeap(heap, freqArray2);
    createMinHeap(heap, heap.size());
    createHuffmanTree(heap);
    NodeData* root= heap[0];

    string decodedData= decodeData(root, newData);
    
    createDecompressedFile(decodedData);
    return "Decompressed file created: File Compressor\\build-dev\\bin\\MyFiles";
}
int main() {
    cout << "Huffman Compressor Running..." << endl;

    string text = "hello world this is test";

    string result = compressFile(text);
    cout << result << endl;

    return 0;
}