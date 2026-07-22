cmake_minimum_required(VERSION 3.19)

target_sources(${TARGET_NAME} PRIVATE
    ActionTypes.h
    IActionHandler.h

    system/SystemActions.cpp
    system/SystemActions.h
)
