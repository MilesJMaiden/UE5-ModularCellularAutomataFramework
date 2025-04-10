# A Modular Cellular Automata Framework in Unreal Engine 5.5

This repository contains a modular implementation of Conway's Game of Life built within Unreal Engine 5.5. Leveraging the power of C++ for performance and the flexibility of Blueprints for rapid iteration, this framework demonstrates how simple, local rules can create complex, emergent behaviors in a visually stunning, real-time simulation.

## Overview

Conway's Game of Life is a classic cellular automaton that illustrates how basic rules can generate intricate patterns. In this project, a central **CellularAutomataManager** orchestrates grid updates and pattern spawning. Custom pattern classes define various configurations—ranging from still lifes to oscillators and spaceships—that are exposed to Blueprints for easy customization.

## Features

- **Modular Design:** Easily extend the simulation by adding new pattern classes without modifying the core logic.
- **Efficient Simulation:** Employs object pooling and partial grid updates to maintain performance, even with large grids.
- **Customizable Parameters:** Adjust grid size, update intervals, and visual effects such as opacity fades and rotations via Blueprint-exposed variables.
- **Dynamic Visuals:** Utilizes Unreal Engine’s advanced rendering features to create smooth, real-time animations.
- **Future-Proof:** Designed to support future enhancements like interactive UI, 3D/VR extensions, and alternative automata rules.

## Getting Started

### Prerequisites

- **Unreal Engine 5.5:** Ensure you have the correct version of Unreal Engine installed.
- **C++ Development Environment:** A suitable IDE (e.g., Visual Studio) configured for Unreal Engine C++ projects.

### Installation

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/MilesJMaiden/PGE2.git
