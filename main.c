#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_BULLET 5
#define MAX_ENEMY_BULLET 5
#define MAX_ENEMY_ROW 3
#define MAX_ENEMY_COL 5

typedef struct {
    int x;
    int y;
    bool alive;
} Bullet;

int add_bullet(Bullet *list, int x, int y)
{
    for (int i=0; i < MAX_BULLET; i++) {
        if (!list[i].alive) {
            list[i].x = x;
            list[i].y = y;
            list[i].alive = true;
            return 1;
        }
    }
    return 0;
}

int add_enemy_bullet(Bullet *list, int x, int y)
{
    for (int i=0; i < MAX_ENEMY_BULLET; i++) {
        if (!list[i].alive) {
            list[i].x = x;
            list[i].y = y;
            list[i].alive = true;
            return 1;
        }
    }
    return 0;
}

typedef struct {
    int x;
    int y;
    bool alive;
} Enemy;

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    InitAudioDevice();
    SetTargetFPS(60);
    Sound laser_shot = LoadSound("assets/laser-shot.mp3");
    Sound explosion = LoadSound("assets/explosion.mp3");

    int ship_x = SCREEN_WIDTH/2;
    int ship_y = SCREEN_HEIGHT - 50;
    int ship_width = 50;
    int ship_height = 10;
    int enemy_dx = 3;
    int enemy_dy = 0;
    Vector2 bullet_size = {4, 8};
    Vector2 bullet_speed = {0.0, -5.0};
    Vector2 enemy_bullet_speed = {0.0, 5.0};
    Vector2 enemy_size = {40, 20};
    int points = 0;

    Bullet bullets[MAX_BULLET];
    Bullet enemy_bullets[MAX_ENEMY_BULLET];
    for (int i=0; i < MAX_BULLET; i++) {
        bullets[i].alive = false;
    }

    for (int i=0; i < MAX_ENEMY_BULLET; i++) {
        enemy_bullets[i].alive = false;
    }

    Enemy enemy[MAX_ENEMY_ROW][MAX_ENEMY_COL];
    for (int i=0; i < MAX_ENEMY_ROW; i++) {
        for (int j=0; j < MAX_ENEMY_COL; j++) {
            enemy[i][j].x = 50 + j * 60;
            enemy[i][j].y = 50 + i * 60;
            enemy[i][j].alive = true;
        }
    }

    while(!WindowShouldClose()) {
        for (int i=0; i < MAX_BULLET; i++) {
            bullets[i].x += bullet_speed.x;
            bullets[i].y += bullet_speed.y;

            if (bullets[i].y < 0) {
                bullets[i].alive = false;
            }
        }

        for (int i=0; i < MAX_ENEMY_BULLET; i++) {
            enemy_bullets[i].x += enemy_bullet_speed.x;
            enemy_bullets[i].y += enemy_bullet_speed.y;

            if (enemy_bullets[i].y > SCREEN_HEIGHT) {
                enemy_bullets[i].alive = false;
            }
        }

        int t = GetRandomValue(0, 50);
        if (t < 5) {
            int x = GetRandomValue(10, SCREEN_WIDTH - 10);
            add_enemy_bullet(enemy_bullets, x, 0);
        }

        if (IsKeyDown(KEY_RIGHT)) {
            if (ship_x < (SCREEN_WIDTH - ship_width))
            ship_x += 5;
        }
        if (IsKeyDown(KEY_LEFT)) {
            if (ship_x > 0)
            ship_x -= 5;
        }

        if (IsKeyReleased(KEY_SPACE)) {
            if(add_bullet(bullets, ship_x, ship_y)) {
                PlaySound(laser_shot);
            }
            
        }

        for (int i=0; i < MAX_BULLET; i++) {
            for (int a=0; a < MAX_ENEMY_ROW; a++) {
                for (int b=0; b < MAX_ENEMY_COL; b++) {
                    if (bullets[i].alive && enemy[a][b].alive) {
                        Rectangle r1 = {bullets[i].x, bullets[i].y, bullet_size.x, bullet_size.y};
                        Rectangle r2 = {enemy[a][b].x, enemy[a][b].y, enemy_size.x, enemy_size.y};
                        if (CheckCollisionRecs(r1, r2)) {
                            bullets[i].alive = false;
                            enemy[a][b].alive = false;
                            points++;
                            PlaySound(explosion);
                        }
                    }
                }
            }
        }

        for (int i=0; i < MAX_ENEMY_BULLET; i++) {
            if (enemy_bullets[i].alive) {
                Rectangle r1 = {enemy_bullets[i].x, enemy_bullets[i].y, bullet_size.x, bullet_size.y};
                Rectangle r2 = {ship_x, ship_y, ship_width, ship_height};
                if (CheckCollisionRecs(r1, r2)) {
                    enemy_bullets[i].alive = false;
                    printf("Hit!!!\n");
                }
            }
        }

        for (int i=0; i < MAX_ENEMY_ROW; i++) {
            for (int j=0; j < MAX_ENEMY_COL; j++) {
                enemy[i][j].x -= enemy_dx;
                enemy[i][j].y -= enemy_dy;

                if (enemy[i][j].x < 0 || enemy[i][j].x > SCREEN_WIDTH) {
                    enemy_dx *= -1;
                }
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawText(TextFormat("POINTS: %d", points), 10, 10, 20, GREEN);
            DrawRectangle(ship_x, ship_y, ship_width, ship_height, RED);

            for (int i=0; i < MAX_BULLET; i++) {
                if (bullets[i].alive)
                DrawRectangle(bullets[i].x, bullets[i].y, bullet_size.x, bullet_size.y, RED);
            }

            for (int i=0; i < MAX_ENEMY_BULLET; i++) {
                if (enemy_bullets[i].alive)
                DrawRectangle(enemy_bullets[i].x, enemy_bullets[i].y, bullet_size.x, bullet_size.y, YELLOW);
            }

            for (int a=0; a < MAX_ENEMY_ROW; a++) {
                for (int b=0; b < MAX_ENEMY_COL; b++) {
                    if (enemy[a][b].alive)
                    DrawRectangle(enemy[a][b].x, enemy[a][b].y, enemy_size.x, enemy_size.y, BLUE);
                }
            }
        EndDrawing();
    }

    UnloadSound(explosion);
    UnloadSound(laser_shot);
    CloseWindow();
}
