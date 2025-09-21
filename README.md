# Final project - Fare Calculator (wxWidgets + C++)

A small GUI **fare calculator application** built in **C++/wxWidgets**, edited and run on **VS Code** using the **MSVC** toolchain from **Visual Studio Community 2022**.

## ✨ Features

- Car type and variant selection (Bezza, Saga, Persona, Vios, Myvi)
- Shows engine CC, fuel consumption, and per-km rate
- Inputs for distance (km) and time (minutes)
- Traffic jam surcharge (+20%) and night charge surcharge (+30%)
- Displays computed fare in `RMXX.XX` format
- Green themed pallete used to fit Grab color profile

---

## 🧰 Requirements (one-time)
**Windows 10/11 (x64)**
**Visual Studio Community 2022**  
   - Open **Visual Studio Installer** → install **Desktop development with C++** workload
**vcpkg** at `C:\vcpkg`
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
cd Final-project
code .
```

### 2. Configure, build, run (in VS Code)

1. **Select a Kit (first time only)**
   **Ctrl+Shift+P** → **CMake: Select a Kit** → pick **Visual Studio 17 2022 – amd64**

2. **Configure**
    **Ctrl+Shift+P** → **CMake: Configure**

3. **Build**
    **Ctrl+Shift+P** → **CMake: Build** (F7) or click **Build** in the CMake status bar.

4. **Run / Debug**
    **Ctrl+Shift+P** → **CMake: Debug** (Shift+F5) or click the **Debug** button in the CMake status bar.