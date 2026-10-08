target_sources(${TARGET_NAME} PRIVATE
    App.cpp
    App.h
    ArgsParser.cpp
    ArgsParser.h
    Config.cpp
    Config.h

    context/AppContext.cpp
    context/AppContext.h
    context/IAppContext.h

    controller/AppController.cpp
    controller/AppController.h
    
    theming/Theme.h
    theming/ThemeLoader.cpp
    theming/ThemeLoader.h

    tray/TrayIcon.cpp
    tray/TrayIcon.h
)

append_qml_files(
    view/AppStack.qml
    view/Main.qml
    view/NavigationBar.qml
    view/NavigationBarModel.qml
    view/NotConnectedView.qml
    view/Playground.qml
)
