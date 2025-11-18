#include <cmath>
#include <cstdlib>
#include <vector>

#include "CuerpoCeleste.h"
#include "raylib.h"
#include "raymath.h"

// Declaraciones de funciones de Physics.cpp
void UpdateBodies(std::vector<CuerpoCeleste>& bodies, float deltaTime);
float CalculateTotalEnergy(const std::vector<CuerpoCeleste>& bodies);
Vector3 CalculateTotalAngularMomentum(const std::vector<CuerpoCeleste>& bodies);

int main() {
#ifdef __linux__
  setenv("LIBGL_ALWAYS_SOFTWARE", "1", 1);
#endif

  const int screenWidth = 1920;
  const int screenHeight = 1080;

  InitWindow(screenWidth, screenHeight, "Sistema Solar 3D - RK4 2-Cuerpos");
  SetTargetFPS(60);

  // Cámara 3D
  Camera3D camera = {0};
  camera.position = (Vector3){40.0f, 30.0f, 40.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  // CONSTANTES BIEN AJUSTADAS
  const float G = 0.1f;           // Constante gravitacional
  const float sunMass = 1000.0f;  // Masa del Sol (grande)

  std::vector<CuerpoCeleste> bodies;

  // SOL - Centro fijo
  bodies.push_back(CuerpoCeleste({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
                                 sunMass, 2.0f, YELLOW));

  // Función lambda para calcular velocidad circular
  auto calcCircularSpeed = [&](float radius) {
    return sqrtf(G * sunMass / radius);
  };

  // MERCURIO - Órbita más cercana
  float r = 4.0f;
  bodies.push_back(CuerpoCeleste(
      {r, 0.0f, 0.0f},
      {0.0f, 0.0f, calcCircularSpeed(r)},  // ✅ Velocidad circular pura
      0.055f, 0.35f, GRAY));

  // VENUS
  r = 6.5f;
  bodies.push_back(CuerpoCeleste({r, 0.0f, 0.0f},
                                 {0.0f, 0.0f, calcCircularSpeed(r)}, 0.815f,
                                 0.48f, ORANGE));

  // TIERRA
  r = 9.0f;
  bodies.push_back(CuerpoCeleste(
      {r, 0.0f, 0.0f}, {0.0f, 0.0f, calcCircularSpeed(r)}, 1.0f, 0.5f, BLUE));

  // MARTE
  r = 12.0f;
  bodies.push_back(CuerpoCeleste(
      {r, 0.0f, 0.0f}, {0.0f, 0.0f, calcCircularSpeed(r)}, 0.107f, 0.4f, RED));

  // JÚPITER
  r = 18.0f;
  bodies.push_back(CuerpoCeleste({r, 0.0f, 0.0f},
                                 {0.0f, 0.0f, calcCircularSpeed(r)}, 317.8f,
                                 1.2f, Color{200, 170, 130, 255}));

  // SATURNO
  r = 25.0f;
  bodies.push_back(CuerpoCeleste({r, 0.0f, 0.0f},
                                 {0.0f, 0.0f, calcCircularSpeed(r)}, 95.2f,
                                 1.0f, Color{230, 200, 150, 255}));

  // URANO
  r = 32.0f;
  bodies.push_back(CuerpoCeleste({r, 0.0f, 0.0f},
                                 {0.0f, 0.0f, calcCircularSpeed(r)}, 14.5f,
                                 0.8f, Color{130, 200, 220, 255}));

  // NEPTUNO
  r = 40.0f;
  bodies.push_back(CuerpoCeleste({r, 0.0f, 0.0f},
                                 {0.0f, 0.0f, calcCircularSpeed(r)}, 17.1f,
                                 0.8f, Color{50, 100, 200, 255}));

  // Control de tiempo
  float timeScale = 1.0f;
  bool paused = false;

  // Trayectorias
  const int MAX_TRAIL_POINTS = 600;
  std::vector<std::vector<Vector3>> trails(bodies.size());
  int trailUpdateCounter = 0;
  bool showTrails = true;
  bool showGrid = true;
  bool showStars = true;

  // Energía inicial
  float initialEnergy = CalculateTotalEnergy(bodies);
  float initialAngularMomentum =
      Vector3Length(CalculateTotalAngularMomentum(bodies));

  // Main loop
  while (!WindowShouldClose()) {
    UpdateCamera(&camera, CAMERA_FREE);

    // Controles
    if (IsKeyPressed(KEY_P)) paused = !paused;
    if (IsKeyPressed(KEY_KP_ADD) || IsKeyPressed(KEY_EQUAL)) {
      timeScale *= 1.5f;
      if (timeScale > 10.0f) timeScale = 10.0f;
    }
    if (IsKeyPressed(KEY_KP_SUBTRACT) || IsKeyPressed(KEY_MINUS)) {
      timeScale /= 1.5f;
      if (timeScale < 0.1f) timeScale = 0.1f;
    }
    if (IsKeyPressed(KEY_R)) timeScale = 1.0f;
    if (IsKeyPressed(KEY_T)) showTrails = !showTrails;
    if (IsKeyPressed(KEY_G)) showGrid = !showGrid;
    if (IsKeyPressed(KEY_S)) showStars = !showStars;
    if (IsKeyPressed(KEY_C)) {
      for (auto& trail : trails) trail.clear();
    }

    // Actualizar física
    if (!paused) {
      float dt = GetFrameTime() * timeScale * 2.0f;  // Factor de tiempo
      if (dt > 0.04f) dt = 0.04f;

      UpdateBodies(bodies, dt);

      // Actualizar trayectorias
      trailUpdateCounter++;
      if (trailUpdateCounter >= 2) {
        trailUpdateCounter = 0;
        for (size_t i = 1; i < bodies.size(); i++) {
          trails[i].push_back(bodies[i].position);
          if (trails[i].size() > MAX_TRAIL_POINTS) {
            trails[i].erase(trails[i].begin());
          }
        }
      }
    }

    // Métricas físicas
    float totalEnergy = CalculateTotalEnergy(bodies);
    Vector3 angularMomentum = CalculateTotalAngularMomentum(bodies);
    float angularMagnitude = Vector3Length(angularMomentum);

    float energyDrift =
        fabs((totalEnergy - initialEnergy) / initialEnergy) * 100.0f;
    float angularDrift = fabs((angularMagnitude - initialAngularMomentum) /
                              initialAngularMomentum) *
                         100.0f;

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera);

    // Estrellas
    if (showStars) {
      for (int i = 0; i < 200; i++) {
        float x = ((i * 73) % 200 - 100) * 1.2f;
        float y = ((i * 37) % 100 - 50) * 0.6f;
        float z = ((i * 97) % 200 - 100) * 1.2f;
        float brightness = ((i * 47) % 100) / 100.0f;
        DrawSphere((Vector3){x, y, z}, 0.1f,
                   Fade(WHITE, 0.3f + brightness * 0.4f));
      }
    }

    // Grid curvado
    if (showGrid) {
      const int gridSize = 60;
      const float gridSpacing = 1.0f;
      const float maxWarpDist = 20.0f;

      for (int x = -gridSize; x <= gridSize; x += 3) {
        for (int z = -gridSize; z <= gridSize; z += 3) {
          float posX = x * gridSpacing;
          float posZ = z * gridSpacing;
          float dist = sqrtf(posX * posX + posZ * posZ);

          float warp = 0.0f;
          if (dist < maxWarpDist && dist > 0.1f) {
            warp = -2.5f * (1.0f - dist / maxWarpDist) *
                   (1.0f - dist / maxWarpDist);
          }

          Vector3 p1 = {posX, warp, posZ};

          if (x < gridSize) {
            float posX2 = (x + 3) * gridSpacing;
            float dist2 = sqrtf(posX2 * posX2 + posZ * posZ);
            float warp2 = 0.0f;
            if (dist2 < maxWarpDist && dist2 > 0.1f) {
              warp2 = -2.5f * (1.0f - dist2 / maxWarpDist) *
                      (1.0f - dist2 / maxWarpDist);
            }
            Vector3 p2 = {posX2, warp2, posZ};
            float alpha = dist < maxWarpDist ? 0.6f : 0.35f;
            DrawLine3D(p1, p2, Fade(DARKGRAY, alpha));
          }

          if (z < gridSize) {
            float posZ2 = (z + 3) * gridSpacing;
            float dist3 = sqrtf(posX * posX + posZ2 * posZ2);
            float warp3 = 0.0f;
            if (dist3 < maxWarpDist && dist3 > 0.1f) {
              warp3 = -2.5f * (1.0f - dist3 / maxWarpDist) *
                      (1.0f - dist3 / maxWarpDist);
            }
            Vector3 p3 = {posX, warp3, posZ2};
            float alpha = dist < maxWarpDist ? 0.6f : 0.35f;
            DrawLine3D(p1, p3, Fade(DARKGRAY, alpha));
          }
        }
      }
    }

    // Trayectorias
    if (showTrails) {
      for (size_t i = 1; i < trails.size(); i++) {
        const auto& trail = trails[i];
        if (trail.size() > 1) {
          Color trailColor = bodies[i].color;
          for (size_t j = 1; j < trail.size(); j++) {
            float alpha = (float)j / trail.size();
            Color fadeColor = trailColor;
            fadeColor.a = (unsigned char)(alpha * 140);
            DrawLine3D(trail[j - 1], trail[j], fadeColor);
          }
        }
      }
    }

    // Planetas
    for (size_t i = 0; i < bodies.size(); i++) {
      const auto& body = bodies[i];

      if (i == 0) {  // Sol con glow
        DrawSphere(body.position, body.radius * 1.4f, Fade(YELLOW, 0.25f));
        DrawSphere(body.position, body.radius * 1.2f, Fade(ORANGE, 0.45f));
      }

      DrawSphere(body.position, body.radius, body.color);
    }

    EndMode3D();

    // UI
    DrawRectangle(5, 5, 720, 260, Fade(BLACK, 0.87f));
    DrawFPS(10, 10);
    DrawText("SISTEMA SOLAR 3D - RK4 (Fisica 2-Cuerpos)", 10, 30, 21, YELLOW);
    DrawLine(10, 52, 715, 52, DARKGRAY);

    DrawText("Controles:", 10, 58, 17, SKYBLUE);
    DrawText("WASD + Mouse: Camara | P: Pausa | +/-: Velocidad | R: Reset", 10,
             78, 14, WHITE);
    DrawText("T: Trayectorias | G: Grid | S: Estrellas | C: Limpiar", 10, 96,
             14, LIGHTGRAY);
    DrawLine(10, 114, 715, 114, DARKGRAY);

    const char* status = paused       ? "[PAUSADO]"
                         : showTrails ? "[Trayectorias ON]"
                                      : "[Normal]";
    DrawText(TextFormat("Velocidad: x%.2f %s", timeScale, status), 10, 120, 16,
             paused ? RED : GREEN);

    DrawText("Metricas Fisicas:", 10, 142, 17, SKYBLUE);

    Color energyColor =
        energyDrift < 1.0f ? GREEN : (energyDrift < 5.0f ? YELLOW : RED);
    DrawText(
        TextFormat("Energia: %.2f (drift: %.3f%%)", totalEnergy, energyDrift),
        10, 162, 13, energyColor);

    Color angularColor =
        angularDrift < 1.0f ? GREEN : (angularDrift < 5.0f ? YELLOW : RED);
    DrawText(TextFormat("Momento Angular: %.2f (drift: %.3f%%)",
                        angularMagnitude, angularDrift),
             10, 180, 13, angularColor);

    DrawText("Integracion: RK4 (4º orden) | Solo gravedad solar", 10, 198, 13,
             LIGHTGRAY);
    DrawText("Orbitas circulares estables | Timestep adaptativo", 10, 216, 12,
             DARKGRAY);
    DrawText(TextFormat("Cuerpos: %d | G=%.2f | M_sol=%.0f", (int)bodies.size(),
                        G, sunMass),
             10, 234, 12, DARKGRAY);

    // Panel lateral
    DrawRectangle(screenWidth - 205, 5, 200, 350, Fade(BLACK, 0.87f));
    DrawText("Planetas:", screenWidth - 195, 10, 17, SKYBLUE);
    DrawLine(screenWidth - 195, 30, screenWidth - 15, 30, DARKGRAY);

    const char* planetNames[] = {"Sol",     "Mercurio", "Venus",
                                 "Tierra",  "Marte",    "Jupiter",
                                 "Saturno", "Urano",    "Neptuno"};

    int yPos = 38;
    for (size_t i = 0; i < bodies.size() && i < 9; i++) {
      float distance = Vector3Distance(bodies[0].position, bodies[i].position);
      float speed = Vector3Length(bodies[i].velocity);

      DrawText(planetNames[i], screenWidth - 195, yPos, 15, bodies[i].color);
      yPos += 17;

      if (i > 0) {
        DrawText(TextFormat("  R: %.1f", distance), screenWidth - 195, yPos, 12,
                 LIGHTGRAY);
        yPos += 14;
        DrawText(TextFormat("  V: %.2f", speed), screenWidth - 195, yPos, 12,
                 DARKGRAY);
        yPos += 20;
      } else {
        yPos += 8;
      }
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}