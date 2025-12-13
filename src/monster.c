#include "monster.h"
#include "player.h"
#include "game.h"

// Fait avancer le monstre vers la gauche tant qu’il ne touche pas le joueur.
// Si une collision est détectée, le monstre inflige des dégâts au joueur. 
void forward(Monster *monster, Player *player)
{
    // Si aucune collision n'est détectée entre le monstre et le joueur, déplacer le monstre
    if (!CheckCollision(&monster->rect, &player->rect))
    {
        monster->rect.x -= monster->velocity;  // Déplacement vers la gauche
    }
    else
    {
        // Si une collision est détectée, infliger des dégâts au joueur
        player->DamagePlayer(player, monster->attack);  // Dégâts infligés au joueur
    }
}

// Applique des dégâts au monstre en fonction de la valeur 'damage' passée.
// Si les dégâts sont suffisants pour tuer le monstre, son statut 'is_alive' est mis à 'false'.
void DamageMonster(Monster *monster, double damage)
{
    // Si les dégâts n'atteignent pas un seuil critique, réduire la vie du monstre
    if ((monster->health - damage) > damage)
    {
        monster->health -= damage;
    }
    else
    {
        // Si les dégâts tuent le monstre, il devient mort
        monster->is_alive = false;  // Le monstre meurt si sa vie tombe trop bas
    }
}

// Affiche la barre de vie du monstre au-dessus de lui.
// La barre est verte pour la vie restante et rouge pour la bordure.
void HealthBarMonster(Monster *monster, SDL_Renderer *render)
{
    // Création des rectangles pour le contour et le remplissage de la barre de vie
    SDL_Rect border = {monster->rect.x + 50, monster->rect.y - 20, monster->max_health, 5};
    SDL_Rect fill = border;

    // Calcul de la largeur de la barre de vie en fonction de la santé actuelle du monstre
    if (monster->max_health > 0)
    {
        fill.w = (monster->health * border.w) / monster->max_health;
    }
    else
    {
        fill.w = 0;  // La barre ne s'affiche pas si la santé maximale est 0
    }

    // Affichage du contour de la barre (rouge)
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);  // Couleur rouge
    SDL_RenderDrawRect(render, &border);

    // Affichage de la barre de vie (verte)
    SDL_SetRenderDrawColor(render, 0, 255, 0, 255);  // Couleur verte
    SDL_RenderFillRect(render, &fill);  // Remplissage de la barre de vie
}

// Réinitialise les caractéristiques d'un monstre (position, vie, etc.) selon son type.
// Cette fonction permet de remettre à zéro les statistiques du monstre après sa mort ou lors d'un respawn.
void resetMonster(Monster *monster, Entities *entities)
{
    switch(monster->type)
    {
        case MONSTER:
            // Pour un monstre standard, définir une position aléatoire et réinitialiser la vie
            monster->rect.x = rand() % (SCREEN_WIDTH - 1350 + 1) + 1350;  // Nouvelle position aléatoire à droite
            monster->velocity = rand() % (MAX_SPEED - 1 + entities->GameLvl) + entities->GameLvl;  // Vitesse aléatoire
            monster->health = monster->max_health;  // Réinitialiser la santé
            monster->is_alive = true;  // Le monstre est réanimé
            break;
        case ALIEN:
            // Pour un alien, définir également une position aléatoire mais avec des caractéristiques spécifiques
            monster->rect.x = rand() % (SCREEN_WIDTH - 1350 + 1) + 1350;
            monster->health = monster->max_health;
            monster->is_alive = true;
            break;
        default:
            break;
    }
}

// Libère la mémoire allouée pour un tableau de monstres.
// Cette fonction est cruciale pour éviter les fuites de mémoire lorsqu'un monstre n'est plus nécessaire.
void freeMonsters(Monster *monster[])
{
    for (int i = 0; i < MAX_MONSTER; i++)
    {
        if (monster[i])
        {
            SDL_DestroyTexture(monster[i]->texture);  // Libération de la texture du monstre
            free(monster[i]);  // Libération de la mémoire allouée pour le monstre
        }
    }
}

// Initialise un monstre standard de type MONSTER avec les paramètres donnés.
// Cela inclut l'affectation de la texture, de la position, de la vitesse, des points de loot, etc.
void initMonster(Monster *monster, SDL_Renderer *render, const char *imgpath)
{
    monster->type = MONSTER;
    monster->rect.x = rand() % (SCREEN_WIDTH - 1350 + 1) + 1350;  // Position aléatoire sur l'écran
    monster->rect.y = 325;  // Position verticale fixe
    monster->rect.h = 200;  // Taille du monstre (hauteur)
    monster->rect.w = 200;  // Taille du monstre (largeur)
    monster->texture = IMG_LoadTexture(render, imgpath);  // Chargement de la texture du monstre
    monster->velocity = rand() % (7 - 1 + 1) + 1;  // Vitesse aléatoire
    monster->is_alive = true;
    monster->attack = 0.3f;  // Puissance d'attaque par défaut
    monster->pointLoot = rand() % (5 - 1 + 1) + 1;  // Points de loot aléatoires
    monster->max_health = 100;  // Santé maximale
    monster->health = 100;  // Santé actuelle (initialisée à la santé maximale)
    
    // Affectation des fonctions associées au monstre
    monster->forward = forward;
    monster->HealthBarMonster = HealthBarMonster;
    monster->DamageMonster = DamageMonster;
    monster->reset = resetMonster;
}

// Initialise un monstre de type ALIEN avec les paramètres donnés.
// Les aliens ont des caractéristiques différentes : plus de vie, une attaque plus forte, etc.
void initAlien(Monster *alien, SDL_Renderer *render, const char *imgpath)
{
    alien->type = ALIEN;
    alien->rect.x = rand() % (SCREEN_WIDTH - 1350 + 1) + 1350;  // Position aléatoire à droite
    alien->rect.y = 150;  // Position verticale plus haute
    alien->rect.h = 400;  // Taille du monstre (hauteur)
    alien->rect.w = 400;  // Taille du monstre (largeur)
    alien->texture = IMG_LoadTexture(render, imgpath);  // Chargement de la texture
    alien->velocity = 1;  // Vitesse plus lente que celle des monstres standards
    alien->is_alive = true;
    alien->attack = 0.5f;  // Plus de puissance d'attaque pour les aliens
    alien->pointLoot = rand() % (10 - 2 + 1) + 2;  // Points de loot plus élevés
    alien->max_health = 250;  // Plus de vie pour les aliens
    alien->health = 250;  // Santé actuelle initiale égale à la santé maximale
    
    // Affectation des comportements spécifiques des aliens
    alien->forward = forward;
    alien->HealthBarMonster = HealthBarMonster;
    alien->DamageMonster = DamageMonster;
    alien->reset = resetMonster;
}

