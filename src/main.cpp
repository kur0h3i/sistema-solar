#include "raylib.h"

int main() {
  // Inicialización
  const int screenWidth = 1280;
  const int screenHeight = 720;

  InitWindow(screenWidth, screenHeight, "Sistema Solar");
  SetTargetFPS(60);

  // Configurar cámara 3D
  Camera3D camera = {0};
  camera.position = (Vector3){10.0f, 10.0f, 10.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  // Main loop
  while (!WindowShouldClose()) {
    // Update
    UpdateCamera(&camera, CAMERA_FREE);

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera);
    // Dibuja una esfera de prueba en el centro
    DrawSphere((Vector3){0.0f, 0.0f, 0.0f}, 1.0f, YELLOW);
    DrawGrid(20, 1.0f);
    EndMode3D();

    DrawFPS(10, 10);
    DrawText("WASD + Mouse to move camera", 10, 30, 20, WHITE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}