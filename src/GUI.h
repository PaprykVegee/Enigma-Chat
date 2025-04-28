#ifndef GUI_H
#define GUI_H

void DefineUI();

char** gen_alfabet_array();

std::map<char, char> Recorde_Table(int baseX = 50, int baseY = 20);

void rotor(int seed, int& rotor_pos, int baseX, int baseY);

#endif