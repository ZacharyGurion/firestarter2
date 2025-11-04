#include <raylib.h>
#include <iostream>

using namespace std;

int main() {
  cout << "Test" << endl;
  InitWindow(800, 600, "Test");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Hello World!", 190, 200, 20, BLACK);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
