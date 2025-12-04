#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdlib.h>

// Verifica inicialização
void must_init(_Bool test, const char* description);
// Limpa teclas
void ClearKeyboardKeys(unsigned char* keyboard_keys);

#endif