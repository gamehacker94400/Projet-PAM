#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <time.h>
#include "game.h" // Contient les déclarations liées au jeu (player, monster, etc.)

int main()
{
    // Initialise le générateur de nombres aléatoires avec l'heure actuelle
    srand(time(NULL));

    // Variables pour le contrôle du framerate
    Uint32 frameStart;
    int frameTime;
    const int FPS = 60;                        // Nombre d'images par seconde
    const int frameDelay = 1000 / FPS;         // Temps minimum par frame en ms

    // Initialisation de la SDL (tous les sous-systèmes)
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return 1;
    }

    // Initialisation de la SDL_ttf (pour afficher du texte)
    if (TTF_Init() == -1)
    {
        SDL_Log("Erreur TTF_Init: %s", TTF_GetError());
        return 1;
    }

    // Création de la fenêtre principale du jeu
    SDL_Window *window = SDL_CreateWindow("Shooter in C", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window)
    {
        SDL_Log("Erreur création fenêtre: %s", SDL_GetError());
        return 1;
    }

    // Création du renderer pour dessiner dans la fenêtre
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!render)
    {
        SDL_Log("Erreur création renderer: %s", SDL_GetError());
        return 1;
    }

    // Chargement de l'image de fond du jeu
    SDL_Texture *background = IMG_LoadTexture(render, "../assets/bg.jpg");
    if (!background)
    {
        SDL_Log("Erreur chargement background: %s", IMG_GetError());
        return 1;
    }

    // Chargement de la police utilisée pour les textes
    TTF_Font *font = TTF_OpenFont("../assets/font.otf", 24);
    if (!font)
    {
        SDL_Log("Erreur chargement font: %s", TTF_GetError());
        return 1;
    }

    // --- Initialisation des entités du jeu ---

    // Allocation mémoire pour le joueur et les entités
    Player *player = malloc(sizeof(Player));
    Entities *entities = malloc(sizeof(Entities));

    // Initialisation des champs de la structure Entities
    entities->player = player;
    entities->is_playing = false;          // Le jeu commence en pause (dans le menu d'accueil)
    entities->EventPercentSpeed = 5;       // Vitesse de chargement de la barre d'événement
    entities->event_start = false;         // L'événement (météorites) n'est pas encore lancé
    entities->JusteStarted = true;         // Permet d'afficher une seule fois l'écran d'accueil

    // Chargement et initialisation du joueur, des monstres, des projectiles et des météorites
    initPlayer(player, render, "../assets/player.png");
    InitMonsters(render, entities);
    InitProjectiles(entities, render);
    InitComets(render, entities);

    // Affichage de l'écran d'accueil
    Home(render, background);

    // --- Boucle principale du jeu ---
    bool running = true;
    int countMonster = 0; // Inutilisé pour l’instant

    while (running)
    {
        // Début de la frame
        frameStart = SDL_GetTicks();

        // Gestion des événements (clavier, souris, fermeture fenêtre, etc.)
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)  // Si on ferme la fenêtre
                running = false;
        }

        // Lancement de la logique et du rendu du jeu
        Game(render, entities, background, font, countMonster);

        // Calcul du temps passé durant cette frame
        frameTime = SDL_GetTicks() - frameStart;

        // Si la frame a été trop rapide, on attend pour respecter le FPS cible
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // --- Nettoyage mémoire et fermeture du programme ---

    FreeProjectiles(entities);            // Libération mémoire des projectiles
    freeMonsters(entities->monster);      // Libération mémoire des monstres
    free(entities->player);               // Libération mémoire du joueur
    free(entities);                       // Libération mémoire de la structure principale
    SDL_DestroyTexture(background);       // Destruction de la texture de fond
    TTF_CloseFont(font);                  // Fermeture de la police
    SDL_DestroyRenderer(render);          // Destruction du renderer
    SDL_DestroyWindow(window);            // Destruction de la fenêtre
    TTF_Quit();                           // Fermeture de SDL_ttf
    SDL_Quit();                           // Fermeture de SDL

    return 0;
}

