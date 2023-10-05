#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

Rectangle p1_rec = {0};
float p1_speed = 200.0f;
int p1_score = 0;

Rectangle p2_rec = {0};
float p2_speed = 200.0f;
int p2_score = 0;

Vector2 ball = {0};
float ball_radius = 30.0f;
float ball_speed = 250.0f;
Vector2 ball_vel = {0};

void start_round()
{
    p1_rec.x = 20;
    p1_rec.y = HEIGHT / 2 - p1_rec.height / 2 + 75;

    p2_rec.x = WIDTH - 20 - p2_rec.width;
    p2_rec.y = HEIGHT / 2 - p2_rec.height / 2 + 75;

    ball_vel.x = 1.0f;
    ball_vel.y = 0.0f;

    ball.x = WIDTH / 2;
    ball.y = HEIGHT / 2 + 75;
}

void draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    DrawRectangleRec(p1_rec, WHITE);
    DrawRectangleRec(p2_rec, WHITE);

    DrawCircleV(ball, ball_radius, WHITE);

    DrawRectangle(0, 130, WIDTH, 20, WHITE);

    const char *scores = TextFormat("%d - %d", p1_score, p2_score);

    Vector2 size = MeasureTextEx(GetFontDefault(), scores, 60, 0);

    // DrawText(scores, WIDTH / 2 - size.x / 2 - 12, 75 - size.y / 2, 60, WHITE);
    DrawTextEx(GetFontDefault(), scores, (Vector2){WIDTH / 2 - size.x / 2, 75 - size.y / 2}, 60, 0, WHITE);

    EndDrawing();
}

void constrain_paddle(Rectangle *paddle)
{
    if (paddle->y + paddle->height > HEIGHT)
    {
        paddle->y = HEIGHT - paddle->height;
    }

    if (paddle->y < 150)
    {
        paddle->y = 150;
    }
}

bool check_win()
{
    if (p1_score > 9)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Player 1 wins!", 20, 20, 60, WHITE);
        EndDrawing();
        return true;
    }

    if (p2_score > 9)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Player 2 wins!", 20, 20, 60, WHITE);
        EndDrawing();
        return true;
    }

    return false;
}

void update()
{
    if (check_win())
    {
        return;
    }

    if (IsKeyDown(KEY_W))
    {
        p1_rec.y -= GetFrameTime() * p1_speed;
    }

    if (IsKeyDown(KEY_S))
    {
        p1_rec.y += GetFrameTime() * p1_speed;
    }

    if (IsKeyDown(KEY_UP))
    {
        p2_rec.y -= GetFrameTime() * p2_speed;
    }

    if (IsKeyDown(KEY_DOWN))
    {
        p2_rec.y += GetFrameTime() * p2_speed;
    }

    constrain_paddle(&p1_rec);
    constrain_paddle(&p2_rec);

    if (ball_vel.x > 0)
    {
        if (CheckCollisionCircleRec(ball, ball_radius, p2_rec))
        {
            ball_vel.x = ball_vel.x * -1;
            ball_vel.y += (ball.y - (p2_rec.y + p2_rec.height / 2)) / 100;
        }

        if (ball.x + ball_radius > WIDTH)
        {
            p1_score++;
            start_round();
        }
    }

    if (ball_vel.x < 0)
    {
        if (CheckCollisionCircleRec(ball, ball_radius, p1_rec))
        {
            ball_vel.x = ball_vel.x * -1;
            ball_vel.y += (ball.y - (p1_rec.y + p1_rec.height / 2)) / 100;
        }

        if (ball.x - ball_radius < 0)
        {
            p2_score++;
            start_round();
        }
    }

    if ((ball_vel.y > 0 && ball.y + ball_radius > HEIGHT) || (ball_vel.y < 0 && ball.y - ball_radius < 150))
    {
        ball_vel.y = ball_vel.y * -1;
    }

    Vector2Normalize(ball_vel);

    ball.x += ball_vel.x * ball_speed * GetFrameTime();
    ball.y += ball_vel.y * ball_speed * GetFrameTime();

    draw();
}

int main(void)
{
    p1_rec.width = 20;
    p1_rec.height = 100;

    p2_rec.width = 20;
    p2_rec.height = 100;

    InitWindow(WIDTH, HEIGHT, "Mike's Pong");

    SetTargetFPS(60);

    start_round();

    while (!WindowShouldClose())
    {
        update();
    }

    CloseWindow();
}