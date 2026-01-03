#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

// Déclarations anticipées des structures utilisées sans avoir besoin d'inclure les fichiers de monstres et projectiles.
typedef struct Monster Monster;  // Déclare un type Monster, utilisé dans le code sans avoir besoin d'inclure monster.h
typedef struct Projectile Projectile;  // Déclare un type Projectile, utilisé dans le code sans avoir besoin d'inclure projectile.h

// Définition de la structure Player, représentant un joueur.
typedef struct Player
{
  SDL_Rect rect;  // Rectangle représentant la position et la taille du joueur dans la fenêtre.
  SDL_Rect srcRect;  // Rect source utilisé pour découper une image (si sprite animé).
  SDL_Texture *texture;  // Texture du joueur, permettant de l'afficher à l'écran.
  
  // Propriétés liées à la vie et aux statistiques du joueur
  int health, max_health;  // Santé actuelle et santé maximale du joueur
  int velocity;  // Vitesse de déplacement du joueur
  int attack;  // Points d'attaque du joueur
  int score, high_score;  // Score actuel et meilleur score
  bool is_alive;  // Indique si le joueur est vivant ou non

  // Pointeurs vers les fonctions qui permettent d'agir sur le joueur
  void (*MoveLeft)(struct Player *);  // Fonction pour déplacer le joueur vers la gauche
  void (*MoveRight)(struct Player *, struct Monster *);  // Fonction pour déplacer le joueur vers la droite, avec gestion des collisions
  void (*LaunchProject)(struct Projectile *[], SDL_Renderer *, struct Player *);  // Fonction pour lancer des projectiles
  void (*DamagePlayer)(struct Player *, double);  // Fonction pour infliger des dégâts au joueur
  void (*HealthBarPlayer)(SDL_Renderer *, struct Player *);  // Fonction pour afficher la barre de vie du joueur

} Player;

// Fonction d'initialisation du joueur, appelée pour configurer ses valeurs par défaut.
void InitPlayer(Player *, SDL_Renderer *, const char *);

#endif

