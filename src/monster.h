#ifndef MONSTER_H
#define MONSTER_H

#include <SDL2/SDL_image.h>

// Déclaration anticipée de la structure Player pour éviter les dépendances circulaires
// Cela permet d'utiliser le type Player sans inclure player.h
typedef struct Player Player;

// Enumération définissant les types possibles de monstres
// Chaque type a des caractéristiques différentes (ex : MONSTER standard, ALIEN spécial)
typedef enum {
  MONSTER,  // Type de monstre standard
  ALIEN     // Type de monstre spécial, par exemple un alien avec des caractéristiques uniques
} MonsterType;

// Définition de la structure Monster qui représente un monstre dans le jeu
typedef struct Monster
{
  MonsterType type;            // Type du monstre : MONSTER ou ALIEN
  SDL_Rect rect;               // Position et taille du monstre dans la fenêtre de jeu (utilisé pour le rendu avec SDL)
  SDL_Texture *texture;        // Texture du monstre, utilisée pour l'affichage graphique
  int health, max_health;      // Vie actuelle et vie maximale du monstre
  int pointLoot;               // Points obtenus par le joueur lorsqu'il tue ce monstre
  double attack, velocity;     // Puissance d'attaque et vitesse de déplacement du monstre
  int is_alive;                // Indicateur de l'état du monstre (1 = vivant, 0 = mort)

  // Fonction de réinitialisation du monstre (utilisée pour réinitialiser sa position, ses stats, etc.)
  void (*reset)(struct Monster *);                            

  // Fonction pour déplacer le monstre vers le joueur
  void (*forward)(struct Monster *, Player *);                

  // Fonction pour afficher la barre de vie du monstre à l'écran
  void (*HealthBarMonster)(struct Monster *, SDL_Renderer *); 

  // Fonction pour appliquer des dégâts au monstre
  void (*DamageMonster)(struct Monster *, double);            

} Monster;

// Prototypes des fonctions liées à la gestion des monstres dans le jeu

// Initialise un monstre de type standard (MONSTER) avec les paramètres donnés
// Cette fonction affecte une texture, une position, une vitesse et d'autres caractéristiques spécifiques
void initMonster(Monster *, SDL_Renderer *, const char *);

// Initialise un monstre de type ALIEN avec les paramètres donnés
// Cette fonction affecte des caractéristiques spécifiques aux aliens (par exemple plus de vie, d'attaque)
void initAlien(Monster *, SDL_Renderer *, const char *);

// Réinitialise un monstre à sa position initiale et réinitialise ses statistiques (par exemple, sa vie)
void resetMonster(Monster *);

// Libère la mémoire allouée pour tous les monstres dans un tableau de pointeurs de monstres
// Cela permet de nettoyer la mémoire lorsque les monstres sont supprimés ou ne sont plus nécessaires
void freeMonsters(Monster *[]);

#endif

