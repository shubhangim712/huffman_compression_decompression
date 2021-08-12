#include <iostream>
#include <fstream>
#include <queue>
#include <bitset>
#include <string>
#include <unordered_map>

#define BITS_PER_BYTE 8

class node {
    public:
        char c = '\0';
        int freq = 0;
        node* left = NULL;
        node* right = NULL;

        node() {}

        node(char ch, int f) {
            c = ch;
            freq = f;
        }

        node(int f, node* l, node* r) {
            freq = f;
            left = l;
            right = r;
        }

        bool operator > (const node n) const {
            return freq > n.freq;
        }
};

std::unordered_map<char, int> generateFreqMap(std::ifstream& file) {
    std::unordered_map<char, int> freqMap;
    char c;
    while (file.get(c)) ++(freqMap[c]);
    freqMap[EOF] = 1;

    return freqMap;
}

void assign(const node* m, std::unordered_map<char, std::string>& dict, std::string code) {
    if (!m) return;
    if (m->left) assign(m->left, dict, code+"0");
    if (m->right) assign(m->right, dict, code+"1");
    if (!m->left && !m->right) dict[m->c] = code;
}

std::vector<char> treeToVector(node* m) {
    std::queue<node*> toVisit;
    toVisit.push(m);

    std::vector<char> vectorizedTree;
    while (!toVisit.empty()) {
        node* temp = toVisit.front();
        if (temp->left) toVisit.push(temp->left);
        if (temp->right) toVisit.push(temp->right);
        toVisit.pop();
        char c = temp->c;
        vectorizedTree.push_back(c);
    }

    return vectorizedTree;
}

std::pair<std::unordered_map<char, std::string>, std::vector<char>> generateHuffmanTree(std::unordered_map<char, int> freqMap) {
    std::priority_queue<node, std::vector<node>, std::greater<node>> freqMinHeap;
    for (auto x : freqMap) freqMinHeap.push(node(x.first, x.second));

    while (freqMinHeap.size() != 1) {
        node* m = new node;
        *m = freqMinHeap.top();
        freqMinHeap.pop();
        node* n = new node;
        *n = freqMinHeap.top();
        freqMinHeap.pop();
        node* o = new node((*m).freq+(*n).freq, m, n);
        freqMinHeap.push(*o);
    }

    std::pair<std::unordered_map<char, std::string>, std::vector<char>> huffmanTree;
    node* root = new node;
    *root = freqMinHeap.top();
    assign(root, huffmanTree.first, "");
    huffmanTree.second = treeToVector(root);

    return huffmanTree;
}

void encode(std::ifstream& fin, std::pair<std::unordered_map<char, std::string>, std::vector<char>> huffmanTree, std::ofstream& fout, std::string ext) {
    fout << ext << " ";

    std::vector<char> HTvector = huffmanTree.second;
    fout << HTvector.size() << " ";
    for (auto x : HTvector) fout.put(x);

    char c;
    std::string s, sub = "";
    std::unordered_map<char, std::string> HTdict = huffmanTree.first;
    while (fin.get(c)) {
        s = HTdict[c];
        if (fin.peek() == EOF) s += HTdict[EOF];
        int sz = s.length();
        for (int i = 0; i < sz; ) {
            int x = sub.length();
            if (sz - i + x >= BITS_PER_BYTE) {
                sub += s.substr(i, (BITS_PER_BYTE - x));
                i += (BITS_PER_BYTE - x);
                std::bitset<BITS_PER_BYTE> b(sub);
                fout.put(b.to_ulong());
                sub = "";
            }
            else {
                sub += s.substr(i);
                i = sz;
            }
        }
    }
    if (sub != "") {
        std::bitset<BITS_PER_BYTE> b(sub);
        b = b << (BITS_PER_BYTE - sub.length());
        fout.put(b.to_ulong());
    }
}

bool compress(std::string filename) {
    std::string ext = filename.substr(filename.find_last_of('.'));
    if (ext != ".txt") return false;

    std::ifstream fin(filename);
    if (!fin) return false;
    
    if (fin.peek() == EOF) return false;

    std::unordered_map<char, int> freqMap = generateFreqMap(fin);
    std::pair<std::unordered_map<char, std::string>, std::vector<char>> huffmanTree = generateHuffmanTree(freqMap);

    std::ofstream fout(filename + ".cmp", std::ios::binary);

    fin.clear();
    fin.seekg(0, std::ios::beg);
    encode(fin, huffmanTree, fout, ext);

    fin.close();
    fout.close();

    return true;
}

int main () {
    std::string filename;
    std::cout << "Enter filename of the .txt file to be compressed: ";
    std::getline(std::cin, filename);

    if(!compress(filename)) std::cout << "Failed to compress" << std::endl;

    return 0;
}