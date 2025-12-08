#include <raylib.h>

int main()
{
    InitWindow(700,600,"geam");
    
    while(!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(WHITE);
        DrawText("Denis este cel mai tare",150,GetScreenHeight()/2,40,BLACK);
        EndDrawing();
    }

    return 0;
}
