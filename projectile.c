#include "projectile.h"
#include "player.h"  // Inclut la structure Player pour accéder aux informations du joueur
#include "monster.h" // Inclut la structure Monster pour accéder aux informations des monstres
#include "game.h"    // Inclut d'autres fonctions ou constantes nécessaires au jeu

// Fonction pour initialiser un projectile
void initProjectile(Projectile *project, SDL_Renderer *render, const char *imgpath, Player *player)
{
  // Définir la taille du projectile
  project->rect.h = 50;                                                            // Hauteur du projectile
  project->rect.w = 50;                                                            // Largeur du projectile

  // Placer le projectile juste à droite du joueur
  project->rect.x = player->rect.x + player->rect.w;                               // Le projectile commence à la droite du joueur
  project->rect.y = player->rect.y + (player->rect.h / 2) - (project->rect.h / 2); // Centrer verticalement par rapport au joueur

  // Charger la texture du projectile depuis le chemin spécifié
  project->texture = IMG_LoadTexture(render, imgpath);
  project->velocity = 5;  // La vitesse de déplacement du projectile
  project->angle = 0;     // L'angle du projectile (initialement à 0)
}

// Fonction pour déplacer le projectile et gérer ses interactions avec les monstres
void MoveProject(Projectile *project, Monster **monster, Player *player)
{
  // Vérifie que le projectile, les monstres et le joueur sont valides
  if (!project || !monster || !player)
    return;

  // Déplace le projectile vers la droite
  project->rect.x += project->velocity;
  project->angle += 12;  // Incrémente l'angle du projectile pour peut-être avoir une rotation ou un mouvement spirale (en fonction du jeu)

  // Si le projectile sort de l'écran (à droite), il est désactivé
  if (project->rect.x > SCREEN_WIDTH)
  {
    project->is_projectile_active = false; // Marque le projectile comme inactif
  }

  // Vérifie les collisions entre le projectile et chaque monstre
  for (int i = 0; i < MAX_MONSTER; i++)
  {
    Monster *monste = monster[i];
    // Si le monstre est vivant et qu'il y a collision avec le projectile
    if (monste->is_alive && CheckCollision(&project->rect, &monste->rect))
    {
      // Inflige des dégâts au monstre
      monste->health -= player->attack;
      project->is_projectile_active = false; // Désactive le projectile après la collision
      break; // Sort de la boucle dès qu'un monstre est touché
    }
  }
}

// Fonction pour réinitialiser un projectile
void resetProjectile(Projectile *project, Player *player)
{
  // Réinitialise la position du projectile à la droite du joueur
  project->rect.x = player->rect.x + player->rect.w;
  project->rect.y = player->rect.y + (player->rect.h / 2) - (project->rect.h / 2);

  // Réinitialise les états du projectile
  project->hasHit = false;              // Le projectile n'a pas encore touché un monstre
  project->isOutOfScreen = false;       // Le projectile n'est pas sorti de l'écran
  project->is_projectile_active = false; // Le projectile est inactif
}

