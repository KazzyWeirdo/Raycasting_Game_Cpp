# C++ Raycaster Engine & Level Editor

A *Wolfenstein 3D* style Raycasting engine built from scratch using modern C++. The project features a real-time level editor integrated via ImGui, allowing for map creation, editing, and persistence.

![Example_Scene_Render](https://github.com/user-attachments/assets/9b888697-ef6c-4f5f-b18d-30059f654c75)

![Example_Creator_Maps](https://github.com/user-attachments/assets/6b574d42-f2cf-4275-b232-b36000324494)

## Key Features

* **Rendering Engine:** Implementation of the DDA (Digital Differential Analyzer) algorithm for efficient raycasting.
* **In-Game Level Editor:**
    * User Interface powered by **ImGui**.
    * Dynamic placement of walls and floors.
    * Visual configuration of the player's start position (Spawn Point).
* **Save System:** Map serialization to save and load custom levels.
* **Dependency Management:** Uses **CMake FetchContent** to automatically download and compile SFML and ImGui.

## Tech Stack

* **Language:** C++17
* **Graphics:** [SFML 3.0.1](https://www.sfml-dev.org/)
* **UI:** [Dear ImGui](https://github.com/ocornut/imgui) + [ImGui-SFML](https://github.com/SFML/imgui-sfml)
* **Build System:** CMake 3.10+

## Controls

### Game Mode
| Key | Action |
| :--- | :--- |
| **W / A / S / D** | Move |
| **Arrows <- ->** | Rotate Camera |
| **ESC** | Exit |

### Editor Mode
2.  **Left Click:** Place Wall.
3.  **Right Click:** Remove Wall (Place Floor).
4.  **Set Spawn:** Select the tool and click on the map to define the player start position (marked in green).
5.  **Save Map:** Save changes to the level file.
6.  **CTRL + S:** Shortcut to save changes to the level file.


You can play with this game engine on the releases section inside the repository!
