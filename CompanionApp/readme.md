## Getting started

### Linux

#### **Pre-requisites** (Fedora based, other distros/packages managers should have similar packages)

- install related build-essentials via `sudo dnf group install c-development development-tools`
- install *Qt6.9* via https://www.qt.io/development/download-qt-installer-oss
    + set `QTDIR` ENV variable to where Qt is installed `<<path-to>>/Qt/6.9.X/gcc_64/lib/cmake/Qt6`
    + by this time, the 6.9 version may have a different patch hence the placeholder *6.9.X*
- install *WrapOpenGL* via `sudo dnf install mesa-libGL-devel`
- install *hidapi* via `sudo dnf install hidapi hidapi-devel`
- install *PulseAudio* via `sudo dnf install pulseaudio-libs-devel`

#### **Building** a release deployment

- using **cmake** (assuming terminal opened in repo root)
    + `cd CompanionApp && mkdir build`
    + `cmake --preset linux-release`
    + `cmake --build --preset linux-deploy`
    + `cd build/linux-release/deploy/bin`
    + `./Companion`

#### **VSCodium** development setup

- use `Open Workspace from File` to open `Macropad.code-workspace`
- extensions:
    + `C/C++ Debug` by `KylinIdeTeam` - launch debugger
    + `Kylin CMake Workflow` by `KylinIdeTeam` - cmake tools
    + `Kylin Clangd` by `KylinIdeTeam` - c++ auto complete
        - install *clang* via `sudo dnf install clang clang-tools-extra`
    + `CMake IntelliSense` by `KylinIdeTeam` - cmake autocomplete and syntax highlighting
    + `Qt Core` by `Qt Group`
        - open command palette (`Ctrl+Shift+P`)
        - start typing `Register Qt installation`
        - navigate to where Qt is installed `<<path-to>>/Qt`
    + `Qt Qml` by `Qt Group` - qml language server
        - steps in case qml linting is not working out of the box
        - open extension settings
        - find `Qmlls: Custom Exe Path`
        - set it to `<<path-to>>/Qt/6.9.X/gcc_64/bin/qmlls`
        - reload window

### Windows (TODO)
