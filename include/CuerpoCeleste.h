#ifndef CUERPO_CELESTE_H
#define CUERPO_CELESTE_H

#include "raylib.h"

struct CuerpoCeleste {
  Vector3 position;      // Posición (x, y, z)
  Vector3 velocity;      // Velocidad (vx, vy, vz)
  Vector3 acceleration;  // Aceleración
  float mass;            // Masa
  float radius;          // Radio visual
  Color color;           // Color

  // Constructor
  CuerpoCeleste(Vector3 pos, Vector3 vel, float m, float r, Color c)
      : position(pos), velocity(vel), mass(m), radius(r), color(c) {
    acceleration = {0.0f, 0.0f, 0.0f};
  }
};

#endif