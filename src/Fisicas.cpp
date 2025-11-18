#include <cmath>
#include <vector>

#include "CuerpoCeleste.h"
#include "raylib.h"
#include "raymath.h"

// Constante gravitacional (ajustada para la simulación)
const float G = 0.1f;

// Distancia mínima para evitar singularidades
const float MIN_DISTANCE = 1.0f;

// Estructura para derivadas (velocidad y aceleración)
struct Derivative {
  Vector3 velocity;
  Vector3 acceleration;
};

// FÍSICA DE 2 CUERPOS: Solo el Sol ejerce gravedad (más estable)
// Modelo simplificado pero visualmente preciso para órbitas
Vector3 CalculateAcceleration(const std::vector<CuerpoCeleste>& bodies,
                              size_t bodyIndex) {
  // El Sol (índice 0) no se mueve
  if (bodyIndex == 0) {
    return {0.0f, 0.0f, 0.0f};
  }

  // Todos los planetas solo sienten la gravedad del Sol
  // Vector del planeta hacia el Sol
  Vector3 toSun =
      Vector3Subtract(bodies[0].position, bodies[bodyIndex].position);
  float distance = Vector3Length(toSun);

  // Evitar división por cero
  if (distance < MIN_DISTANCE) {
    distance = MIN_DISTANCE;
  }

  // Dirección normalizada
  Vector3 direction = Vector3Normalize(toSun);

  // Ley de gravitación: a = G * M_sol / r^2
  float accelMagnitude = (G * bodies[0].mass) / (distance * distance);
  Vector3 accel = Vector3Scale(direction, accelMagnitude);

  return accel;
}

// Integrador Runge-Kutta 4º orden (RK4) - MUY PRECISO
// Con física de 2-cuerpos: solo el Sol ejerce gravedad (estable y eficiente)
void UpdateBodies(std::vector<CuerpoCeleste>& bodies, float deltaTime) {
  if (bodies.empty()) return;

  // Timestep adaptativo para estabilidad
  const float maxDt = 0.02f;  // Timestep óptimo para 2-cuerpos
  int subSteps = (int)ceilf(deltaTime / maxDt);
  float dt = deltaTime / (float)subSteps;

  for (int step = 0; step < subSteps; step++) {
    // Guardar posiciones actuales para los cálculos
    std::vector<Vector3> currentPositions(bodies.size());
    std::vector<Vector3> currentVelocities(bodies.size());

    for (size_t i = 0; i < bodies.size(); i++) {
      currentPositions[i] = bodies[i].position;
      currentVelocities[i] = bodies[i].velocity;
    }

    // Arrays para almacenar los 4 pasos de RK4 para cada cuerpo
    std::vector<Derivative> k1(bodies.size());
    std::vector<Derivative> k2(bodies.size());
    std::vector<Derivative> k3(bodies.size());
    std::vector<Derivative> k4(bodies.size());

    // RK4 - Paso 1: Evaluar en t
    for (size_t i = 0; i < bodies.size(); i++) {
      k1[i].velocity = bodies[i].velocity;
      k1[i].acceleration = CalculateAcceleration(bodies, i);
    }

    // RK4 - Paso 2: Evaluar en t + dt/2 usando k1
    for (size_t i = 0; i < bodies.size(); i++) {
      bodies[i].position = Vector3Add(currentPositions[i],
                                      Vector3Scale(k1[i].velocity, dt * 0.5f));
    }
    for (size_t i = 0; i < bodies.size(); i++) {
      k2[i].velocity = Vector3Add(currentVelocities[i],
                                  Vector3Scale(k1[i].acceleration, dt * 0.5f));
      k2[i].acceleration = CalculateAcceleration(bodies, i);
    }

    // RK4 - Paso 3: Evaluar en t + dt/2 usando k2
    for (size_t i = 0; i < bodies.size(); i++) {
      bodies[i].position = Vector3Add(currentPositions[i],
                                      Vector3Scale(k2[i].velocity, dt * 0.5f));
    }
    for (size_t i = 0; i < bodies.size(); i++) {
      k3[i].velocity = Vector3Add(currentVelocities[i],
                                  Vector3Scale(k2[i].acceleration, dt * 0.5f));
      k3[i].acceleration = CalculateAcceleration(bodies, i);
    }

    // RK4 - Paso 4: Evaluar en t + dt usando k3
    for (size_t i = 0; i < bodies.size(); i++) {
      bodies[i].position =
          Vector3Add(currentPositions[i], Vector3Scale(k3[i].velocity, dt));
    }
    for (size_t i = 0; i < bodies.size(); i++) {
      k4[i].velocity = Vector3Add(currentVelocities[i],
                                  Vector3Scale(k3[i].acceleration, dt));
      k4[i].acceleration = CalculateAcceleration(bodies, i);
    }

    // RK4 - Combinar los 4 pasos con pesos: (k1 + 2*k2 + 2*k3 + k4) / 6
    for (size_t i = 0; i < bodies.size(); i++) {
      // Actualizar posición
      Vector3 positionChange = Vector3Scale(k1[i].velocity, 1.0f / 6.0f);
      positionChange =
          Vector3Add(positionChange, Vector3Scale(k2[i].velocity, 2.0f / 6.0f));
      positionChange =
          Vector3Add(positionChange, Vector3Scale(k3[i].velocity, 2.0f / 6.0f));
      positionChange =
          Vector3Add(positionChange, Vector3Scale(k4[i].velocity, 1.0f / 6.0f));
      bodies[i].position =
          Vector3Add(currentPositions[i], Vector3Scale(positionChange, dt));

      // Actualizar velocidad
      Vector3 velocityChange = Vector3Scale(k1[i].acceleration, 1.0f / 6.0f);
      velocityChange = Vector3Add(
          velocityChange, Vector3Scale(k2[i].acceleration, 2.0f / 6.0f));
      velocityChange = Vector3Add(
          velocityChange, Vector3Scale(k3[i].acceleration, 2.0f / 6.0f));
      velocityChange = Vector3Add(
          velocityChange, Vector3Scale(k4[i].acceleration, 1.0f / 6.0f));
      bodies[i].velocity =
          Vector3Add(currentVelocities[i], Vector3Scale(velocityChange, dt));

      // Actualizar aceleración para referencia
      bodies[i].acceleration = k4[i].acceleration;
    }
  }
}

// Calcular energía total del sistema (útil para verificar conservación)
float CalculateTotalEnergy(const std::vector<CuerpoCeleste>& bodies) {
  float kineticEnergy = 0.0f;
  float potentialEnergy = 0.0f;

  // Energía cinética: KE = 0.5 * m * v²
  for (const auto& body : bodies) {
    float v2 = Vector3LengthSqr(body.velocity);
    kineticEnergy += 0.5f * body.mass * v2;
  }

  // Energía potencial gravitacional: PE = -G * m1 * m2 / r
  for (size_t i = 0; i < bodies.size(); i++) {
    for (size_t j = i + 1; j < bodies.size(); j++) {
      float distance = Vector3Distance(bodies[i].position, bodies[j].position);
      if (distance > MIN_DISTANCE) {
        potentialEnergy -= G * bodies[i].mass * bodies[j].mass / distance;
      }
    }
  }

  return kineticEnergy + potentialEnergy;
}

// Calcular momento angular total (debe conservarse)
Vector3 CalculateTotalAngularMomentum(
    const std::vector<CuerpoCeleste>& bodies) {
  Vector3 totalL = {0.0f, 0.0f, 0.0f};

  for (const auto& body : bodies) {
    // L = r × (m * v)
    Vector3 momentum = Vector3Scale(body.velocity, body.mass);
    Vector3 angularMomentum = Vector3CrossProduct(body.position, momentum);
    totalL = Vector3Add(totalL, angularMomentum);
  }

  return totalL;
}