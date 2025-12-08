#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int rezultat=0;

bool buton(Rectangle rec, const char *text)
{
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rec);
    bool click = hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    DrawRectangleRec(rec, hover ? LIGHTGRAY : GRAY);
    DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, BLACK);
    DrawText(text, rec.x + 10, rec.y + 10, 30, BLACK);

    return click;
}

void SimulareZaruri()
{ 
    printf("Simulare zaruri\n");
    rezultat = (rand() % 6 + 1) + (rand() % 6 + 1);
    printf("Rezultat: %d\n", rezultat);
}
void ProbabilitateSuma()
{ 
    printf("Probabilitate suma\n"); 
}
void Craps() 
{ 
    printf("Joc Craps\n"); 
}
void yahtzee() 
{ 
    printf("Joc Yahtzee\n"); 
}
void statisticiZaruri() 
{ 
    printf("Statistici zaruri\n"); 
}
void salvareLog() 
{ 
    printf("Salvare log\n"); 
}
void histograma() 
{ 
    printf("Histograma\n"); 
}
void comparatieProbTeoretice() 
{ 
    printf("Comparatie probabilitati teoretice\n"); 
}

int main()
{
    InitWindow(900, 600, "Meniu Simulari Zaruri");
    Image img = LoadImage("poza.png");
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    Rectangle b1 = { 450-125, 100, 250, 40 };
    Rectangle b2 = { 450-150, 155, 300, 40 };
    Rectangle b3 = { 450-90, 210, 180, 40 };
    Rectangle b4 = { 450-105, 265, 210, 40 };
    Rectangle b5 = { 450-135, 320, 270, 40 };
    Rectangle b6 = { 450-97, 375, 195, 40 };
    Rectangle b7 = { 450-92, 430, 185, 40 };
    Rectangle b8 = { 450-90, 485, 180, 40 };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(texture, 0, 0, WHITE);


        DrawText("Alege jocul", 240, 20, 60, BLACK);

        if (buton(b1, "Simulare zaruri"))  SimulareZaruri();
        if (buton(b2, "Probabilitate suma")) ProbabilitateSuma();
        if (buton(b3, "Joc Craps")) Craps();
        if (buton(b4, "Joc Yahtzee")) yahtzee();
        if (buton(b5, "Statistici zaruri")) statisticiZaruri();
        if (buton(b6, "Salvare log")) salvareLog();
        if (buton(b7, "Histograma")) histograma();
        if (buton(b8, "Comparatie")) comparatieProbTeoretice();

        EndDrawing();
    }
    UnloadTexture(texture);
    CloseWindow();
    return 0;
}
