#include "imgui.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <random>
#include <map>

// === GLOBALNE ZMIENNE INTERFEJSU ===
static char inputText[128] = "";
static char inputText1[128] = "";
static char inputText2[128] = "";
static char inputText3[128] = "";
static std::stringstream consoleHistory;  // Bufor historii komunikatów

// === FUNKCJA: Definiowanie danych interfejsu ===
void DefineUI()
{
    inputText[0] = '\0';
    consoleHistory.str("");  // Wyczyszczenie historii
    consoleHistory.clear();  // Resetowanie flagi
}

char** gen_alfabet_array() {
    char** alfabet = (char**)malloc(26 * sizeof(char*));
    for (int i = 0; i < 26; i++) {
        alfabet[i] = (char*)malloc(2);
        alfabet[i][0] = 'A' + i;
        alfabet[i][1] = '\0';
    }
    return alfabet;
}

std::map<char, char> Recorde_Table(int baseX, int baseY)
{
    static char** letters = gen_alfabet_array();
    static int selected_indices[26];
    static bool initialized = false;
    bool enable_array[26] = {true};

    for (int i = 0; i < 26; i++) {
        enable_array[i] = true;
    }

    std::string selected_letters = "";

    std::map<char, char> recorde_table_map;

    if (!initialized) {
        for (int i = 0; i < 26; i++) {
            selected_indices[i] = i;
        }
        initialized = true;
    }

    int currentLetter = 0;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 7; col++) {
            if (currentLetter < 26) {

                char original = 'A' + currentLetter;
                char selected = letters[selected_indices[currentLetter]][0];

                if (original != selected) {
                    recorde_table_map[original] = selected;
                    enable_array[selected_indices[currentLetter]] = false;
                    
                }

                if (enable_array[currentLetter]){
                    ImGui::PushID(currentLetter);

                    int offsetCol = col + (row == 3 ? 1 : 0);
                    float cellX = baseX + offsetCol * 150;
                    float cellY = baseY + row * 30; 

                    ImGui::SetCursorPos(ImVec2(cellX, cellY));
                    ImGui::Text("%s <->", letters[currentLetter]);

                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::SetCursorPos(ImVec2(cellX + 40, cellY));
                    ImGui::Combo("##combo", &selected_indices[currentLetter], (const char* const*)letters, 26);

                    ImGui::PopID();
                }
                currentLetter++;
            }
        }
    }
    return recorde_table_map;
}

void rotor(int seed, int& rotor_pos, int baseX, int baseY) {
    std::string alphabet;
    for (char letter = 'A'; letter <= 'Z'; letter++) {
        alphabet += letter;
    }

    std::mt19937 rng(seed);
    std::shuffle(alphabet.begin(), alphabet.end(), rng);

    // Wyświetlenie nagłówka
    ImGui::SetCursorPos(ImVec2(baseX, baseY));
    ImGui::Text("Rotor [%d]", rotor_pos);

    char prev = alphabet[(rotor_pos + 25) % 26];
    char curr = alphabet[rotor_pos % 26];
    char next = alphabet[(rotor_pos + 1) % 26];

    ImGui::SetCursorPos(ImVec2(baseX, baseY + 25));
    ImGui::Text("  %c", prev);

    ImGui::SetCursorPos(ImVec2(baseX, baseY + 45));
    ImGui::Text("> %c <", curr);

    ImGui::SetCursorPos(ImVec2(baseX, baseY + 65));
    ImGui::Text("  %c", next);

    ImGui::SetCursorPos(ImVec2(baseX, baseY + 90));
    if (ImGui::Button(("Obróć##" + std::to_string(baseX)).c_str())) {
        rotor_pos = (rotor_pos + 1) % 26;
    }
}
