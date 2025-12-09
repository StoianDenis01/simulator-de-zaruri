#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int rezultat=0;
int fete =6;
int numar_aruncari=2;

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

typedef enum {
    SCREEN_MENU,
    SIMULARE_ZARURI,
    PROBABILITATE_SUMA,
    CRAPS,
    YAHTZEE,
    STATISTICI_ZARURI,
    SALVARE_LOG,
    HISTOGRAMA,
    COMPARATIE_PROB_TEORICE
} ScreenState;

void SimulareZaruri(int numar_aruncari,int fete)
{
    rezultat=0;
    srand(time(0));
    for(int i=0;i<numar_aruncari;i++)
    {
        rezultat+=rand()%fete+1;
    }
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

    ScreenState screen = SCREEN_MENU;

    Rectangle b1 = { 450-125, 100, 250, 40 };
    Rectangle b2 = { 450-150, 155, 300, 40 };
    Rectangle b3 = { 450-90, 210, 180, 40 };
    Rectangle b4 = { 450-105, 265, 210, 40 };
    Rectangle b5 = { 450-135, 320, 270, 40 };
    Rectangle b6 = { 450-97, 375, 195, 40 };
    Rectangle b7 = { 450-92, 430, 185, 40 };
    Rectangle b8 = { 450-90, 485, 180, 40 };

    Rectangle rollBtn = { 350, 300, 250, 50 };
    Rectangle backBtn = { 20, 20, 150, 40 };

    Rectangle rollAdd= { 675, 100, 40, 40 };
    Rectangle rollSub= { 625, 100, 40, 40 };
    Rectangle faceAdd= { 650, 150, 40, 40 };
    Rectangle faceSub= { 600, 150, 40, 40 };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(texture, 0, 0, WHITE);

        if (screen == SCREEN_MENU)
        {  DrawText("Alege jocul", 240, 20, 60, BLACK);

            if (buton(b1, "Simulare zaruri"))
            {
                screen = SIMULARE_ZARURI;
            };
            if (buton(b2, "Probabilitate suma")) ProbabilitateSuma();
            if (buton(b3, "Joc Craps")) Craps();
            if (buton(b4, "Joc Yahtzee")) yahtzee();
            if (buton(b5, "Statistici zaruri")) statisticiZaruri();
            if (buton(b6, "Salvare log")) salvareLog();
            if (buton(b7, "Histograma")) histograma();
            if (buton(b8, "Comparatie")) comparatieProbTeoretice();
        }
        if (screen == SIMULARE_ZARURI)
        {
                        DrawText("Simulare Zaruri", 230, 40, 60, BLACK);

            DrawText(TextFormat("Rezultat: %d", rezultat), 300, 200, 40, BLACK);
            DrawText(TextFormat("Numar zaruri: %d", numar_aruncari), 300, 100, 40, BLACK);
            DrawText(TextFormat("Fete zaruri: %d", fete), 300, 150, 40, BLACK);
            
            if (buton(rollAdd, "+"))
                numar_aruncari++;

            if (buton(rollSub, "-") && numar_aruncari>1)
                numar_aruncari--;
            
            if (buton(faceAdd, "+"))
                fete++;
            
            if (buton(faceSub, "-") && fete>2)
                fete--;

            if (buton(rollBtn, "Da cu zarurile"))
                SimulareZaruri(numar_aruncari,fete);

            if (buton(backBtn, "<- Inapoi"))
                screen = SCREEN_MENU;
        }

        EndDrawing();
    }
    UnloadTexture(texture);
    CloseWindow();
    return 0;
}
