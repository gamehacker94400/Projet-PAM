#include "game.h"

// Affiche tous les projectiles actifs du joueur
void RenderProject(Entities *entitie, SDL_Renderer *render)
{
  for (int i = 0; i < MAX_PROJECTILE; i++)
  {
    if (entitie->projectile[i]->is_projectile_active)
    {
      SDL_RenderCopyEx(render, entitie->projectile[i]->texture, NULL, &entitie->projectile[i]->rect, entitie->projectile[i]->angle, NULL, SDL_FLIP_NONE);
    }
  }
}

// Affiche toutes les comètes actives
void RenderComet(Entities *entities , SDL_Renderer *render){
  for (int i = 0; i < MAX_COMET; i++){
    if (entities->comets[i]->is_comet_active){
      SDL_RenderCopy(render,entities->comets[i]->texture,NULL,&entities->comets[i]->rect);
    }
  }
}

// Affiche le joueur et sa barre de vie
void RenderPlayer(Entities *entitie, SDL_Renderer *render)
{
  SDL_RenderCopy(render, entitie->player->texture, NULL, &entitie->player->rect);
  entitie->player->HealthBarPlayer(render, entitie->player);
}

// Affiche tous les monstres vivants et leur barre de vie
void RenderMonsters(Entities *entitie, SDL_Renderer *render)
{
  for (int i = 0; i < MAX_MONSTER; i++)
  {
    if (entitie->monster[i]->is_alive)
    {
      SDL_RenderCopy(render, entitie->monster[i]->texture, NULL, &entitie->monster[i]->rect);
      entitie->monster[i]->HealthBarMonster(entitie->monster[i], render);
    }
  }
}

// Regroupe tous les rendus principaux à l'écran
void RenderAll(SDL_Renderer *render, SDL_Texture *background, Entities *entities)
{
  SDL_RenderClear(render);
  SDL_RenderCopy(render, background, NULL, NULL);
  RenderPlayer(entities, render);
  RenderMonsters(entities, render);
  RenderEventBar(render, entities);
  
  if (entities->event_start) {
    RenderComet(entities, render);
  }
}

// Affiche le score actuel du joueur
void RenderScore(Player *player, TTF_Font *font, SDL_Renderer *render)
{
  char score[32];
  sprintf(score, "score : %d", player->score);
  SDL_Color couleur = {255, 255, 255, 255};
  SDL_Surface *fontSurf = TTF_RenderText_Blended(font, score, couleur);
  SDL_Texture *fontText = SDL_CreateTextureFromSurface(render, fontSurf);
  SDL_FreeSurface(fontSurf);
  
  SDL_Rect fontRect = {50, 50, 0 , 0};
  SDL_QueryTexture(fontText, NULL, NULL, &fontRect.w, &fontRect.h);
  SDL_RenderCopy(render, fontText, NULL, &fontRect);
}

// Affiche l’écran d’accueil avec le bouton et la bannière
void Home(SDL_Renderer *render, SDL_Texture *background)
{
  SDL_Texture *button = NULL;
  SDL_Texture *banner = NULL;

  SDL_Rect bannerRect = {SCREEN_WIDTH / 3, 10, 500, 450};
  SDL_Rect buttonRect = {(SCREEN_WIDTH / 3)+60, (SCREEN_HEIGHT / 2) + 50, 400, 150};

  button = IMG_LoadTexture(render, "assets/button.png");
  banner = IMG_LoadTexture(render, "assets/banner.png");

  SDL_RenderCopy(render, background, NULL, NULL);
  SDL_RenderCopy(render, button, NULL, &buttonRect);
  SDL_RenderCopy(render, banner, NULL, &bannerRect);
  SDL_RenderPresent(render);

  SDL_DestroyTexture(button);
  SDL_DestroyTexture(banner);
}

// Vérifie s’il y a une collision entre deux rectangles
bool CheckCollision(SDL_Rect *firstRect, SDL_Rect *secondRect)
{
  return SDL_HasIntersection(firstRect, secondRect);
}

// Réinitialise un monstre donné
void ClearMonster(Entities *entitie, int count)
{
  resetMonster(entitie->monster[count]);
}

// Initialise toutes les comètes du jeu
void InitComets(SDL_Renderer *render, Entities *entitie){
  for (int i = 0; i < MAX_COMET; i++){
    entitie->comets[i] = malloc(sizeof(Comet));
    initComet(entitie->comets[i], render, "assets/comet.png");
  }
}

// Fait tomber toutes les comètes actives
void FallAllComets(Entities *entities){
  for (int i = 0; i < MAX_COMET; i++){
    if (entities->comets[i]->is_comet_active){
      entities->comets[i]->cometFall(entities->comets[i], entities->player);
    }
  }
}

// Vérifie si toutes les comètes sont tombées
bool AllCometsFallen(Comet *comets[])
{
  for (int i = 0; i < MAX_COMET; i++)
  {
    if (comets[i] && comets[i]->is_comet_active)
      return false;
  }
  return true;
}

// Active toutes les comètes et les repositionne
void ActivateComets(Entities *entities) {
  for (int i = 0; i < MAX_COMET; i++) {
    entities->comets[i]->is_comet_active = true;
    entities->comets[i]->rect.y = -entities->comets[i]->rect.h;
    entities->comets[i]->rect.x = rand() % (SCREEN_WIDTH - entities->comets[i]->rect.w);
  }
}

// Augmente progressivement le pourcentage de l’événement comète
void UpPercent(Entities *entities){
  entities->EventPercent += entities->EventPercentSpeed / 100;
  if (entities->EventPercent > 100) {
    entities->EventPercent = 100;
  }
}

// Met à jour l'état de l'événement comète
void UpdateEvent(Entities *entities){
  if(entities->EventPercent >= 100 && !entities->event_start){
    entities->event_start = true;
    entities->JusteStarted = true;
  }

  if (entities->event_start && AllCometsFallen(entities->comets)) {
    entities->event_start = false;
    ActivateComets(entities);
    entities->EventPercent = 0;
  }

  UpPercent(entities);
}

// Affiche la barre de progression de l'événement comète
void RenderEventBar(SDL_Renderer *render, Entities *entities){
  SDL_Rect border = {20, SCREEN_HEIGHT - 20, (SCREEN_WIDTH / 100) * entities->EventPercent, 10};
  SDL_Rect fill = border;

  SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
  SDL_RenderDrawRect(render, &border);

  SDL_SetRenderDrawColor(render, 187, 11, 11, 255);
  SDL_RenderFillRect(render, &fill);
}

// Initialise tous les monstres du jeu
void InitMonsters(SDL_Renderer *render, Entities *entitie)
{
  for (int i = 0; i < MAX_MONSTER - 1; i++)
  {
    entitie->monster[i] = malloc(sizeof(Monster));
    initMonster(entitie->monster[i], render, "assets/mummy.png");
  }

  entitie->monster[MAX_MONSTER - 1] = malloc(sizeof(Monster));
  initAlien(entitie->monster[MAX_MONSTER - 1], render, "assets/alien.png");
}

// Fait avancer tous les monstres et gère leur mort
void ForwardAllMonster(Entities *entitie)
{
  for (int i = 0; i < MAX_MONSTER; i++)
  {
    if (entitie->monster[i]->is_alive)
    {
      entitie->monster[i]->forward(entitie->monster[i], entitie->player);
    }

    if (entitie->monster[i]->health <= 0)
    {
      entitie->monster[i]->is_alive = false;
      entitie->player->score += entitie->monster[i]->pointLoot;
      ClearMonster(entitie, i);
    }
  }
}

// Initialise tous les projectiles du joueur
void InitProjectiles(Entities *entitie, SDL_Renderer *render)
{
  for (int i = 0; i < MAX_PROJECTILE; i++)
  {
    entitie->projectile[i] = malloc(sizeof(Projectile));
    initProjectile(entitie->projectile[i], render, "assets/projectile.png", entitie->player);
    entitie->projectile[i]->is_projectile_active = false;
  }
}

// Fait bouger tous les projectiles actifs
void MoveAllProject(Entities *entitie)
{
  for (int i = 0; i < MAX_PROJECTILE; i++)
  {
    if (entitie->projectile[i]->is_projectile_active)
    {
      MoveProject(entitie->projectile[i], entitie->monster, entitie->player);
    }
  }
}

// Libère la mémoire allouée aux projectiles
void FreeProjectiles(Entities *entitie)
{
  for (int i = 0; i < MAX_PROJECTILE; i++)
  {
    if (entitie->projectile[i])
    {
      SDL_DestroyTexture(entitie->projectile[i]->texture);
      free(entitie->projectile[i]);
      entitie->projectile[i] = NULL;
    }
  }
}

// Réinitialise les éléments du jeu quand le joueur meurt
void GameOver(Entities *entities){
  for (int i = 0; i < MAX_MONSTER; i++){
    resetMonster(entities->monster[i]);
  }
  entities->player->health = entities->player->max_health;
  entities->EventPercent = 0;
  entities->is_playing = false;
  entities->player->score = 0;
  entities->player->rect.x = 0;
  entities->player->rect.y = 350;
}

// Boucle principale du jeu (gère les entrées, logique, affichage et collisions)
void Game(SDL_Renderer *render, Entities *entities, SDL_Texture *background, TTF_Font *font, int countmonster)
{
  SDL_Event event;
  SDL_PollEvent(&event);

  const Uint8 *keystate = SDL_GetKeyboardState(NULL);

  // Gérer les entrées clavier
  if (keystate[SDL_SCANCODE_SPACE]) {
    if (!entities->is_playing) {
      entities->is_playing = true;
      return;
    } else {
      entities->player->LaunchProject(entities->projectile, render, entities->player);
    }
  }

  if (keystate[SDL_SCANCODE_ESCAPE]) {
    if (entities->is_playing) {
      entities->is_playing = false;
      Home(render, background);
    }
  }

  if (!entities->is_playing) return;

  // Déplacement joueur
  if (keystate[SDL_SCANCODE_RIGHT]) {
    entities->player->MoveRight(entities->player, entities->monster[countmonster]);
  }

  if (keystate[SDL_SCANCODE_LEFT]) {
    entities->player->MoveLeft(entities->player);
  }

  // Logique de jeu
  UpdateEvent(entities);

  if (entities->event_start) {
    FallAllComets(entities);

    if (entities->JusteStarted) {
      for (int i = 0; i < MAX_MONSTER; i++)
        ClearMonster(entities, i);

      entities->JusteStarted = false;
    }
  } else {
    ForwardAllMonster(entities);
  }

  MoveAllProject(entities);

  // Rendu graphique
  RenderAll(render, background, entities);
  RenderProject(entities, render);
  RenderScore(entities->player, font, render);
  SDL_RenderPresent(render);
  SDL_Delay(16); // Limite à ~60 FPS

  // Fin de partie si joueur mort
  if (entities->player->health <= 0)
  {
    GameOver(entities);
    Home(render, background);
  }
}

