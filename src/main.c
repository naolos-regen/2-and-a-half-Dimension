#include "raylib.h"
#include <math.h>
#include <stdio.h>

#define screenWidth 1024
#define screenHeight 860
#define mapWidth 24
#define mapHeight 24

int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};// Player variables


float playerX = 10.0f, playerY = 10.0f; // Starting position
float dirX = -1.0f, dirY = 0.0f;        // Initial direction vector
float planeX = 0.0f, planeY = 0.66f;    // Camera plane (FOV)

// Movement speed
const float moveSpeed = 0.1f;
float cameraPitch = 0.0f;
float maxPitch = 0.90f;
const float rotSpeed = 0.05f;

void UpdatePlayer();
void RenderScene();
void UpdateCameraPitch();
void ColisionCheck();

int main(void)
{
    InitWindow(screenWidth, screenHeight, "2.5D Raycasting Game");
    SetMousePosition(screenWidth / 2, screenHeight / 2);
    DisableCursor();
    SetTargetFPS(65);

    while (!WindowShouldClose())
    {
        UpdatePlayer();

        BeginDrawing();
        ClearBackground(BLACK);

        RenderScene();

        DrawFPS(10, 10);
        printf("p_x : %f, p_y: %f\n", playerX, playerY);
        EndDrawing();
    }

    CloseWindow();
    EnableCursor();

    return 0;
}

int CollisionCheck(float newX, float newY) {
    if (worldMap[(int)newX][(int)newY] > 0) {
       return 1;
    }
    return 0;
}

void UpdatePlayer() {
    float nextX = playerX;
    float nextY = playerY;

    // Move Upwards
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        nextX = playerX + dirX * moveSpeed;
        nextY = playerY + dirY * moveSpeed;

        if (CollisionCheck(nextX, nextY)) {

            if (!CollisionCheck(playerX + dirX * 0.25f, playerY)) {
                playerX += dirX * 0.25f;
            }
            if (!CollisionCheck(playerX, playerY + dirY * 0.25f)) {
                playerY += dirY * 0.25f;
            }
        } else {
               playerX = nextX;
               playerY = nextY;
         }
     }

    // Move backward (S or DOWN)
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        nextX = playerX - dirX * moveSpeed;
        nextY = playerY - dirY * moveSpeed;

        if (CollisionCheck(nextX, nextY)) {
            if (!CollisionCheck(playerX - dirX * 0.25f, playerY)) {
                playerX -= dirX * 0.25f;
            }
            if (!CollisionCheck(playerX, playerY - dirY * 0.25f)) {
                playerY -= dirY * 0.25f;
            }
        } else {
            playerX = nextX;
            playerY = nextY;
        }
    }

    if (IsKeyDown(KEY_A)) {
        nextX = playerX - planeX * moveSpeed;
        nextY = playerY - planeY * moveSpeed;
        if (CollisionCheck(nextX, nextY)) {
            if (!CollisionCheck(playerX - planeX * 0.25f, playerY)) {
                playerX -= planeX * 0.25f;
            }
        } else {
            playerX = nextX;
            playerY = nextY;
        }
    }


    if (IsKeyDown(KEY_D)) {
        nextX = playerX + planeX * moveSpeed;
        nextY = playerY + planeY * moveSpeed;
        if (CollisionCheck(nextX, nextY)) {
            if (!CollisionCheck(playerX + planeX * 0.25f, playerY)) {
                playerX += planeX * 0.25f;
            }
        } else {
            playerX = nextX;
            playerY = nextY;
        }
    }

    int center_x = screenWidth / 2;
    int center_y = screenHeight / 2;
    Vector2 mouse = GetMousePosition();

    float mouseDeltaX = (mouse.x - center_x) * rotSpeed * 0.010f;
    float mouseDeltaY = (mouse.y - center_y) * 0.010f;

    cameraPitch += mouseDeltaY * 0.03f;
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;

    if (mouseDeltaX != 0) {
        float oldDirX = dirX;
        dirX = dirX * cos(-mouseDeltaX) - dirY * sin(-mouseDeltaX);
        dirY = oldDirX * sin(-mouseDeltaX) + dirY * cos(-mouseDeltaX);

        float oldPlaneX = planeX;
        planeX = planeX * cos(-mouseDeltaX) - planeY * sin(-mouseDeltaX);
        planeY = oldPlaneX * sin(-mouseDeltaX) + planeY * cos(-mouseDeltaX);
    }

    SetMousePosition(center_x, center_y);


    if (IsKeyDown(KEY_LEFT)) {
        float oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
        float oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }

    if (IsKeyDown(KEY_RIGHT)) {
        float oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
        float oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
}


void RenderScene()
{

  for (int x = 0; x < screenWidth; x++) {
    float cameraX = 2 * x / (float)screenWidth - 1;
    float rayDirX = dirX + planeX * cameraX;
    float rayDirY = dirY + planeY * cameraX;

    int mapX = (int)playerX;
    int mapY = (int)playerY;

    float sideDistX;
    float sideDistY;

    float deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
    float deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
    float perpWallDist;

    int stepX, stepY;

    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (playerX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - playerX) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (playerY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - playerY) * deltaDistY;
    }

    int hit = 0;
    int side;

    while (hit == 0) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }
        if (worldMap[mapX][mapY] > 0) hit = 1;
    }

    if (side == 0) perpWallDist = (mapX - playerX + (1.0f - stepX) / 2) / rayDirX;
    else           perpWallDist = (mapY - playerY + (1.0f - stepY) / 2) / rayDirY;

    int lineHeight = (int)(screenHeight / perpWallDist);

    int drawStart = -lineHeight / 2 + screenHeight / 2 - (int)(cameraPitch * screenHeight);
    if (drawStart < 0) drawStart = 0;
    int drawEnd = lineHeight / 2 + screenHeight / 2 - (int)(cameraPitch * screenHeight);
    if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

    Color color;
    switch (worldMap[mapX][mapY]) {
        case 1: color = RED; break;
        case 2: color = GREEN; break;
        case 3: color = BLUE; break;
        case 4: color = WHITE; break;
        case 5: color = YELLOW; break;
        default: color = GRAY; break;
    }

    if (side == 1) color = Fade(color, 0.7f);


    DrawLine(x, drawStart, x, drawEnd, color);

    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    DrawLine(centerX - 10, centerY, centerX + 10, centerY, WHITE);
    DrawLine(centerX, centerY - 10, centerX, centerY + 10, WHITE);
  }
}
