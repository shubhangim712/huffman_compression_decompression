#include <iostream>
#include <fstream>
#include <queue>
#include <bitset>
#include <string>

#define BITS_PER_BYTE 8

class node {
    public:
        char c = '\0';
        node* left = NULL;
        node* right = NULL;

        node(char c) {
            this->c = c;
        }
};

std::string getExt(std::filebuf& fin) {
    std::string ext = "";
    for(char c = fin.sbumpc(); c != ' '; c = fin.sbumpc()) ext += c;
    return ext;
}

std::vector<char> extractHTvector(std::filebuf& fin) {
    std::string num = "";
    for(char c = fin.sbumpc(); c != ' '; c = fin.sbumpc()) num += c;
    int n = stoi(num);

    std::vector<char> v(n);
    for (int i = 0; i < n; ++i) v[i] = fin.sbumpc();

    return v;
}

node* generateHuffmanTree(std::vector<char> HTvector) {
    int n = HTvector.size();
    node* root = new node(HTvector[0]);
    if (n == 1) return root;

    int i = 1;
    std::queue<node*> parent;
    parent.push(root);

    while (i < n) {
        node* temp = parent.front();
        parent.pop();
        node* l = new node(HTvector[i]);
        temp->left = l;
        if (HTvector[i++] == '\0') parent.push(l);
        node* r = new node(HTvector[i]);
        temp->right = r;
        if (HTvector[i++] == '\0') parent.push(r);
    }

    return root;
}

node* getHuffmanTree(std::filebuf& fin) {
    std::vector<char> HTvector = extractHTvector(fin);

    node* huffmanTree = generateHuffmanTree(HTvector);

    return huffmanTree;
}

void decode(std::filebuf& fin, node* huffmanTree, std::ofstream& fout) {
    std::string op = "";

    node* it = huffmanTree;
    bool flag = true;

    while (flag) {
        std::bitset<BITS_PER_BYTE> b(fin.sbumpc());
        int i = BITS_PER_BYTE - 1;
        while (i > -1) {
            while (it->left && i > -1) it = !b[i--] ? it->left : it->right;
            if (!it->left) {
                if (it->c == -1) {
                    flag = false;
                    break;
                }
                fout << it->c;
                it = huffmanTree;
            }
        }
    }
}

bool decompress(std::string filename) {
    if (filename.substr(filename.find_last_of('.')) != ".cmp") return false;

    std::filebuf fin;
    if (!fin.open(filename, std::ios::binary|std::ios::in)) return false;

    if (fin.sgetc() == EOF) return false;

    std::string ext = getExt(fin);
    node* huffmanTree = getHuffmanTree(fin);

    std::string outfilename = filename + ext;
    std::ofstream fout(outfilename);

    decode(fin, huffmanTree, fout);

    fin.close();
    fout.close();

    return true;
}

int main () {
    std::string filename;
    std::cout << "Enter the name of the file to be decompressed: ";
    std::getline(std::cin, filename);

    if(!decompress(filename)) std::cout << "Failed to decompress" << std::endl;

    return 0;
}