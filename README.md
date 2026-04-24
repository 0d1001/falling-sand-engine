# Falling Sand Engine

A real-time 2D particle sandbox simulation with physics-based interactions. Features sand, water, and walls with realistic movement behaviors.

## Features

- **Sand** - Falls downward, rolls down slopes
- **Water** - Flows downward and sideways, can be displaced by sand
- **Walls** - Static obstacles that block particle movement
- Real-time world editing (LMB/RMB)
- 60 FPS rendering

## TODO
- [ ] Migrate from SFML to OpenGL for better performance

- [ ] Improve particle physics (velocity, momentum, acceleration)

- [ ] Add more particle types (at least 10+ different materials)

- [ ] Implement density system (lighter particles float on heavier ones)

- [ ] Add gases (smoke, steam, fire) with rising behavior

- [ ] Pressure system for gases and liquids

- [ ] Temperature and heat simulation

- [ ] Particle state changes (water -> steam, sand -> glass)

- [ ] Different brush sizes

- [ ] Save/load worlds

- [ ] Pause and step-by-step mode

- [ ] Particle color mixing

- [ ] Explosives with chain reactions

- [ ] GPU acceleration using compute shaders

- [ ] Infinite procedurally generated world

- [ ] Multi-threaded physics updates

- [ ] Custom particle editor

- [ ] Network multiplayer support

## Controls

| Key | Action |
|-----|--------|
| `1` | Select Sand |
| `2` | Select Water |
| `3` | Select Wall |
| `LMB` | Place particle |
| `RMB` | Delete particle |
| `ESC` | Exit |