# ConcoursUbisoft2026

📋 Setup Instructions

Prerequisites

    Unreal Engine 5.6.1

    Visual Studio 2022 (With "Game Development with C++" workload installed).


1. Clone the Repository

Open your terminal/command prompt:
Bash

git clone <YOUR_REPO_URL_HERE>
cd <YOUR_REPO_NAME>


2. Generate Project Files

    Navigate to the Game_UE folder in File Explorer.

    Right-click on ubi26.uproject.

    Select Show More Options (Windows 11) → Generate Visual Studio project files.

        Note: This generates the .sln file which is not shared on GitHub.

3. Compile the Project

    Open the newly generated ubi26.sln in Visual Studio.

    Set your solution configuration to Development Editor (Top toolbar).

    Right-click the project name in the Solution Explorer and hit Build.

        Wait for the build to finish successfully.

4. Launch

You can now double-click ubi26.uproject or press F5 inside Visual Studio to debug/launch the editor.
📂 Folder Structure

    Game_UE/: The main Unreal Engine project.

        Note: Do not modify .uproject name without updating C++ modules.

    Art_Source/: Raw assets for Artists (PSDs, Blends, High-poly sculpts). Do not import these directly into UE.

    Design/: Documentation, GDD, and spreadsheets.