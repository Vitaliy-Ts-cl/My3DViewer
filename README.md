# My3DViewer

Базовий C++17-проєкт 3D-в'ювера для лабораторної роботи № 7.
Майбутня мета — завантаження і відображення 3D-моделей, зокрема STL.
**На поточному етапі завантаження STL ще не реалізоване.**

## Використані технології

- **GLM** — математика векторів і матриць;
- **GLFW** — створення вікна і контексту OpenGL;
- **Glad** — завантаження функцій OpenGL;
- **CMake** — налаштування і збірка;
- **vcpkg** — встановлення зовнішніх залежностей;
- **Git** — контроль версій.

## Збірка (Windows, Visual Studio 2022, PowerShell)

Потрібні Git, CMake, Visual Studio 2022 із компонентом **Desktop development with C++**, vcpkg.

```powershell
$env:VCPKG_ROOT = "$HOME\vcpkg"
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 "-DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
cmake --build build --config Release
.\build\Release\My3DViewer.exe
```

Під час конфігурації CMake автоматично встановлює залежності з `vcpkg.json`.

## Результат

Програма виводить вектор `(1, 0, 0)`, версію OpenGL і повідомлення
`LAB 7 SUCCESS: GLM, GLFW and Glad initialized.` та відкриває порожнє
вікно OpenGL із темним фоном. Вікно є основою для майбутнього 3D-в'ювера.
