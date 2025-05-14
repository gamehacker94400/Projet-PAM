#include "player.h"
#include "monster.h"    // Inclut la structure Monster pour l'accès aux monstres
#include "projectile.h" // Inclut la structure Projectile pour la gestion des projectiles
#include "game.h"       // Si nécessaire pour d'autres fonctions du jeu

// Déplace le joueur vers la gauche, si ce n’est pas en dehors de l'écran.
void MoveLeft(Player *player)
{
  if (player->rect.x > 0)  // Vérifie si le joueur ne dépasse pas la limite gauche de l'écran
  {
    player->rect.x -= player->velocity;  // Déplace le joueur vers la gauche
  }
}

// Déplace le joueur vers la droite, si ce n’est pas en dehors de l'écran et qu'il n'y a pas de collision avec un monstre.
void MoveRight(Player *player, Monster *monster)
{
  if (player->rect.x + player->rect.w < SCREEN_WIDTH && !CheckCollision(&player->rect, &monster->rect))
  {
    player->rect.x += player->velocity;  // Déplace le joueur vers la droite
  }
}

// Affiche la barre de vie du joueur, avec une couleur verte proportionnelle à sa vie restante.
void HealthBarPlayer(SDL_Renderer *render, Player *player)
{
  // Rectangle pour afficher le contour de la barre de vie
  SDL_Rect border = {player->rect.x + 15, player->rect.y + 5, 150, 10};
  SDL_Rect fill = border;

  // Calcul de la largeur de la barre de vie en fonction de la santé du joueur
  fill.w = (player->health * border.w) / player->max_health;

  // Dessine le contour de la barre de vie en rouge
  SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
  SDL_RenderDrawRect(render, &border);

  // Remplit la barre de vie en vert en fonction de la santé restante
  SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
  SDL_RenderFillRect(render, &fill);
}

// Gère les dégâts infligés au joueur. Si la santé du joueur tombe à zéro ou moins, il meurt.
void DamagePlayer(Player *player, double damage)
{
  if ((player->health - damage) > damage)  // Si la santé restante est toujours positive
  {
    player->health -= damage;  // Réduit la santé du joueur
  }
  else
  {
    player->is_alive = false;  // Le joueur meurt si sa santé tombe à zéro ou moins
  }
}

// Lance un projectile à partir du joueur, en activant un projectile inactif si disponible.
void LaunchProject(Projectile *project[], SDL_Renderer *render, Player *player)
{
  // Cherche un projectile inactif pour le réinitialiser
  for (int i = 0; i < Max_Projectiles; i++)
  {
    if (project[i] && !project[i]->is_projectile_active)  // Si le projectile est inactif
    {
      // Initialise le projectile avec une image et la position du joueur
      initProjectile(project[i], render, "assets/projectile.png", player);
      project[i]->is_projectile_active = true;  // Active le projectile
      break;  // Sort de la boucle dès qu'un projectile a été lancé
    }
  }
}

// Initialise le joueur avec des paramètres par défaut (position, taille, attaque, santé, etc.)
void initPlayer(Player *player, SDL_Renderer *render, const char *imgpath)
{
  player->rect.x = 0;  // Position initiale du joueur (gauche de l'écran)
  player->rect.y = 350;  // Position verticale du joueur
  player->rect.h = 200;  // Hauteur du joueur
  player->rect.w = 200;  // Largeur du joueur
  player->texture = IMG_LoadTexture(render, imgpath);  // Charge l'image du joueur

  player->attack = 10;  // Points d'attaque du joueur
  player->health = 100;  // Santé initiale du joueur
  player->max_health = 100;  // Santé maximale du joueur
  player->velocity = 10;  // Vitesse de déplacement du joueur
  
  // Liaison des fonctions aux actions du joueur
  player->MoveRight = MoveRight;
  player->MoveLeft = MoveLeft;
  player->HealthBarPlayer = HealthBarPlayer;
  player->LaunchProject = LaunchProject;
  player->DamagePlayer = DamagePlayer;
}

