//////////////////////////////////////////
////////////Create by PaLorenc////////////
//////////////////////////////////////////

#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "Enigma_Module.h"

// Rotor class implementation
Rotor::Rotor(int position, int rotor_num, int notch) {
    // Inicjalizuje wirnik z pozycją startową, numerem i notch'em
    this->alphabet_shuffled = shuffle_alphabet(rotor_num);  // Mieszanie alfabetu
    this->alphabet = genrate_alphabet();  // Generowanie alfabetu
    this->notch = notch;  // Ustalenie pozycji notch'a
    rotate(position);  // Obracanie wirnika na wybraną pozycję
}

/*
Opis:
    Funkcja generuje pomieszany alfabet na podstawie zadanego seeda
@param
    int seed - liczba, na podstawie której zostanie pomieszany alfabet
@return
    std::string - pomieszany alfabet
*/
std::string Rotor::shuffle_alphabet(int seed) {
    std::string alphabet;
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        alphabet += letter;  // Dodanie liter do alfabetu
    }
    std::mt19937 rng(seed);  // Inicjalizacja generatora liczb losowych
    std::shuffle(alphabet.begin(), alphabet.end(), rng);  // Mieszanie alfabetu
    return alphabet;  // Zwracamy pomieszany alfabet
}

/*
Opis:
    Funkcja generuje pełny alfabet
@param
    Brak
@return
    std::string - pełny alfabet
*/
std::string Rotor::genrate_alphabet() {
    std::string alphabet;
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        alphabet += letter;  // Dodanie liter do alfabetu
    }
    return alphabet;  // Zwrócenie pełnego alfabetu
}

/*
Opis:
    Funkcja obraca wirnik na określoną pozycję
@param
    int position - pozycja, na którą wirnik ma zostać obrócony
@return
    Brak
*/
void Rotor::rotate(int position) {
    std::rotate(this->alphabet_shuffled.rbegin(),
                this->alphabet_shuffled.rbegin() + position,
                this->alphabet_shuffled.rend());  // Obracanie wirnika o określoną liczbę pozycji
}

/*
Opis:
    Funkcja zwraca wartość notch'a (pozycja w której wirnik zaczyna obracać kolejny)
@param
    Brak
@return
    int - pozycja notch'a
*/
int Rotor::get_notch() const {
    return this->notch;  // Zwrócenie wartości notch'a
}

/*
Opis:
    Funkcja zwraca pomieszany alfabet
@param
    Brak
@return
    std::string - pomieszany alfabet
*/
std::string Rotor::get_alphabet_shuffled() {
    return this->alphabet_shuffled;  // Zwracanie pomieszanego alfabetu
}

/*
Opis:
    Funkcja zwraca pełny alfabet
@param
    Brak
@return
    std::string - pełny alfabet
*/
std::string Rotor::get_alphabet() {
    return this->alphabet;  // Zwracanie pełnego alfabetu
}

/*
Opis:
    Funkcja inicjuje obrót wirnika (jeśli rot == true)
@param
    bool rot - flaga określająca, czy wirnik ma zostać obrócony
@return
    Brak
*/
void Rotor::init_rotate(bool rot) {
    if (rot) rotate();  // Obrót wirnika, jeśli rot == true
}

/*
Opis:
    Funkcja do szyfrowania litery
@param
    char letter - litera, która ma zostać zaszyfrowana
@return
    char - zaszyfrowana litera
*/
char Rotor::encrypte(char letter) {
    auto it = std::find(this->alphabet.begin(), this->alphabet.end(), letter);
    if (it != this->alphabet.end()) {
        int idx = std::distance(this->alphabet.begin(), it);  // Znalezienie indeksu litery w alfabecie
        return this->alphabet_shuffled[idx];  // Zwrócenie zaszyfrowanej litery
    }
    std::cout << "Nie znaleziono znaku";  // Obsługa błędu w przypadku braku litery
    return 0;
}

/*
Opis:
    Funkcja do deszyfrowania litery
@param
    char letter - litera, która ma zostać odszyfrowana
@return
    char - odszyfrowana litera
*/
char Rotor::decrypte(char letter) {
    auto it = std::find(this->alphabet_shuffled.begin(), this->alphabet_shuffled.end(), letter);
    if (it != this->alphabet_shuffled.end()) {
        int idx = std::distance(this->alphabet_shuffled.begin(), it);  // Znalezienie indeksu litery w pomieszanym alfabecie
        return this->alphabet[idx];  // Zwrócenie odszyfrowanej litery
    }
    std::cout << "Nie znaleziono znaku";  // Obsługa błędu w przypadku braku litery
    return 0;
}

// Reflector class implementation
Reflector::Reflector() {
    wiring.resize(26);  // Tworzenie wektora dla 26 liter
    for (int i = 0; i < 13; ++i) {
        wiring[i] = 25 - i;  // Odbicie 1-13 liter
        wiring[25 - i] = i;  // Odbicie 14-26 liter
    }
}

/*
Opis:
    Funkcja zwraca odbicie liter (znak naprzeciwko według ustawienia na okręgu)
@param
    char letter - litera przed odbiciem
@return
    char - litera po odbiciu
*/
char Reflector::reflect(char letter) {
    int idx = letter - 'A';  // Obliczanie indeksu litery
    if (idx < 0 || idx >= 26) return letter;  // Sprawdzanie, czy litera jest poprawna
    return wiring[idx] + 'A';  // Zwrócenie odbitej litery
}

// RecodeTable class implementation
void RecodeTable::add_recorde(std::map<char, char>& mappings) {
    std::map<char, char> tempMappings = mappings; 
    for (const auto& pair : mappings) {
        tempMappings[pair.second] = pair.first;  // Tworzenie odwrotnej mapy
    }
    mappings = tempMappings;  // Aktualizacja mapy
}

RecodeTable::RecodeTable(std::map<char, char> mappings) {
    add_recorde(mappings);  // Dodanie odwrotnej mapy
    for (const auto& pair : mappings) {
        char from = std::toupper(pair.first);  // Przekształcanie liter na wielkie
        char to = std::toupper(pair.second);  // Przekształcanie liter na wielkie
        forward_map[from] = to;  // Mapa szyfrowania
        reverse_map[to] = from;  // Mapa deszyfrowania
    }
}

/*
Opis:
    Funkcja szyfrująca literę
@param
    char letter - litera do zaszyfrowania
@return
    char - zaszyfrowana litera
*/
char RecodeTable::encrypt(char letter) {
    letter = std::toupper(letter);  // Przekształcanie litery na wielką
    if (forward_map.count(letter)) {
        return forward_map[letter];  // Zwrócenie zaszyfrowanej litery
    }
    return letter;  // Zwrócenie litery bez zmian, jeśli nie ma jej w mapie
}

/*
Opis:
    Funkcja deszyfrująca literę
@param
    char letter - litera do odszyfrowania
@return
    char - odszyfrowana litera
*/
char RecodeTable::decrypt(char letter) {
    letter = std::toupper(letter);  // Przekształcanie litery na wielką
    if (reverse_map.count(letter)) {
        return reverse_map[letter];  // Zwrócenie odszyfrowanej litery
    }
    return letter;  // Zwrócenie litery bez zmian, jeśli nie ma jej w mapie
}

// Enigma class implementation
Enigma::Enigma(int r1, int r2, int r3, int pos1, int pos2, int pos3, int notch1, int notch2, int notch3,
               std::map<char, char> recorde_table) :
    rotor1(pos1, r1, notch1), rotor2(pos2, r2, notch2), rotor3(pos3, r3, notch3), reflector(), recorde_table(recorde_table) {
    this->pos1 = pos1;
    this->pos2 = pos2;
    this->pos3 = pos3;
    this->counter = 0;
}

/*
Opis:
    Funkcja obracająca wirniki na odpowiednie pozycje w zależności od liczby obrotów
@param
    Brak
@return
    Brak
*/
void Enigma::rotate_Rotors() {
    rotor1.init_rotate(true);
    
    if ((pos1 + counter) % 26 == rotor1.get_notch()) {
        rotor2.init_rotate(true);
        if ((pos2 + counter / 26) % 26 == rotor2.get_notch()) {
            rotor3.init_rotate(true);
        }
    }
}

/*
Opis:
    Funkcja szyfrująca literę przy użyciu wszystkich komponentów Enigmy
@param
    char letter - litera do zaszyfrowania
@return
    char - zaszyfrowana litera
*/
char Enigma::encrypte(char letter) {

    if ((letter >= 'A' && letter <= 'Z')){
        rotate_Rotors();
        letter = recorde_table.encrypt(letter);  // Szyfrowanie tabelą rekodowania
        letter = rotor1.encrypte(letter);  // Szyfrowanie wirnikiem 1
        letter = rotor2.encrypte(letter);  // Szyfrowanie wirnikiem 2
        letter = rotor3.encrypte(letter);  // Szyfrowanie wirnikiem 3

        letter = reflector.reflect(letter);  // Odbicie

        letter = rotor3.decrypte(letter);  // Deszyfrowanie wirnikiem 3
        letter = rotor2.decrypte(letter);  // Deszyfrowanie wirnikiem 2
        letter = rotor1.decrypte(letter);  // Deszyfrowanie wirnikiem 1
        letter = recorde_table.decrypt(letter);  // Deszyfrowanie tabelą rekodowania

        ++counter;
    }
    return letter;  // Zwrócenie zaszyfrowanej litery
}



/*
    int main() {
        int pos1 = 1;
        int pos2 = 20;
        int pos3 = 13;
    
        int notch1 = 20;
        int notch2 = 20;
        int notch3 = 20;
    
        std::map<char, char> my_recode_table = {{'B','A'}, {'C', 'D'}};
    
        Enigma enigma1(1, 2, 3, pos1, pos2, pos3, notch1, notch2, notch3, my_recode_table);
    
        std::string word = "mam nadzieje ze jutro bedzie ladna pogoda to byla bardzo dziwna sytuacja ale jakos sobie poradzilismy kiedy ostatnio widziales swojego przyjaciela z dziecinstwa";
        std::string encode_word;
        char letter;
    
        for (char c : word){
            c = std::toupper(c);
            if (c == ' '){
                encode_word += c;
            } else {
                letter = enigma1.encrypte(c);
                encode_word += letter;
            }
        }
    
        std::cout << "Zaszyfrowane: " << encode_word << '\n';
    
        // TUTAJ poprawka:
        Enigma enigma2(1, 2, 3, pos1, pos2, pos3, notch1, notch2, notch3, my_recode_table);
    
        std::string decode_word;
    
        for (char c : encode_word){
            c = std::toupper(c);
            if (c == ' '){
                decode_word += c;
            } else {
                letter = enigma2.encrypte(c);
                decode_word += letter;
            }
        }
    
        std::cout << "Odszyfrowane: " << decode_word << '\n';
    
        return 0;
    }
*/
    