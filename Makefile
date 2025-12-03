# Define o nome do arquivo final
TARGET := game.out

# ADICIONE SEUS NOVOS ARQUIVOS .C AQUI
SRC    := main.c utils.c renderer.c setup.c logic.c

# --- Configuração do Allegro (copiado do devkit) ---
PKGCONF := pkg-config
ALLEGRO_PKGS := allegro-5 allegro_main-5 allegro_image-5 allegro_primitives-5 \
                allegro_font-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5

# Flags de compilação: -g (debug) e bibliotecas
CFLAGS   := -g -lm $(shell $(PKGCONF) --cflags $(ALLEGRO_PKGS))
# Flags de biblioteca: Onde encontrar as bibliotecas do Allegro
LDLIBS   := $(shell $(PKGCONF) --libs   $(ALLEGRO_PKGS))

# --- Regras (Targets) ---

# 1. REGRA PRINCIPAL: Faz com que "make game" compile o $(TARGET)
game: $(TARGET)
	@echo "--- Compilacao concluida. Execute: ./$(TARGET) ---"

# 2. REGRA DE COMPILAÇÃO: Como criar o game.out
# $^ significa "todos os arquivos em SRC"
# $@ significa "o nome do alvo (game.out)"
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

# 3. REGRA DE LIMPEZA
clean:
	rm -f $(TARGET)
	rm -f *.out