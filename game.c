#include <raylib.h>
#include <stdlib.h>

#include "game.h"
#include "snake.h"
#include "food.h"
#include "ressources.h"
#include "hint.h"

// === GLOBAL VARIABLES ===
int score = 0;             // Current score of the player
int highScore = 0;         // Highest score achieved
int lastScore = 0;         // Score from the last game session
int fps = 60;              // Target frames per second
int frameCounter = 0;      // Frame counter used for timing movements
float moveDelay = 10.0f;   // Delay (in frames) between snake movements
GameScreen currentScreen = TITLE; // Current game screen (title, gameplay, pause, ending)

// === INITIALIZE THE GAME ===
// Set up window, audio, load resources, and initialize game entities
void InitSnakeGame(void)
{
    InitWindow(screenWidth, screenHeight, "The Snakeman");  // Create game window
    InitAudioDevice();                                      // Initialize audio
    SetTargetFPS(fps);                                      // Set target FPS

    LoadGameRessources();  // Load textures, audio, and fonts
    SetGameVariables();    // Initialize game variables (score, angles, etc.)
    InitGameEntities();    // Initialize snake and fruit entities
}

// === SET GAME VARIABLES ===
// Reset game variables at the start or after restart
void SetGameVariables(void)
{
    score = 0;
    currentScreen = TITLE;
    headAngle = 90;      // Initial snake head rotation
    frameCounter = 0;
    moveDelay = 10.0f;   // Initial movement delay
}

// === INITIALIZE GAME ENTITIES ===
// Initialize snake and fruit
void InitGameEntities(void)
{
    InitializeSnake();  // Create snake
    InitFruit();        // Initialize fruit positions
}

// === RESET GAME ===
// Reset snake, score, fences, and audio for a new game
void GameReset(void)
{
    FreeSnake();             // Free existing snake linked list
    head = CreateSnake();    // Create a new snake
    direction = (Vector2){ (float)tileSize, 0.0f };
    nextDirection = direction;
    fruitActive = false;     // No active fruit initially
    score = 0;               // Reset score
    frameCounter = 0;
    moveDelay = 10.0f;

    fenceCount = 0;          // Reset fence count

    // Reset audio flags to start music appropriately
    firstFrameTitle = true;
    playMusicGameplay = -1;
    playMusicPause = -1;
    playMusicEnding = -1;
}

// === UPDATE TITLE SCREEN ===
void UpdateTitleScreen(void)
{
    PlayTitleAudio();  // Play background music for title screen

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawGreenTiles(screenHeight, screenWidth, tileSize, lightGreen, darkGreen); // Draw grass tiles
    DrawRectangle(0, 0, screenWidth, screenHeight, semiTransparentBlack);       // Overlay for title
    DrawTitleText();   // Draw "Press ENTER" text
    EndDrawing();

    // Switch to gameplay when Enter is pressed
    if (IsKeyPressed(KEY_ENTER))
        currentScreen = GAMEPLAY;
}

// === UPDATE GAMEPLAY SCREEN ===
void UpdateGameplayScreen(void)
{
    frameCounter++;          // Increment frame counter
    PlayGameplayAudio();     // Play gameplay music
    SnakeDirectionInput();   // Handle player input
    SnakeMovement();         // Move snake
    FruitSpawn(head);        // Spawn a fruit if none is active
    FruitColision();         // Check for collisions with fruit
    SelfColision();          // Check for collisions with snake itself
    BorderColision();        // Check for collisions with borders
    FenceColision();        // Check for collisions with fences

    // --- DRAW GAMEPLAY ---
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawGreenTiles(screenHeight, screenWidth, tileSize, lightGreen, darkGreen); // background
    DrawGameplayText();   // Draw score, high score, last score
    DrawFruit();          // Draw fruit
    DrawSnake();          // Draw snake
    DrawFences();          // Draw fences
    EndDrawing();

    // Pause the game if 'P' is pressed
    if (IsKeyPressed(KEY_P))
        currentScreen = PAUSE;
}

// === UPDATE PAUSE SCREEN ===
void UpdatePauseScreen(void)
{
    PlayPauseAudio();  // Play pause screen music

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawGreenTiles(screenHeight, screenWidth, tileSize, lightGreen, darkGreen);
    DrawRectangle(0, 0, screenWidth, screenHeight, semiTransparentBlack); // overlay
    DrawPauseText();    // Draw pause text
    EndDrawing();

    // Resume gameplay if Enter is pressed
    if (IsKeyPressed(KEY_ENTER))
    {
        StopMusicStream(gameMusic[playMusicPause]);
        playMusicPause = -1;
        ResumeMusicStream(gameMusic[playMusicGameplay]);
        currentScreen = GAMEPLAY;
    }
}

// === UPDATE ENDING SCREEN ===
void UpdateEndingScreen(void)
{
    lastScore = score;        // Store last score
    if (score > highScore)
        highScore = score;    // Update high score

    PlayEndingAudio();        // Play ending music

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawGreenTiles(screenHeight, screenWidth, tileSize, lightGreen, darkGreen);
    DrawRectangle(0, 0, screenWidth, screenHeight, semiTransparentBlack); // overlay
    DrawEndingText();  // Show game over text
    EndDrawing();

    // Restart game if Enter is pressed
    if (IsKeyPressed(KEY_ENTER))
    {
        GameReset();
        currentScreen = TITLE;
    }
}

// === UPDATE GAME BASED ON CURRENT SCREEN ===
void UpdateGame(void)
{
    switch (currentScreen)
    {
    case TITLE:
        UpdateTitleScreen();
        break;
    case GAMEPLAY:
        UpdateGameplayScreen();
        break;
    case PAUSE:
        UpdatePauseScreen();
        break;
    case ENDING:
        UpdateEndingScreen();
        break;
    default:
        break;
    }
}

// === UNLOAD ALL TEXTURES ===
void UnloadGameTextures(void)
{
    UnloadTexture(headTexture);
    UnloadTexture(bodyTexture);
    UnloadTexture(legsTexture);

    for (int i = 0; i < FRUIT_NUMBER; i++)
        UnloadTexture(fruitTextures[i]);

    UnloadTexture(fenceTexture);
    UnloadFont(myFont);
}

// === FREE ALL RESOURCES ===
void FreeSnakeGame(void)
{
    FreeSnake();         // Free linked list of snake
    UnloadGameTextures(); // Free textures and font
    FreeMusic();          // Free music and sounds
}
