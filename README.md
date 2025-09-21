# Final project

# 🚗 Fare Calculator

A graphical **fare calculator application** built and run from **Visual Studio Code** using the **MSVC** toolchain from **Visual Studio Community 2022**. Dependencies are managed by **vcpkg**.

## ✨ Features

- Car type and variant selection (Bezza, Saga, Persona, Vios, Myvi)
- Shows engine CC, fuel consumption, and per-km rate
- Inputs for distance (km) and time (minutes)
- Traffic jam surcharge (+20%) and night charge surcharge (+30%)
- Displays computed fare in `RMXX.XX` format

---

## 🧰 Requirements (one-time)
**Windows 10/11 (x64)**

1) **Visual Studio Community 2022**  
   - Open **Visual Studio Installer** → **Modify** your VS 2022 install  
   - Check **Desktop development with C++**

2) **vcpkg** at `C:\vcpkg`
```bat
git clone https://github.com/microsoft/vcpkg C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
C:\vcpkg\vcpkg integrate install
```

---

## 🚀 Getting Started

### 1. Clone the repo
```bash
git clone https://github.com/pavienm/Final-project
cd fare-calculator
```

### 2. Configure, build, run (in VS Code)

1. **Select a Kit (first time only)**
   - Press **Ctrl+Shift+P** → **CMake: Select a Kit**
   - Choose **Visual Studio 17 2022 – amd64**

2. **Configure**
    - Press **Ctrl+Shift+P** → **CMake: Configure**
    - Dependencies from `vcpkg.json` (including **wxWidgets**) will be restored automatically.

3. **Build**
    - Press **Ctrl+Shift+P** → **CMake: Build** (F7)
    or click **Build** in the CMake status bar.

4. **Run / Debug**
    - Press **Ctrl+Shift+P** → **CMake: Debug** (Shift+F5)
    or click the **Debug** button in the CMake status bar.



