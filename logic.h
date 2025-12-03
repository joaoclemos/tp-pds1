#ifndef LOGIC_H
#define LOGIC_H
#include "game_structs.h"

void draw_cards(Player* player, int n);
void start_player_turn(Player* player);
int play_card(Player* player, int card_index, Enemy* target);
void execute_enemy_turn(Player* player, Enemy inimigos[]);

#endif