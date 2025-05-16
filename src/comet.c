#include "comet.h"
#include "game.h"

// Fonction appelée à chaque frame pour faire tomber la comète
void cometfall(Comet *comet, Player *player) {
    // Déplace la comète vers le bas en fonction de sa vitesse
    comet->rect.y += comet->velocity;

    // Si la comète entre en collision avec le joueur
    if (CheckCollision(&comet->rect, &player->rect)) {
        comet->is_comet_active = false;          // La comète disparaît (désactivée)
        player->DamagePlayer(player, 20);        // Le joueur subit des dégâts
    }

    // Si la comète sort de l'écran vers le bas (valeur arbitraire de 400 ici)
    if (comet->rect.y > 400) {
        comet->is_comet_active = false;          // On la désactive (hors de l'écran)
    }
}


// Réinitialise la position d'une comète en haut de l'écran
void ResetComet(Comet *comet) {
    comet->rect.x = rand() % SCREEN_WIDTH;   // Position X aléatoire
    comet->rect.y = rand() % 80;             // Position Y aléatoire en haut
    comet->is_comet_active = true;           // On la rend active
}


// Initialise une comète avec texture, position, vitesse, etc.
void initComet(Comet *comet, SDL_Renderer *render, const char *imgpath) {
    comet->rect.x = rand() % SCREEN_WIDTH;       // Position X aléatoire
    comet->rect.y = rand() % 80;                 // Position Y aléatoire (en haut de l’écran)
    comet->rect.w = 100;                         // Largeur de la comète
    comet->rect.h = 100;                         // Hauteur de la comète
    comet->texture = IMG_LoadTexture(render, imgpath);  // Chargement de la texture
    comet->damage = 20;                          // Dégâts infligés
    comet->velocity = 2;                         // Vitesse de chute
    comet->is_comet_active = true;               // Comète active à l'initialisation
    comet->cometFall = cometfall;                // On assigne la fonction de chute
}

