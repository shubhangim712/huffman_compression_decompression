#include <iostream>
#include <fstream>
#include <queue>
#include <bitset>
#include <string>
#include <unordered_map>

#define BITS_PER_BYTE 8

class node {
    public:
        char c;
        int freq;
        node* left;
        node* right;

        node() {
            left = right = NULL;
        }

        node(char ch, int f) {
            c = ch;
            freq = f;
            left = right = NULL;
        }

        node(char ch, int f, node* l, node* r) {
            c = ch;
            freq = f;
            left = l;
            right = r;
        }

        bool operator > (const node n) const {
            return freq > n.freq;
        }
};

std::priority_queue<node, std::vector<node>, std::greater<node>> getFreq(std::ifstream &file) {
    std::unordered_map<char, int> freq;
    std::priority_queue<node, std::vector<node>, std::greater<node>> freqMaxHeap;

    char c;
    file >> std::noskipws;
    while (file >> c) ++(freq[c]);

    for (auto x : freq) freqMaxHeap.push(node(x.first, x.second));

    return freqMaxHeap;
}

// node generateHuffmanTree(std::priority_queue<node, std::vector<node>, decltype(comp)> *freqMaxHeap) {
//     while (freqMaxHeap->size() != 1) {
//         node* m = new node;
//         *m = freqMaxHeap->top();
//         freqMaxHeap->pop();
//         node* n = new node;
//         *n = freqMaxHeap->top();
//         freqMaxHeap->pop();
//         node* o = new node('\0', (*m).freq+(*n).freq, m, n);
//         freqMaxHeap->push(*o);
//     }

//     return freqMaxHeap->top();
// }

// void assign(const node* m, std::unordered_map<char, std::string>& dict, std::string code) {
//     if (!m) return;
//     if (!m->left && !m->right) dict[m->c] = code;
//     if (m->left) assign(m->left, dict, code+"0");
//     if (m->right) assign(m->right, dict, code+"1");
// }

// void encode(std::ifstream &fin, std::ofstream &fout, std::unordered_map<char, std::string>& dict) {
//     char c;
//     fin >> std::noskipws;
//     std::string s, sub = "";

//     fout << " ";
//     for (auto x : dict) fout << x.first << x.second << " ";
//     fout << ">>";

//     while (fin >> c) {
//         s = dict[c];
//         int sz = s.length();
//         int i = 0;
//         while (i < sz) {
//             int x = sub.length();                
//             if (sz - i + x >= BITS_PER_BYTE) {
//                 sub += s.substr(i, (BITS_PER_BYTE - x));
//                 i += (BITS_PER_BYTE - x);
//                 std::bitset<BITS_PER_BYTE> b(sub);
//                 fout << (char)b.to_ulong();
//                 sub = "";
//             }
//             else {
//                 sub += s.substr(i);
//                 i = sz;
//             }
//         }
//     }
//     if (sub != "") {
//         std::bitset<BITS_PER_BYTE> b(sub);
//         b = b << (BITS_PER_BYTE - sub.length());
//         fout << (char)b.to_ulong();
//     }

//     fout.seekp(0, std::ios::beg);
//     fout << (BITS_PER_BYTE - sub.length()) % BITS_PER_BYTE;
// }

int main () {
    std::string filename;
    std::cout << "Filename: ";
    std::getline(std::cin, filename);

    auto ext_pos = filename.find_last_of('.');
    std::string ext = filename.substr(ext_pos);
    if (ext != ".txt") {
        std::cerr << "Not a .txt file" << std::endl;
        exit(-1);
    }
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Error opening file" << std::endl;
        exit(-1);
    }
    if (fin.peek() == EOF) {
        std::cerr << "Empty file" << std::endl;
        exit(-1);
    }

    std::priority_queue<node, std::vector<node>, std::greater<node>> freqMaxHeap = getFreq(fin);

    // node huffmanTree = generateHuffmanTree(&freqMaxHeap);

    // std::unordered_map<char, std::string> dict;
    // assign(&huffmanTree, dict, "");

    // fin.clear();
    // fin.seekg(0, std::ios::beg);
    // std::string outfilename = filename.substr(0, ext_pos) + ".comp";
    // std::ofstream fout(outfilename);
    // encode(fin, fout, dict);
    // fin.close();
    // fout.close();

    return 0;
}