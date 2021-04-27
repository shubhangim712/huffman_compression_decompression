#include <iostream>
#include <fstream>
#include <queue>
#include <bitset>
#include <string>

#define BITS_PER_BYTE 8

class basic_node {
    public:
        char c;
        int freq;

        basic_node(char ch, int f) {
            c = ch;
            freq = f;
        }
};

class node {
    public:
        char c;
        int freq;
        node* left;
        node* right;
        std::string s;

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

        node(basic_node n) {
            c = n.c;
            freq = n.freq;
            left = right = NULL;
        }
};

bool operator<(node const &a, node const &b) {
        return a.freq > b.freq;
}

struct code {
    char c;
    std::string s;
};

std::priority_queue<node> getFreq(std::ifstream &file) {
    char c;
    int count = 0;
    std::vector<basic_node> freq;
    std::priority_queue<node> q;
    file >> std::noskipws;
    while (file >> c) {
        ++count;
        auto a = std::find_if(freq.begin(), freq.end(), [&c](const node x){return x.c == c;});
        if (a != freq.end()) ++(a->freq);
        else {
            basic_node n(c, 1);
            freq.push_back(n);
        }
    }

    for (auto a: freq) q.push(a);

    std::vector<basic_node>().swap(freq);

    return q;
}

void huffmanTree(std::priority_queue<node> *q) {
    while ((*q).size() != 1) {
        node* m = new node;
        *m = (*q).top();
        (*q).pop();
        node* n = new node;
        *n = (*q).top();
        (*q).pop();
        node* o = new node('\0', (*m).freq+(*n).freq, m, n);
        (*q).push(*o);
    }
}

void assign(const node* m, std::vector<code>* dict) {
    if (m == NULL) return;

    if ((*m).left != NULL) (*m).left->s = (*m).s + "0";
    assign((*m).left, dict);

    if ((*m).left == NULL && (*m).right == NULL) {
        code c = {(*m).c, (*m).s};
        dict->push_back(c);
    }
        
    if ((*m).right != NULL) (*m).right->s = (*m).s + "1";
    assign((*m).right, dict);
}

void encode(std::ifstream &fin, std::ofstream &fout, std::vector<code> dict) {
    char c;
    fin >> std::noskipws;
    std::string s, sub = "";

    fout << " ";
    for (auto a: dict) {
        fout << a.c << a.s << " ";
    }
    fout << ">>";

    while (fin >> c) {
        auto a = std::find_if(dict.begin(), dict.end(), [&c](const code x) {return x.c == c;});
        s = a->s;
        int sz = s.length();
        int i = 0;
        while (i < sz) {
            int x = sub.length();                
            if (sz - i + x >= BITS_PER_BYTE) {
                sub += s.substr(i, (BITS_PER_BYTE - x));
                i += (BITS_PER_BYTE - x);
                std::bitset<BITS_PER_BYTE> b(sub);
                fout << (char)b.to_ulong();
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
        fout << (char)b.to_ulong();
    }

    fout.seekp(0, std::ios::beg);
    fout << (BITS_PER_BYTE - sub.length()) % BITS_PER_BYTE;
}

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

    std::priority_queue<node> q = getFreq(fin);

    huffmanTree(&q);

    std::vector<code> dict;
    assign(&(q.top()), &dict);

    fin.clear();
    fin.seekg(0, std::ios::beg);
    std::string outfilename = filename.substr(0, ext_pos) + ".comp";
    std::ofstream fout(outfilename);
    encode(fin, fout, dict);
    fin.close();
    fout.close();

    std::vector<code>().swap(dict);

    return 0;
}