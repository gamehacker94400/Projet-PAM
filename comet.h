#ifndef COMET_H
#define COMET_H

#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>

// Déclaration anticipée de la structure Player pour éviter les dépendances circulaires
typedef struct Player Player;

// Définition de la structure représentant une comète
typedef struct Comet {
    SDL_Rect rect;               // Rectangle définissant la position et la taille de la comète
    SDL_Texture *texture;        // Texture de l'image de la comète
    bool is_comet_active;        // Indique si la comète est active (visible et en chute)
    int damage, velocity;        // Dégâts infligés par la comète et sa vitesse de chute

    // Pointeur vers la fonction qui fait tomber la comète et gère les collisions
    void (*cometFall)(struct Comet *, Player *);

    // Pointeur vers la fonction qui réinitialise la comète (ex: repositionnement)
    void (*resetComet)(struct Comet *);
} Comet;

// Initialise une comète : charge sa texture, configure sa position, etc.
void initComet(Comet *, SDL_Renderer *, const char *);

#endif

