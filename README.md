# 🚢 Battleship Command (C++/SFML)

### 📋 Project Overview
**Battleship Command** is a high-stakes, grid-based strategy game developed from the ground up using **C++** and the **SFML (Simple and Fast Multimedia Library)**. This project represents my journey from understanding core programming logic to implementing a fully functional graphical user interface (GUI) with real-time audio and state management.

The player takes on the role of a Naval Commander tasked with eliminating 5 hidden enemy ships within a high-fog combat zone. With a limited supply of 30 shells, every click is a tactical decision that determines the success or failure of the mission.

### 🚀 Key Technical Features

* **Advanced Game State Management:** The engine transitions smoothly between multiple game states: a cinematic **Intro**, an interactive **Main Menu**, a **Loading** sequence, and a dedicated **Mission Objective** briefing before the tactical gameplay begins.
    
* **Procedural Content Generation:** Ships are placed using a randomized algorithm (`srand` and `time`), ensuring that no two games are ever the same. This logic handles coordinate validation to prevent overlapping or out-of-bounds errors.

* **Tactical UI & Real-Time Feedback:** Includes a "Command Center" log box that provides instant feedback on every shot (HIT or MISS), alongside an ammunition counter and mission progress tracker.

* **Immersive Multimedia Integration:** - **Audio:** Spatial sound effects for explosions and splashes, coupled with a looping military-style background score.
    - **Graphics:** Custom textures for water, ships, and hit/miss markers, utilizing alpha-blending for flickering UI effects.

### 🛠️ Development Challenges & Learning
Developing this game in C++ required overcoming several challenges:
1.  **Coordinate Mapping:** Translating mouse clicks from screen pixels to 10x10 grid coordinates.
2.  **Resource Handling:** Efficiently loading and managing textures, fonts, and buffers to prevent memory leaks.
3.  **Modern SFML Standards:** Implementing `sf::Vector2f` and modern class structures to ensure compatibility with recent SFML versions.

### 🕹️ How to Play
1.  **Launch** the game and wait for the "MZF Developers" intro.
2.  **Select PLAY** from the Main Menu.
3.  **Read the Mission Briefing** to understand your objectives.
4.  **Click on the grid** to fire shells. 
    - **Green Marker:** Enemy ship hit!
    - **Red Marker:** Splash—you missed.
5.  **Win** by sinking all 5 ships before your 30 shells run out!

### 🔧 Tech Stack
- **Language:** C++17
- **Library:** SFML (Simple and Fast Multimedia Library)
- **IDE:** VS Code / Dev-C++
- **Compiler:** MinGW (GCC)
