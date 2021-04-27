#include <iostream>
#include <fstream>
#include <queue>
#include <bitset>
#include <string>

#define BITS_PER_BYTE 8

struct code {
    char c;
    std::string s;
};

void decode(std::ifstream &fin, std::ofstream &fout) {
    char c;

    fin >> std::noskipws;

    fin >> c;
    int trail = (int)c - '0';

    std::vector<code> v;
    while (fin >> c) {
        if (c == '>' && fin.peek() == '>') break;
        code x;
        x.s = "";
        x.c = c;
        while (fin >> c && c != ' ') x.s += c;
        v.push_back(x);
    }

    std::string s = "";
    fin >> c;
    while (fin >> c) {
        std::bitset<BITS_PER_BYTE> b(c);
        std::string y = b.to_string();

        int l = fin.peek() == EOF ? BITS_PER_BYTE - trail : BITS_PER_BYTE;
        for (int i = 0; i < l; ++i) {
            s += y[i];
            auto a = std::find_if(v.begin(), v.end(), [&s](const code x){return x.s==s;});
            if (a != v.end()) {
                fout << a->c;
                s = "";
            }
        }      
    }
    
    std::vector<code>().swap(v);
}

int main () {
    std::string filename;
    std::cout << "File to be decompressed: ";
    std::getline(std::cin, filename);

    auto ext_pos = filename.find_last_of('.');
    std::string ext = filename.substr(ext_pos);
    if (ext != ".comp") {
        std::cerr << "Not a .comp file" << std::endl;
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

    std::string outfilename = filename.substr(0, ext_pos) + ".txt";
    std::ofstream fout(outfilename);

    decode(fin, fout);

    fin.close();
    fout.close();

    return 0;
}