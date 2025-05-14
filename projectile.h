#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define Max_Projectiles 20  // Définit le nombre maximal de projectiles pouvant être actifs à un moment donné

// Déclarations anticipées des structures utilisées sans inclure directement les fichiers de Player et Monster
typedef struct Player Player;  // Déclare un type Player pour pouvoir l'utiliser sans inclure player.h
typedef struct Monster Monster;  // Déclare un type Monster pour pouvoir l'utiliser sans inclure monster.h

// Définition de la structure Projectile
typedef struct Projectile
{
  SDL_Rect rect;  // Rectangle représentant la position et la taille du projectile à l'écran
  SDL_Texture *texture;  // Texture du projectile, permettant de l'afficher à l'écran
  int velocity;  // Vitesse de déplacement du projectile
  double angle;  // Angle de rotation du projectile (s'il en a un)
  
  // Indicateurs pour l'état du projectile
  bool isOutOfScreen;  // Indique si le projectile est sorti de l'écran
  bool hasHit;  // Indique si le projectile a touché un monstre
  bool is_projectile_active;  // Indique si le projectile est actif ou non
} Projectile;

// Prototypes des fonctions de gestion des projectiles
void MoveProject(Projectile *, Monster **, Player *);  // Fonction pour déplacer le projectile et gérer ses collisions
void initProjectile(Projectile *, SDL_Renderer *, const char *, Player *);  // Fonction pour initialiser un projectile
void resetProjectile(Projectile *, Player *);  // Fonction pour réinitialiser le projectile

#endif

