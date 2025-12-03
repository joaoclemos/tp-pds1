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
    
    // --- IMAGENS GERAIS ---
    ALLEGRO_BITMAP* img_player;
    ALLEGRO_BITMAP* img_enemy_weak;   
    ALLEGRO_BITMAP* img_enemy_strong; 
    ALLEGRO_BITMAP* img_background; 
    ALLEGRO_BITMAP* img_base; 

    // --- IMAGENS DE FUNDO DE CARTAS / GENÉRICAS ---
    ALLEGRO_BITMAP* img_verso;     
    ALLEGRO_BITMAP* img_ataque;    
    ALLEGRO_BITMAP* img_defesa;    
    ALLEGRO_BITMAP* img_buff_gen;   
    ALLEGRO_BITMAP* img_debuff_gen; 

    // --- ÍCONES ESPECÍFICOS (TODOS) ---
    ALLEGRO_BITMAP* icon_forca;
    ALLEGRO_BITMAP* icon_vulneravel;
    ALLEGRO_BITMAP* icon_veneno;
    ALLEGRO_BITMAP* icon_cura;
    ALLEGRO_BITMAP* icon_supercura;
    ALLEGRO_BITMAP* icon_troca; 
    ALLEGRO_BITMAP* icon_sono;
    ALLEGRO_BITMAP* icon_fraqueza; // <--- NOVO ÍCONE

} Renderer;

void FillRenderer(Renderer* renderer);
void ClearRenderer(Renderer* renderer);

void Render(Renderer* renderer,
            GameState current_state,
            Player player,
            Enemy inimigos[],
            int carta_sel, int inimigo_sel, int selecionando_alvo);

#endif