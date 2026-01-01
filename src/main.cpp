#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Font font;
Font titlufont;

int rezultat=0;
int fete=6;
int numar_aruncari=2;
int sumadorita;
float probabilitate;

int rezultatzar[1000];
int countrezultat=0;

int ultimezaruri[100];
int ultimecount=0;

// Buton modernizat
bool buton(Rectangle rec, const char *text)
{
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rec);
    bool click = hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    Color col = hover ? BLUE : BLACK;
    DrawRectangleRounded(rec, 0.2f, 30, col);
    DrawRectangleRoundedLines(rec, 0.2f, 30, WHITE);

    // text centrat
    int fontSize = 30; // dimensiunea textului pe buton
    Vector2 textSize = MeasureTextEx(font, text, fontSize, 2);
    DrawTextEx(font, text, 
               { rec.x +rec.width/20 - textSize.x/2, rec.y + rec.height/4 - textSize.y/2 },
               fontSize, 2, WHITE);

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
    ultimecount=0;
    srand(time(0));

    for(int i=0;i<numar_aruncari;i++)
    {
        int val=rand()%fete+1;
        rezultat+=val;

        ultimezaruri[ultimecount ++]=val;
        rezultatzar[countrezultat ++]=val;
    }
}

int combinatiiSuma(int zaruri, int fete, int sumaDorita) {
    if (zaruri == 0) return (sumaDorita == 0) ? 1 : 0;

    int total = 0;
    for (int i = 1; i <= fete; i++) {
        if (sumaDorita - i >= 0)
            total += combinatiiSuma(zaruri - 1, fete, sumaDorita - i);
    }
    return total;
}

float calculareaprobabilitatii(int zaruri,int fete,int sumadorita)
{
    int totalcombinatii=1;
    for(int i=0;i<zaruri;i++) totalcombinatii*=fete;

    int combinatiifavorabile = combinatiiSuma(zaruri,fete,sumadorita);
    return (float)combinatiifavorabile/totalcombinatii;
}

void Craps() { printf("Joc Craps\n"); }
void yahtzee() { printf("Joc Yahtzee\n"); }
void statisticiZaruri() { printf("Statistici zaruri\n"); }
void salvareLog() { printf("Salvare log\n"); }
void histograma() { printf("Histograma\n"); }
void comparatieProbTeoretice() { printf("Comparatie probabilitati teoretice\n"); }

int main()
{
    InitWindow(900, 690, "Meniu Simulari Zaruri");
    SetTargetFPS(60);

    font = LoadFontEx("assets/Inter-Regular.ttf", 48, 0, 0);
    titlufont = LoadFontEx("assets/Inter-Bold.ttf", 72, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(titlufont.texture, TEXTURE_FILTER_BILINEAR);

    Image img = LoadImage("assets/poza.png");
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    ScreenState screen = SCREEN_MENU;

    Rectangle b1 = { 450-125, 100, 250, 50};
    Rectangle b2 = { 450-150, 165, 300, 50 };
    Rectangle b3 = { 450-90, 230, 180, 50 };
    Rectangle b4 = { 450-105, 295, 210, 50 };
    Rectangle b5 = { 450-135, 360, 270, 50 };
    Rectangle b6 = { 450-97, 425, 195, 50 };
    Rectangle b7 = { 450-92, 490, 185, 50 };
    Rectangle b8 = { 450-90, 555, 180, 50 };

    Rectangle rollBtn = { 450-125, 300, 250, 60 };
    Rectangle backBtn = { 20, 20, 150, 50 };

    Rectangle rollAdd= { 675, 100, 40, 40 };
    Rectangle rollSub= { 625, 100, 40, 40 };
    Rectangle face6= { 585, 150, 40, 40 };
    Rectangle face8= { 635, 150, 40, 40 };
    Rectangle face10= { 685, 150, 40, 40 };
    Rectangle face12= { 735, 150, 40, 40 };
    Rectangle face20= { 785, 150, 40, 40 };

    Rectangle prob = {450-215,300,430,50};
    Rectangle sumAdd = {675,200,40,40};
    Rectangle sumSub = {625,200,40,40};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Fundal imagine + overlay
        DrawTexture(texture, 0, 0, WHITE);
        DrawRectangle(0,0,900,600,(Color){0,0,0,100});

        if (screen == SCREEN_MENU)
        {   
            DrawTextEx(titlufont, "Simulator de zaruri", Vector2{ 230, 10 }, 48, 2, DARKGRAY); 
            DrawTextEx(titlufont," si jocuri de noroc ",Vector2{230,50},48,2,DARKBLUE);
            DrawTextEx(font, "Stoian Denis", Vector2{775,670},20,2,WHITE);

            if (buton(b1, "Simulare zaruri")) screen = SIMULARE_ZARURI;
            if (buton(b2, "Probabilitate suma")) screen = PROBABILITATE_SUMA;
            if (buton(b3, "Joc Craps")) screen = CRAPS;
            if (buton(b4, "Joc Yahtzee")) screen = YAHTZEE;
            if (buton(b5, "Statistici zaruri")) screen = STATISTICI_ZARURI;
            if (buton(b6, "Salvare log")) screen = SALVARE_LOG;
            if (buton(b7, "Histograma")) screen = HISTOGRAMA;
            if (buton(b8, "Comparatie")) screen = COMPARATIE_PROB_TEORICE;
        }

        if (screen == SIMULARE_ZARURI)
        {
            DrawTextEx(titlufont, "Simulare Zaruri", {220,20}, 60, 2, WHITE);
            DrawTextEx(font, TextFormat("Rezultat: %d", rezultat), {365, 200}, 40, 2, WHITE);
            DrawTextEx(font, TextFormat("Numar zaruri: %d", numar_aruncari), {300, 100}, 40, 2, WHITE);
            DrawTextEx(font, TextFormat("Fete zaruri: %d", fete), {300, 150}, 40, 2, WHITE);

            if (buton(rollAdd, " +")) numar_aruncari++;
            if (buton(rollSub, " -") && numar_aruncari>1) numar_aruncari--;
            if (buton(face6, " 6")) fete=6;
            if (buton(face8, " 8")) fete=8;
            if (buton(face10, "10")) fete=10;
            if (buton(face12, "12")) fete=12;
            if (buton(face20, "20")) fete=20;
            if (buton(rollBtn, "Da cu zarurile")) SimulareZaruri(numar_aruncari,fete);
            if (buton(backBtn, "<- Inapoi")) screen = SCREEN_MENU;
        }

        if (screen == PROBABILITATE_SUMA) 
        {
            DrawTextEx(titlufont, "Probabilitate Suma", {180,20}, 60, 2, WHITE);
            DrawTextEx(font, TextFormat("Numar zaruri: %d", numar_aruncari), {300, 100}, 40, 2, WHITE);
            DrawTextEx(font, TextFormat("Fete zaruri: %d", fete), {300, 150}, 40, 2, WHITE);
            DrawTextEx(font, TextFormat("Suma Dorita: %d",sumadorita), {300, 200}, 40, 2, WHITE);
            DrawTextEx(font, TextFormat("   Probabilitate: %.4f",probabilitate), {200, 250}, 40, 2, WHITE);

            if (buton(prob,"Calculeaza probabilitatea"))
                probabilitate=calculareaprobabilitatii(numar_aruncari,fete,sumadorita);

            if (buton(rollAdd, "+")) numar_aruncari++;
            if (buton(rollSub, "-") && numar_aruncari>1) numar_aruncari--;
            if (buton(sumAdd,"+")) sumadorita++;
            if (buton(sumSub,"-")) sumadorita--;
            if (buton(face6," 6")) fete=6;
            if (buton(face8," 8")) fete=8;
            if (buton(face10,"10")) fete=10;
            if (buton(face12,"12")) fete=12;
            if (buton(face20,"20")) fete=20;

            if (buton(backBtn, "<- Inapoi")) screen = SCREEN_MENU;
        }

        if (screen == SALVARE_LOG)
        {
            DrawTextEx(titlufont, "Salvare Log", {300,20}, 60, 2, WHITE);
            DrawTextEx(font, "Apasa butonul de mai jos pentru a salva log-ul aruncarilor intr-un fisier text.", {50,150}, 30, 2, WHITE);

            if (buton(rollBtn,"Salveaza Log"))
            {
                FILE *f = fopen("log_zaruri.txt", "w");
                if (f == NULL)
                {
                    DrawTextEx(font, "Eroare la deschiderea fisierului!", {200,300}, 30, 2, RED);
                }
                else
                {
                    for (int i = 0; i < countrezultat; i++)
                    {
                        fprintf(f, "Aruncarea %d: %d\n", i + 1, rezultatzar[i]);
                    }
                    fclose(f);
                }
            }

            if (buton(backBtn, "<- Inapoi")) screen = SCREEN_MENU;
        }

        if (screen == CRAPS || screen == YAHTZEE || screen == STATISTICI_ZARURI ||
             screen == HISTOGRAMA || screen == COMPARATIE_PROB_TEORICE)
        {
            DrawTextEx(font, "Functia nu e implementata inca", {200,300}, 40, 2, WHITE);
            if (buton(backBtn, "<- Inapoi")) screen = SCREEN_MENU;
        }

        EndDrawing();
    }

    UnloadTexture(texture);
    UnloadFont(font);
    UnloadFont(titlufont);
    CloseWindow();
    return 0;
}
