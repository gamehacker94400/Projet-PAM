#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "player.h"
#include "monster.h"
#include "projectile.h"
#include "comet.h"

#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 600
#define MAX_MONSTER 4
#define MAX_PROJECTILE 20
#define MAX_COMET 20

typedef struct
{
  Player *player;
  Monster *monster[MAX_MONSTER];
  Projectile *projectile[MAX_PROJECTILE];
  Comet *comets[MAX_COMET];

  // États principaux du jeu
  bool event_start;        // Indique si l'événement des comètes est en cours
  bool JusteStarted;       // Permet de savoir si l’événement vient de commencer pour réinitialiser les monstres une seule fois
  bool is_playing;         // Indique si le jeu est en cours

  // Variables pour la barre de pourcentage de l'événement
  double EventPercent;       // Pourcentage de progression de l'événement
  double EventPercentSpeed;  // Vitesse de remplissage du pourcentage
} Entities;

// Écran d'accueil (également utilisé comme écran de pause)
void Home(SDL_Renderer *, SDL_Texture *);

// Affichage des entités
void RenderPlayer(Entities *, SDL_Renderer *);         // Affiche le joueur et sa barre de vie
void RenderProject(Entities *, SDL_Renderer *);        // Affiche les projectiles lancés par le joueur
void RenderMonsters(Entities *, SDL_Renderer *);       // Affiche les monstres
void RenderComet(Entities *, SDL_Renderer *);          // Affiche les comètes
void RenderEventBar(SDL_Renderer *, Entities *);       // Affiche la barre d'événement
void RenderAll(SDL_Renderer *, SDL_Texture *, Entities *); // Regroupe tous les rendus
void RenderScore(Player *, TTF_Font *, SDL_Renderer *);    // Affiche le score du joueur

// Initialisation des entités
void InitProjectiles(Entities *, SDL_Renderer *); // Initialise les projectiles du joueur
void InitMonsters(SDL_Renderer *, Entities *);    // Initialise les monstres
void InitComets(SDL_Renderer *, Entities *);      // Initialise les comètes

// Gestion des comètes
void ActivateComets(Entities *);                 // Active les comètes pour qu'elles tombent
bool AllCometsFallen(Comet *[]);                 // Retourne true si toutes les comètes sont tombées

// Détection de collision
bool CheckCollision(SDL_Rect *, SDL_Rect *);     // Retourne true s’il y a collision entre deux rectangles

// Événement comète
void UpPercent(Entities *);                      // Augmente le pourcentage de l’événement comète
void UpdateEvent(Entities *);                    // Met à jour l'état de l'événement comète
void RenderEventBar(SDL_Renderer *, Entities *); // Affiche la barre de progression de l’événement

// Mouvements
void MoveAllProject(Entities *);     // Déplace tous les projectiles du joueur
void ForwardAllMonster(Entities *);  // Fait avancer tous les monstres

// Gestion mémoire et état
void ClearMonster(Entities *, int);  // Réinitialise un monstre donné
void FreeProjectiles(Entities *);    // Libère la mémoire allouée aux projectiles
void GameOver(Entities *);           // Réinitialise le jeu lorsque le joueur perd

// Boucle principale du jeu
void Game(SDL_Renderer *, Entities *, SDL_Texture *, TTF_Font *, int);

#endif

