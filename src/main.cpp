#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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

double frecventa[20];
double medie = 0;
double mediana = 0;
double deviatia = 0;

int zar1 = 0, zar2 = 0;
int punct1=0;
int punct2=0;
int rezultatcraps1=0;
int rezultatcraps2=0;

int zarY[5] = {0,0,0,0,0};
int pastratY[5] = {0,0,0,0,0};
int aruncariY = 0;
int stareY = 0;
int scorY = 0;

// buton
bool buton(Rectangle rec, const char *text)
{
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rec);
    bool click = hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    Color col = hover ? BLUE : BLACK;
    DrawRectangleRounded(rec, 0.2f, 30, col);
    DrawRectangleRoundedLines(rec, 0.2f, 30, WHITE);

    int fontSize = 30;
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

void Craps(int *punct, int *rezultatcraps, int *z1, int *z2)
{
    *z1 = rand() % 6 + 1;
    *z2 = rand() % 6 + 1;
    int suma = *z1 + *z2;

    if (*punct ==0){
        if(suma==7 ||suma ==11)
            *rezultatcraps=1;
            else if(suma ==2 ||suma ==3 || suma ==12)
            *rezultatcraps=-1;
            else{
                *punct=suma;
                *rezultatcraps=0;
            }
    }else{
        if(suma==*punct){
            *rezultatcraps=1;
            *punct=0;
        }
        else if (suma==7){
            *rezultatcraps=-1;
            *punct=0;
        }
        else *rezultatcraps=0;
    }
}
void aruncareyahtzee()
{
    if (aruncariY >=3 )return;
    for (int i=0;i<5;i++)
    {
        if (pastratY[i]==0)
        {
            zarY[i]=rand()%6+1;
        }
    }
    aruncariY++;
}
void resetareyahtzee()
{
    for (int i=0;i<5;i++)
    {
        pastratY[i]=0;
        zarY[i]=0;
    }
    aruncariY=0;
    stareY=0;
}
void CalculeazaScorYahtzee()
{
    scorY = 0;
    for (int i = 0; i < 5; i++)
        scorY += zarY[i];

    int toateEgale = 1;
    for (int i = 1; i < 5; i++)
    {
        if (zarY[i] != zarY[0])
        {
            toateEgale = 0;
            break;
        }
    }
    if (toateEgale)
    {
        scorY = 50;
    }
}

void statisticiZaruri()
{
    if (countrezultat ==0) return ;

    int tempfrecventa[20] = {0};
    int i;
    double suma =0;
    for (i=0;i<countrezultat;i++)
    {
        tempfrecventa[rezultatzar[i]-1]++;
        suma += rezultatzar[i];
    }
    for (i=0;i<20;i++)
    {
        frecventa[i]=(double)tempfrecventa[i];
    }

    medie = suma / countrezultat;
    
        int temp[countrezultat];
    for (i = 0; i < countrezultat; i++) temp[i] = rezultatzar[i];
    for (i = 0; i < countrezultat-1; i++) {
        for (int j = 0; j < countrezultat-i-1; j++) {
            if (temp[j] > temp[j+1]) {
                int t = temp[j]; temp[j] = temp[j+1]; temp[j+1] = t;
            }
        }
    }
    if (countrezultat % 2 == 0)
        mediana = (temp[countrezultat/2 -1] + temp[countrezultat/2]) / 2.0;
    else
        mediana = temp[countrezultat/2];
    
    deviatia =0;
    for (i=0;i<countrezultat;i++)
    {
        deviatia += (rezultatzar[i]-medie)*(rezultatzar[i]-medie);
        deviatia = sqrt (deviatia / countrezultat);
    }
}
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

    Rectangle b1 = { 450-150, 120, 300, 50};
    Rectangle b2 = { 450-150, 185, 300, 50 };
    Rectangle b3 = { 450-150, 250, 300, 50 };
    Rectangle b4 = { 450-150, 315, 300, 50 };
    Rectangle b5 = { 450-150, 380, 300, 50 };
    Rectangle b6 = { 450-150, 445, 300, 50 };
    Rectangle b7 = { 450-150, 510, 300, 50 };
    Rectangle b8 = { 450-150, 575, 300, 50 };

    Rectangle rollBtn = { 450-125, 300, 250, 50 };
    Rectangle rollBtn2 = { 450-125, 400, 250, 50 };
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

    Rectangle resetc={ 450-125, 370, 250, 50 };
    Rectangle resety={ 450-125, 470, 250, 50 };



    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(texture, 0, 0, WHITE);
        DrawRectangle(0,0,900,600,(Color){0,0,0,100});

        if (screen == SCREEN_MENU)
        {   
            DrawTextEx(titlufont, "Simulator de zaruri", Vector2{ 230, 10 }, 48, 2, DARKGRAY); 
            DrawTextEx(titlufont," si jocuri de noroc ",Vector2{230,50},48,2,DARKBLUE);
            DrawTextEx(font, "Stoian Denis", Vector2{775,670},20,2,WHITE);

            if (buton(b1, "  Simulare zaruri")) screen = SIMULARE_ZARURI;
            if (buton(b2, "Probabilitate suma")) screen = PROBABILITATE_SUMA;
            if (buton(b3, "     Joc Craps")) screen = CRAPS;
            if (buton(b4, "    Joc Yahtzee")) screen = YAHTZEE;
            if (buton(b5, "  Statistici zaruri")) screen = STATISTICI_ZARURI;
            if (buton(b6, "     Salvare log")) screen = SALVARE_LOG;
            if (buton(b7, "     Histograma")) screen = HISTOGRAMA;
            if (buton(b8, "     Comparatie")) screen = COMPARATIE_PROB_TEORICE;
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

        if (screen == CRAPS){
            DrawTextEx(titlufont, "Joc Craps", {300,20}, 60, 2, WHITE);
            DrawTextEx(font, "Reguli:", Vector2{400,430},40,2,WHITE);
            DrawTextEx(font,"Se arunca 2 zaruri.\n Prima aruncare:\n 7 sau 11 castig\n 2, 3, 12 pierdere\n 4,5,6,8,9,10 devine Punct",Vector2 {100,480},30,2,WHITE);
            DrawTextEx(font,"Dupa Punct:\n Punct  castig \n 7 pierdere",Vector2 {500,480},30,2,WHITE);

            DrawTextEx(font, TextFormat("Zaruri: %d + %d", zar1, zar2),
                    {350, 120}, 40, 2, WHITE);

            DrawTextEx(font, TextFormat("Punct curent: %d", punct1),
                    {350, 170}, 40, 2, WHITE);

            if (rezultatcraps1 == 0)
                DrawTextEx(font, "Stare joc: In desfasurare",
                        {300, 220}, 40, 2, WHITE);
            else if (rezultatcraps1 == 1)
                DrawTextEx(font, "Stare joc: Ai castigat!",
                        {300, 220}, 40, 2, GREEN);
            else
                DrawTextEx(font, "Stare joc: Ai pierdut!",
                        {300, 220}, 40, 2, RED);

            if (buton(rollBtn, "Arunca zarurile") && rezultatcraps1 == 0)
                Craps(&punct1, &rezultatcraps1, &zar1, &zar2);

            if (buton(backBtn, "<- Inapoi")) screen = SCREEN_MENU;

            if (buton(resetc,"Reset")) rezultatcraps1 = 0;
        }
        if (screen == YAHTZEE)
        {
            DrawTextEx(titlufont, "Joc Yahtzee", {280,20}, 60, 2, WHITE);
            DrawTextEx(font, TextFormat("Scor: %d", scorY), {400,100}, 40, 2, WHITE);

            for (int i=0;i<5;i++)
            {
                DrawRectangle(90 + i*150, 150, 130, 130, LIGHTGRAY);
                DrawRectangleLines(90 + i*150, 150, 130, 130, BLACK);
                if (zarY[i]!=0)
                {
                    DrawTextEx(font, TextFormat("%d", zarY[i]),
                               { (float)(150 + (i*150)), 200 }, 40, 2, DARKBLUE);
                }

                Rectangle btnPastrat = {(float)(110 + i*150), 300, 90, 40};
                if (buton(btnPastrat, pastratY[i] ? "  Da" : " Tine"))
                {
                    pastratY[i] = !pastratY[i];
                }
            }

            DrawTextEx(font,
                TextFormat("Aruncari: %d / 3", aruncariY),
                {300, 350}, 35, 2, WHITE);
            if (aruncariY > 0)
                CalculeazaScorYahtzee();

            if (buton(rollBtn2, "Arunca zarurile"))
                aruncareyahtzee();

            if (buton(resety, "      Reset"))
                resetareyahtzee();

            if (buton(backBtn, "<- Inapoi"))
                screen = SCREEN_MENU;


        }
        if (screen == STATISTICI_ZARURI)
        {
            statisticiZaruri();
            DrawTextEx(titlufont, "Statistici Zaruri", {220,20}, 60, 2, WHITE);
            DrawTextEx(font, TextFormat("Numar total aruncari: %d", countrezultat), {300, 100}, 40, 2, WHITE);
            DrawTextEx(font, "Statistici calculate:", {50,250}, 30, 2, WHITE);
            DrawTextEx(font, TextFormat("Medie: %.2f", medie), {50,300}, 30, 2, WHITE);
            DrawTextEx(font, TextFormat("Mediana: %.2f", mediana), {50,350}, 30, 2, WHITE);
            DrawTextEx(font, TextFormat("Deviatia standard: %.2f", deviatia), {50,400}, 30, 2, WHITE);
            DrawTextEx(font,TextFormat ("Frecventa: %.2f", frecventa[0]),{50,450},30,2,WHITE);

            if (buton(backBtn, "<- Inapoi")) screen = SCREEN_MENU;
        }
        if (screen == HISTOGRAMA)
        {
            histograma();
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
