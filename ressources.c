#include <raylib.h>
#include <stdlib.h>

#include "game.h"
#include "ressources.h"
#include "snake.h"
#include "food.h"
#include "hint.h"

// === SCREEN SETTINGS ===
const int screenWidth = 960;       // Width of the game window
const int screenHeight = 1024;     // Height of the game window
const int tileSize = 64;           // Size of a single grid tile
const int whiteHeight = 64;        // Height of the white top bar (HUD)

// === COLORS ===
Color lightGreen = { 70, 158, 43, 255 };        // Light green for tiles
Color darkGreen = { 55, 125, 34, 255 };         // Dark green for tiles
Color black = { 0, 0, 0, 255 };                // Black color for text
Color semiTransparentBlack = { 0, 0, 0, 150 }; // Semi-transparent black

// === TEXTURES AND FONT ===
Texture2D headTexture;                        // Snake head texture
Texture2D bodyTexture;                        // Snake body texture
Texture2D legsTexture;                        // Snake tail/legs texture
Texture2D fruitTextures[FRUIT_NUMBER];        // Array of fruit textures
Texture2D fenceTexture;                        // Fence texture
Font myFont;                                  // Font for on-screen text

// === AUDIO ===
Music gameMusic[MUSIC_NUMBER];               // Array of game music tracks
Sound gameSound[SOUND_NUMBER];               // Array of sound effects (eating, bonuses, etc.)

bool firstFrameTitle = true;                 // Flag for first frame of title screen
int playMusicPause = -1;                     // Index of currently playing pause music (-1 if none)
int playMusicGameplay = -1;                  // Index of currently playing gameplay music (-1 if none)
int playMusicEnding = -1;                    // Index of currently playing ending music (-1 if none)

// === HEAD SETTINGS ===
int headAngle = 270;                         // Rotation angle of the snake head
Vector2 origin = { 0 };                       // Rotation origin of the head
Rectangle sourceRec = { 0 };                  // Source rectangle for head texture

// === AUDIO SETUP FUNCTION ===
void SetAudio(void)
{
    // --- Load Music Tracks ---
    gameMusic[0] = LoadMusicStream("Assets/Tic_Tac.mp3");              // Title screen
    gameMusic[1] = LoadMusicStream("Assets/pause_music.mp3");          // Pause screen
    gameMusic[2] = LoadMusicStream("Assets/Hard_Rock.mp3");            // Gameplay track 1
    gameMusic[3] = LoadMusicStream("Assets/Jay_in_the_elevator.mp3");  // Gameplay track 2
    gameMusic[4] = LoadMusicStream("Assets/bs_loosing_theme.mp3");     // Ending track 1
    gameMusic[5] = LoadMusicStream("Assets/bs_loosing_theme_slowed.mp3"); // Ending track 2

    // Set volume for all music tracks
    for (int i = 0; i < MUSIC_NUMBER; i++)
    {
        SetMusicVolume(gameMusic[i], 1.0f);
    }

    // --- Load Sound Effects ---
    gameSound[0] = LoadSound("Assets/eating_sound.wav");       // Normal eating sound
    gameSound[1] = LoadSound("Assets/eating_bonus_sound.wav"); // Bonus eating sound

    // Set volume for all sound effects
    for (int i = 0; i < SOUND_NUMBER; i++)
    {
        SetSoundVolume(gameSound[i], 1.0f);
    }
}

// === TEXTURE AND FONT SETUP FUNCTION ===
void SetGameTextures(void)
{
    Image image; // Temporary image used to load textures

    // --- Head Texture ---
    image = LoadImage("Assets/head.png");                 // Load image from file
    ImageColorReplace(&image, WHITE, BLANK);             // Replace white with transparency
    headTexture = LoadTextureFromImage(image);           // Create texture from image
    UnloadImage(image);                                  // Free temporary image
    origin = (Vector2){ (float)headTexture.width / 2.0f, (float)headTexture.height / 2.0f }; // Set rotation origin
    sourceRec = (Rectangle){ 0, 0, (float)headTexture.width, (float)headTexture.height };   // Full source rectangle

    // --- Body Texture ---
    image = LoadImage("Assets/body.png");
    ImageColorReplace(&image, WHITE, BLANK);
    bodyTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    // --- Tail/Legs Texture ---
    image = LoadImage("Assets/legs.png");
    ImageColorReplace(&image, WHITE, BLANK);
    legsTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    // --- Fruit Textures ---
    image = LoadImage("Assets/fruit.png");       // Normal fruit
    ImageColorReplace(&image, WHITE, BLANK);
    fruitTextures[0] = LoadTextureFromImage(image);
    UnloadImage(image);

    image = LoadImage("Assets/red_fruit.png");   // Red fruit
    ImageColorReplace(&image, WHITE, BLANK);
    fruitTextures[1] = LoadTextureFromImage(image);
    UnloadImage(image);

    image = LoadImage("Assets/blue_fruit.png");  // Blue fruit
    ImageColorReplace(&image, WHITE, BLANK);
    fruitTextures[2] = LoadTextureFromImage(image);
    UnloadImage(image);

    image = LoadImage("Assets/orange_fruit.png"); // Orange fruit
    ImageColorReplace(&image, WHITE, BLANK);
    fruitTextures[3] = LoadTextureFromImage(image);
    UnloadImage(image);

    image = LoadImage("Assets/purple_fruit.png"); // Purple fruit
    ImageColorReplace(&image, WHITE, BLANK);
    fruitTextures[4] = LoadTextureFromImage(image);
    UnloadImage(image);

    // --- Fence Texture ---
    image = LoadImage("Assets/fence.png");
    ImageColorReplace(&image, WHITE, BLANK);
    fenceTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    // --- Font ---
    myFont = LoadFont("Assets/stickman.ttf");
}

// === DRAW GREEN CHECKERBOARD TILES ===
void DrawGreenTiles(int screenHeightParam, int screenWidthParam, int tileSizeParam, Color light, Color dark)
{
    // Loop through all rows
    for (int i = whiteHeight; i < screenHeightParam; i += tileSizeParam)
    {
        // Loop through all columns
        for (int j = 0; j < screenWidthParam; j += tileSizeParam)
        {
            // Alternate colors for checkerboard pattern
            if ((i / tileSizeParam + j / tileSizeParam) % 2 == 0)
                DrawRectangle(j, i, tileSizeParam, tileSizeParam, light);
            else
                DrawRectangle(j, i, tileSizeParam, tileSizeParam, dark);
        }
    }
}

// === DRAW TITLE SCREEN TEXT ===
void DrawTitleText(void)
{
    DrawTextEx(myFont, "Press ENTER", (Vector2) { 300, 515 }, 64, 2, lightGreen);      // Instruction
    DrawTextEx(myFont, "Pause = P", (Vector2) { 10, 100 }, 32, 2, lightGreen);        // Pause info
    DrawTextEx(myFont, "THE SNAKEMAN", (Vector2) { 235, 190 }, 80, 2, RAYWHITE);      // Game title
    DrawTextEx(myFont, "Press ESC to quit", (Vector2) { 365, 600 }, 32, 2, lightGreen); // Quit instruction
}

// === DRAW GAMEPLAY TEXT ===
void DrawGameplayText(void)
{
    DrawTextEx(myFont, TextFormat("Score : %i", score), (Vector2) { 15, 15 }, 44, 2, black);          // Current score
    DrawTextEx(myFont, TextFormat("High Score : %i", highScore), (Vector2) { 665, 15 }, 44, 2, black); // High score
    DrawTextEx(myFont, TextFormat("Last Score : %i", lastScore), (Vector2) { 300, 15 }, 44, 2, black); // Last score
}

// === DRAW PAUSE SCREEN TEXT ===
void DrawPauseText(void)
{
    DrawTextEx(myFont, TextFormat("Score : %i", score), (Vector2) { 15, 15 }, 44, 2, RAYWHITE);
    DrawTextEx(myFont, TextFormat("High Score : %i", highScore), (Vector2) { 665, 15 }, 44, 2, RAYWHITE);
    DrawTextEx(myFont, TextFormat("Last Score : %i", lastScore), (Vector2) { 300, 15 }, 44, 2, RAYWHITE);
    DrawTextEx(myFont, "Press ENTER to continue", (Vector2) { 160, 515 }, 64, 2, lightGreen);
    DrawTextEx(myFont, "Press ESC to quit", (Vector2) { 365, 600 }, 32, 2, lightGreen);
}

// === DRAW ENDING SCREEN TEXT ===
void DrawEndingText(void)
{
    DrawTextEx(myFont, "Sorry you lost", (Vector2) { 270, 190 }, 80, 2, RAYWHITE);              // Losing message
    DrawTextEx(myFont, "Press ENTER to retry", (Vector2) { 195, 515 }, 64, 2, lightGreen);      // Retry instruction
    DrawTextEx(myFont, TextFormat("Your score was %i", score), (Vector2) { 305, 335 }, 50, 2, RAYWHITE); // Display final score
    DrawTextEx(myFont, TextFormat("High Score : %i", highScore), (Vector2) { 335, 400 }, 50, 2, RAYWHITE); // High score
    DrawTextEx(myFont, "Press ESC to quit", (Vector2) { 365, 600 }, 32, 2, lightGreen);        // Quit instruction
}

// === AUDIO PLAYBACK FUNCTIONS ===
void PlayTitleAudio(void)
{
    if (firstFrameTitle) // First frame of the title screen
    {
        gameMusic[0].looping = true;       // Loop title music
        PlayMusicStream(gameMusic[0]);     // Play title music
        firstFrameTitle = false;           // Mark as played
    }
    UpdateMusicStream(gameMusic[0]);       // Update music stream
}

void PlayGameplayAudio(void)
{
    if (playMusicGameplay == -1) // If gameplay music hasn't started yet
    {
        StopMusicStream(gameMusic[0]);                // Stop title music
        playMusicGameplay = GetRandomValue(2, 3);    // Randomly pick gameplay track
        gameMusic[playMusicGameplay].looping = true; // Loop selected music
        PlayMusicStream(gameMusic[playMusicGameplay]);
    }
    UpdateMusicStream(gameMusic[playMusicGameplay]); // Update music
}

void PlayPauseAudio(void)
{
    if (playMusicPause == -1) // If pause music hasn't started
    {
        PauseMusicStream(gameMusic[playMusicGameplay]); // Pause gameplay music
        playMusicPause = 1;                             // Index of pause music
        gameMusic[playMusicPause].looping = true;       // Loop pause music
        PlayMusicStream(gameMusic[playMusicPause]);     // Play pause music
    }
    UpdateMusicStream(gameMusic[playMusicPause]);
}

void PlayEndingAudio(void)
{
    if (playMusicEnding == -1) // If ending music hasn't started
    {
        StopSound(gameSound[0]);                          // Stop any sound effects
        StopMusicStream(gameMusic[playMusicGameplay]);   // Stop gameplay music
        playMusicEnding = GetRandomValue(4, 5);          // Randomly pick ending music
        gameMusic[playMusicEnding].looping = true;       // Loop ending music
        PlayMusicStream(gameMusic[playMusicEnding]);     // Play ending music
    }
    UpdateMusicStream(gameMusic[playMusicEnding]);
}

// === LOAD ALL GAME RESOURCES ===
void LoadGameRessources(void)
{
    SetAudio();        // Load all sounds and music
    SetGameTextures(); // Load all textures and font
}

// === FREE AUDIO RESOURCES ===
void FreeMusic(void)
{
    // Unload all music streams
    for (int i = 0; i < MUSIC_NUMBER; i++)
    {
        UnloadMusicStream(gameMusic[i]);
    }
    // Unload all sound effects
    for (int i = 0; i < SOUND_NUMBER; i++)
    {
        UnloadSound(gameSound[i]);
    }
}
