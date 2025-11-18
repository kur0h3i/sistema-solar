# 🌌 Sistema Solar 3D

Simulador físico del Sistema Solar en C++ con visualización 3D en tiempo real. Implementa la ley de gravitación universal de Newton para simular órbitas planetarias realistas.

## 📋 Descripción

Este proyecto simula el comportamiento gravitatorio de los planetas del Sistema Solar usando física newtoniana. Los planetas orbitan alrededor del Sol siguiendo sus trayectorias naturales, permitiendo observar el movimiento celestial a diferentes velocidades de tiempo.

## 🎯 Objetivos del Proyecto

- Simular las órbitas de los 8 planetas del Sistema Solar
- Implementar física gravitatoria precisa usando integradores numéricos
- Visualizar trayectorias planetarias en 3D
- Controlar la velocidad de simulación para observar el comportamiento a largo plazo
- Navegación libre con cámara 3D

## 🛠️ Tecnologías

- **Lenguaje:** C++17
- **Gráficos:** raylib
- **Build System:** CMake
- **Física:** Integrador de Verlet/RK4

## ✅ Checklist de Desarrollo

### Setup Inicial

- [ ] Crear estructura del proyecto con CMake
- [ ] Instalar y configurar raylib
- [ ] Compilar y ejecutar "Hello Window" básico
- [ ] Verificar que renderiza una esfera 3D
- [ ] Implementar cámara 3D con controles (WASD + mouse)

### Sistema de Física

- [ ] Crear estructura `CelestialBody` (posición, velocidad, masa, radio, color)
- [ ] Implementar función de fuerza gravitatoria (F = G·m₁·m₂/r²)
- [ ] Implementar integrador numérico (Verlet o Euler mejorado)
- [ ] Crear función `update()` que calcula nuevas posiciones cada frame
- [ ] Probar con 2 cuerpos (Sol + Tierra) y verificar órbita estable

### Renderizado 3D

- [ ] Crear función `render()` que dibuja todas las esferas
- [ ] Implementar sistema de escalado (distancias reales son enormes)
- [ ] Renderizar el Sol en el centro
- [ ] Añadir los 8 planetas con tamaños y colores aproximados
- [ ] Sistema de colores/materiales básicos para distinguir planetas

### Trayectorias Orbitales

- [ ] Guardar historial de posiciones de cada planeta
- [ ] Dibujar líneas conectando posiciones pasadas
- [ ] Limitar longitud de trayectoria (últimas N posiciones)
- [ ] Toggle para mostrar/ocultar trayectorias

### Control de Tiempo

- [ ] Variable `timeScale` para controlar velocidad de simulación
- [ ] Teclas para acelerar (→) y desacelerar (←) tiempo
- [ ] Pausar/reanudar con SPACE
- [ ] Mostrar velocidad actual en pantalla (×1, ×10, ×100...)
- [ ] Contador de días/años simulados

### Sistema Solar Completo

- [ ] Añadir datos reales de todos los planetas:
  - [ ] Mercurio
  - [ ] Venus
  - [ ] Tierra
  - [ ] Marte
  - [ ] Júpiter
  - [ ] Saturno
  - [ ] Urano
  - [ ] Neptuno
- [ ] Verificar que las órbitas son estables a largo plazo
- [ ] Ajustar escala visual para que todo sea visible

### UI y Controles

- [ ] Texto en pantalla con FPS
- [ ] Mostrar velocidad de simulación actual
- [ ] Mostrar tiempo simulado (días/años)
- [ ] Instrucciones de controles en pantalla
- [ ] Tecla R para resetear simulación

### Testing y Refinamiento

- [ ] Verificar conservación de energía (órbitas no se degradan)
- [ ] Optimizar timestep para balance precisión/rendimiento
- [ ] Suavizar movimiento de cámara
- [ ] Ajustar velocidades iniciales para órbitas correctas
- [ ] Documentar código con comentarios

## 🎮 Controles (Planificados)

- **WASD** - Mover cámara
- **Mouse** - Rotar vista
- **Space** - Pausar/Reanudar
- **← →** - Ralentizar/Acelerar tiempo
- **R** - Resetear simulación
- **T** - Toggle trayectorias
- **ESC** - Salir

## 📊 Datos Planetarios Necesarios

Para cada planeta necesitas:

- Masa (kg)
- Radio (km) - para visualización
- Distancia al Sol (UA o km)
- Velocidad orbital inicial (km/s)
- Color RGB aproximado

## 🚀 Compilación

```bash
mkdir build
cd build
cmake ..
make
./solar_system
```

## 📚 Recursos

- [Raylib Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [Gravitational N-body simulation](https://en.wikipedia.org/wiki/N-body_simulation)
- [Verlet Integration](https://en.wikipedia.org/wiki/Verlet_integration)
- Datos planetarios: NASA JPL

## 📝 Notas de Desarrollo

- Las distancias reales son enormes - necesitarás un factor de escala
- Los tamaños de planetas también necesitan escala (si usas tamaños reales, los rocosos son invisibles)
- El timestep de física debe ser pequeño para órbitas estables
- Considera usar unidades astronómicas (UA) en vez de metros para evitar números enormes

---

**Versión:** 1.0 (Básica)  
**Autor:** Pedro Pérez Montero
