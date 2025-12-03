#ifndef RENDERER_H
#define RENDERER_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h> 
#include "constants.h"
#include "game_structs.h"

typedef struct {
    ALLEGRO_DISPLAY* display;
    ALLEGRO_FONT* font;
    
    // --- IMAGENS DE PERSONAGENS E CENÁRIO ---
    ALLEGRO_BITMAP* img_player;
    ALLEGRO_BITMAP* img_enemy_weak;   
    ALLEGRO_BITMAP* img_enemy_strong; 
    ALLEGRO_BITMAP* img_background; 
    ALLEGRO_BITMAP* img_base; // Base de pedra

    // --- IMAGENS DE CARTAS ---
    ALLEGRO_BITMAP* img_verso;     // Verso da carta (para os montes)
    ALLEGRO_BITMAP* img_ataque;    // Frente Ataque
    ALLEGRO_BITMAP* img_defesa;    // Frente Defesa
    ALLEGRO_BITMAP* img_especial;  // Frente Especial
    ALLEGRO_BITMAP* img_buff;      // Frente Buffs
    ALLEGRO_BITMAP* img_debuff;    // Frente Debuffs

} Renderer;

void FillRenderer(Renderer* renderer);
void ClearRenderer(Renderer* renderer);

void Render(Renderer* renderer,
            GameState current_state,
            Player player,
            Enemy inimigos[],
            int carta_sel, int inimigo_sel, int selecionando_alvo);

#endif