#include<bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
//#include <thread>
using namespace std;
const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;
const int GRID_SIZE=20;
struct SnakeSegment {
    int x,y;
};
class SnakeGame {
public:
 SnakeGame();
 ~SnakeGame();
 void run();
private:
 void handleInput();
void update();
 void render();
 void spawnFood();
bool checkCollision();
 bool checkObstacleCollision();
 void activateBonusFood();
void deactivateBonusFood();
 void show();
 void renderText(const string& text, int x, int y);
 SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
vector<SnakeSegment> snake;
 vector<SnakeSegment> obstacles; 
SnakeSegment food;
int direction; 
 int score;
 int length;
  int FoodCount;
 SnakeSegment bonusFood;
 bool isBonusFoodActive;
 chrono::steady_clock::time_point bonusFoodActivationTime;
 bool gameOver;
};
SnakeGame::SnakeGame(){
SDL_Init(SDL_INIT_VIDEO);
 TTF_Init();
 window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
 renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
 font = TTF_OpenFont("Zebulon Condensed bold Italic.otf",28);
 snake.push_back({0,20});
 direction = 4;  
 length = 1;
  score = 0;
 FoodCount = 0;
    isBonusFoodActive = false;
 gameOver = false;
 srand(time(NULL));  
   spawnFood();
    show();
for(int i=100;i<=160;i=i+20)
    {
        obstacles.push_back({240,i});
    }
    //obstacles.push_back({240,100});
    //obstacles.push_back({240,120});
    //obstacles.push_back({240,140});
    //obstacles.push_back({240,160});
    obstacles.push_back({260,100});
    obstacles.push_back({280,100});
    obstacles.push_back({280,120});
    obstacles.push_back({280,140});
    obstacles.push_back({300,140});
    obstacles.push_back({320,140});
    obstacles.push_back({340,140});
    obstacles.push_back({340,120});
    obstacles.push_back({340,100});
    obstacles.push_back({360,100});
    obstacles.push_back({380,100});
    obstacles.push_back({380,120});
    obstacles.push_back({380,140});
    obstacles.push_back({380,160});
    obstacles.push_back({60,60});
    obstacles.push_back({60,80});
    obstacles.push_back({60,100});
    obstacles.push_back({80,60});
    obstacles.push_back({100,60});
    obstacles.push_back({60,420});
    obstacles.push_back({80,420});
    obstacles.push_back({100,420});
    obstacles.push_back({60,400});
    obstacles.push_back({60,380});
    obstacles.push_back({580,60});
    obstacles.push_back({580,80});
    obstacles.push_back({580,100});
    obstacles.push_back({560,60});
    obstacles.push_back({540,60});
    obstacles.push_back({580,420});
    obstacles.push_back({560,420});
    obstacles.push_back({540,420});
    obstacles.push_back({580,400});
    obstacles.push_back({580,380});
}
//constructive
void SnakeGame::show() {
    bool Active = true;
    SDL_Event p;
    bool renderTextOnce = true;
 while (Active) {
     while (SDL_PollEvent(&p) != 0) {
 if (p.type == SDL_QUIT) {
    gameOver = true;
 Active = false;
 } else if (p.type == SDL_KEYDOWN) {
     if (p.key.keysym.sym == SDLK_s) {
     Active = false;
 } else if (p.key.keysym.sym == SDLK_e) {
 gameOver = true;
Active = false;
 }
}
}
 if (renderTextOnce) {
 SDL_SetRenderDrawColor(renderer, 200, 14, 0, 1); 
 SDL_RenderClear(renderer);
 renderText("Snake Game", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50);
 renderText("Press 'S' for start or 'E' for exit", SCREEN_WIDTH / 7, SCREEN_HEIGHT / 2);
 SDL_RenderPresent(renderer);
             }}
}
//destructive
SnakeGame::~SnakeGame() {
    TTF_CloseFont(font);
    TTF_Quit();
}
void SnakeGame::run() {
    SDL_Event e;
    while (!gameOver) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                gameOver = true;
            }
        }
        handleInput();
        update();
        render();
        SDL_Delay(150);  
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void SnakeGame::handleInput() {

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_UP] && direction != 2) {
        direction = 1;
    } else if (keys[SDL_SCANCODE_DOWN] && direction != 1) {
        direction = 2;
    } else if (keys[SDL_SCANCODE_LEFT] && direction != 4) {
        direction = 3;
    } else if (keys[SDL_SCANCODE_RIGHT] && direction != 3) {
        direction = 4;
    }
}
void SnakeGame::update() {
    if (gameOver) {
        return;
    }
    
    SnakeSegment newHead = snake.front();
    switch (direction) {
        case 1:
            //if (newHead.y <= 0 ) newHead.y = SCREEN_HEIGHT-GRID_SIZE;
            //else
            newHead.y -= GRID_SIZE;
            break;
        case 2:
           // if (newHead.y >= SCREEN_HEIGHT-GRID_SIZE ) newHead.y = 0;
            //else
                newHead.y += GRID_SIZE;
            break;
        case 3:
            if (newHead.x <= 0 ) newHead.x = SCREEN_WIDTH-GRID_SIZE;
            else
            newHead.x -= GRID_SIZE;
            break;
        case 4:
            if (newHead.x >= SCREEN_WIDTH-GRID_SIZE ) newHead.x = 0;
            else
            newHead.x += GRID_SIZE;
            break;
    }
    
    if (checkObstacleCollision()) {
        renderText(" Collision with obstacle .", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2);
        renderText("Press Y to continue or N to exit.", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2+40);
        SDL_Delay(1500);
        SDL_Event keyEvent;
        while (true) {
            SDL_PollEvent(&keyEvent);
            if (keyEvent.type == SDL_QUIT) exit(1);
            else if (keyEvent.type == SDL_KEYDOWN) {
                if (keyEvent.key.keysym.sym == SDLK_y) {
    score -= 10;
snake.clear();
int x = 0;
for (int i = 0; i < length; ++i){
 SnakeSegment newSeg = {x,20};
 snake.insert(snake.begin(), newSeg);
 x += GRID_SIZE;
 }
 direction = 4;
 gameOver = false;
 break;} 
 else if (keyEvent.key.keysym.sym == SDLK_n) { 
         gameOver = true;
        break; }
            }}
        return;}
    snake.insert(snake.begin(), newHead);
     if (newHead.x == food.x && newHead.y == food.y) {
        spawnFood();
        length++;
        score += 10;  
        FoodCount++;
        if (FoodCount % 5 == 0) {
            activateBonusFood();}} 
    else if (isBonusFoodActive && newHead.x == bonusFood.x && newHead.y == bonusFood.y) {
    deactivateBonusFood();
        score += 100; 
    FoodCount = 0;  
    }
    else {
    snake.pop_back();
    }
    if (checkCollision()) {
    gameOver = true; }
    if (isBonusFoodActive) {
        auto currentTime = chrono::steady_clock::now();
        if (chrono::duration_cast<chrono::seconds>(currentTime - bonusFoodActivationTime).count() >= 5) {
            deactivateBonusFood();
        }}}
//draw graphics window
void SnakeGame::render() {
    SDL_SetRenderDrawColor(renderer, 0,100, 100, 50); 
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,160, 240, 0, 0);
    SDL_Rect topWall = { 0, 0, SCREEN_WIDTH,GRID_SIZE/2};
    SDL_RenderFillRect(renderer, &topWall);
    SDL_Rect bottomWall = { 0, SCREEN_HEIGHT - GRID_SIZE/2, SCREEN_WIDTH, GRID_SIZE/2 };
    SDL_RenderFillRect(renderer, &bottomWall);
     SDL_Rect leftWall = { 0, 0, GRID_SIZE/2,SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &leftWall);
    SDL_Rect rightWall = { SCREEN_WIDTH-GRID_SIZE/2,0, GRID_SIZE/2,SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &rightWall);
    SDL_SetRenderDrawColor(renderer, 0, 25, 20, 255);  
    for (const auto& segment : snake) {
        SDL_Rect rect = {segment.x, segment.y, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_SetRenderDrawColor(renderer,160, 240, 0, 0);   
    for (const auto& obstacle : obstacles) {
        SDL_Rect obstacleRect = {obstacle.x, obstacle.y, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &obstacleRect);
    }
    SDL_SetRenderDrawColor(renderer,19, 54, 85, 100);  
    SDL_Rect foodRect = {food.x, food.y, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);
    if (isBonusFoodActive) {
    SDL_SetRenderDrawColor(renderer, 25, 255, 90, 25);   
    SDL_Rect bonusFoodRect = {bonusFood.x, bonusFood.y, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &bonusFoodRect);
    }
    renderText("Score: " + to_string(score), 20, 20);
    if (gameOver) {
        renderText("Game Over!", SCREEN_WIDTH/2-100,SCREEN_HEIGHT/2-50);
        renderText("Your score is: " + to_string(score), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2);
       renderText("thank you", SCREEN_WIDTH/2-100,SCREEN_HEIGHT/2+50);
        SDL_Delay(1500);}
    SDL_RenderPresent(renderer);
}

//new food create
void SnakeGame::spawnFood() {
     int maxX = (SCREEN_WIDTH / GRID_SIZE - 1) * GRID_SIZE;
    int maxY = (SCREEN_HEIGHT / GRID_SIZE - 1) * GRID_SIZE;
    do {int randX = GRID_SIZE * (1 + rand() % (maxX / GRID_SIZE));
        int randY = GRID_SIZE * (1 + rand() % (maxY / GRID_SIZE));
        bool onObstacleOrWall = false;
        for (const auto& obstacle : obstacles) {
            if (randX == obstacle.x && randY == obstacle.y) {
                onObstacleOrWall = true;
                break;} }
        if (!onObstacleOrWall && randX > GRID_SIZE && randX < SCREEN_WIDTH - GRID_SIZE && randY > GRID_SIZE && randY < SCREEN_HEIGHT - GRID_SIZE) {
            food = { randX, randY };
            break;
        }} while (true);}

//bonus food create
void SnakeGame::activateBonusFood() { int maxX = (SCREEN_WIDTH / GRID_SIZE - 1) * GRID_SIZE;
    int maxY = (SCREEN_HEIGHT / GRID_SIZE - 1) * GRID_SIZE;
    do {int randX = GRID_SIZE * (1 + rand() % (maxX / GRID_SIZE));
        int randY = GRID_SIZE * (1 + rand() % (maxY / GRID_SIZE));
        bool onObstacleOrWall = false;
        for (const auto& obstacle : obstacles) {
            if (randX == obstacle.x && randY == obstacle.y) {
                onObstacleOrWall = true;
                break;}}
        if (!onObstacleOrWall && randX > GRID_SIZE && randX < SCREEN_WIDTH - GRID_SIZE && randY > GRID_SIZE && randY < SCREEN_HEIGHT - GRID_SIZE &&
    randX != food.x && randY != food.y) {
    bonusFood = { randX, randY };
     isBonusFoodActive = true;
     bonusFoodActivationTime = chrono::steady_clock::now();
    return;}
} while (true);
}

void SnakeGame::deactivateBonusFood() {
    isBonusFoodActive = false;}

//obstacle collision
bool SnakeGame::checkObstacleCollision() {
for (const auto& obstacle : obstacles) {
        if (snake.front().x == obstacle.x && snake.front().y == obstacle.y) {
        return true;
        } }
    return false;
}

//collision check
bool SnakeGame::checkCollision() {
    if(snake.front().y <= 10 || snake.front().y >= SCREEN_HEIGHT-10) {
        return true;}
        for (size_t i = 1; i < snake.size(); ++i) {
        if (snake[i].x == snake.front().x && snake[i].y == snake.front().y) {
            return true;
        } }
    return false;}

//texture decorate
void SnakeGame::renderText(const std::string& text, int x, int y) {
SDL_Color textColor = {0,0,0,0};  
SDL_Surface* textSurface = TTF_RenderText_Solid(font,text.c_str(), textColor);
SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
SDL_Rect textRect = {x,y,textSurface->w,textSurface->h};
SDL_RenderCopy(renderer,textTexture,NULL,&textRect);
SDL_RenderPresent(renderer);
SDL_FreeSurface(textSurface);
SDL_DestroyTexture(textTexture);
}
int main(int argc, char* argv[]) {
SnakeGame game;
 game.run();
return 0;
}