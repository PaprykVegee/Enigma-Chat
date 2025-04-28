#ifndef ENIGMA_MODULE_H
#define ENIGMA_MODULE_H

#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

class Rotor {
private:
    int rotate_num;
    std::string alphabet_shuffled;
    std::string alphabet;
    int notch;

    std::string shuffle_alphabet(int seed);
    std::string genrate_alphabet();
    void rotate(int position = 1);

public:
    Rotor(int position, int rotor_num, int notch);

    int get_notch() const;
    std::string get_alphabet_shuffled();
    std::string get_alphabet();
    void init_rotate(bool rot);
    char encrypte(char letter);
    char decrypte(char letter);
};

class Reflector {
private:
    std::vector<int> wiring;

public:
    Reflector();
    char reflect(char letter);
};

class RecodeTable {
private:
    std::map<char, char> forward_map;
    std::map<char, char> reverse_map;

    static void add_recorde(std::map<char, char>& mappings);

public:
    RecodeTable(std::map<char, char> mappings = {});
    char encrypt(char letter);
    char decrypt(char letter);
};

class Enigma {
private:
    int counter;
    int notch;
    int pos1, pos2, pos3;
    Rotor rotor1, rotor2, rotor3;
    Reflector reflector;
    RecodeTable recorde_table;

    void rotate_Rotors();

public:
    Enigma(int r1, int r2, int r3, int pos1, int pos2, int pos3, int notch1, int notch2, int notch3,
           std::map<char, char> recorde_table = {});
    char encrypte(char letter);
};

#endif // ENIGMA_MODULE_H
