# SPDX-License-Identifier: GPL-3.0-or-later

# CMAKE

cmake_minimum_required(VERSION 3.16)

# PROJECT

set(NOVAREWIND_VERSION 0.1.15)
project(novarewind VERSION ${NOVAREWIND_VERSION} LANGUAGES CXX)
set(PROJECT_URI org.kde.${PROJECT_NAME})

# REQUIRED QT/KF

set(QT_MAJOR_VERSION 6)
set(REQUIRED_QT_VERSION 6.5.2)
set(REQUIRED_KF_VERSION 5.240.0)

# CMAKE OPTIONS

# set(CMAKE_CXX_STANDARD 17): Set C++17 standard.
# set(CMAKE_CXX_STANDARD_REQUIRED ON): Boolean describing whether the value of CXX_STANDARD is a requirement.
# set(CMAKE_INCLUDE_CURRENT_DIR ON): Add the CMAKE_CURRENT_SOURCE_DIR and CMAKE_CURRENT_BINARY_DIR to the include path.
# set(CMAKE_AUTOMOC ON): CMake will scan the header and source files at build time and invoke moc accordingly.
# set(CMAKE_AUTORCC ON): CMake will handle .qrc files added as target sources at build time and invoke rcc accordingly.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# REQUIRED DEVEL PACKAGES

find_package(ECM ${REQUIRED_KF_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${ECM_MODULE_PATH})

find_package(Qt6 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Gui Widgets Qml Quick)
find_package(KF6 ${REQUIRED_KF_VERSION} REQUIRED COMPONENTS I18n Config CoreAddons)
find_package(MauiKit4 REQUIRED)
find_package(MauiKit4 REQUIRED COMPONENTS FileBrowsing)
find_package(MauiMan4 REQUIRED)

# INCLUDE THE EXTRA CMAKE MODULES (ECM)
# PROVIDES HELPFUL MACROS, EXTEND FIND MODULES, COMMON SETTINGS AND CHANGE BUILD TOOLS (ANDROID)

# KDEInstallDirs6: Define KDE standard installation directories such as BINDIR, KDE_INSTALL_<dir>
# KDECMakeSetting: Changes various CMake settings to what the KDE community views as more sensible defaults.
# KDECompilerSettings: Set useful compile and link flags for C++ (and C) code.
# ECMSetupVersion: Handle library version information. Maui apps use it to add version to about dialog from main.cpp.
# ECMSourceVersionControl: Tries to determinate wheter ther source is under version control (git clone, svn checkout, etc).

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(ECMSourceVersionControl)

# ECM SETUP VERSION

ecm_setup_version(${NOVAREWIND_VERSION}
    VARIABLE_PREFIX NOVAREWIND
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/novarewind_version.h")

# ECM SOURCE VERSION CONTROL

if(${ECM_SOURCE_UNDER_VERSION_CONTROL})
    execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Get the latest abbreviated commit hash of the working branch
    execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    add_definitions(-DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")
    add_definitions(-DGIT_BRANCH="${GIT_BRANCH}")

else()
    add_definitions(-DGIT_COMMIT_HASH="${NOVAREWIND_VERSION}")
    add_definitions(-DGIT_BRANCH="Stable")
endif()

# POLICY

# QTP0001: Default resource prefix for QML modules. The NEW behavior ensure that modules are put into the QML import path.
# The right URL to main.qml also must be set in main.cpp file (QQMLAPPLICATIONENGINE section).
# OLD behavior defaults to /
# NEW behavior defaults to /qt/qml/.

qt_policy(SET QTP0001 OLD)

# QTP0004: Enable this policy means that all the components can see each other without explcitly importing the module.
# The OLD behavior is that a qmldir file is only generated for the root directory of a module.
# The NEW behavior is that for each directory with QML files in a module a separate qmldir file is generated.

qt_policy(SET QTP0004 NEW)

# ADD COMPILE DEFINITIONS

add_compile_definitions(APP_COPYRIGHT_NOTICE="© 2023-2025 Nova Flow OS")

# ADD SUBDIRECTORY SRC

add_subdirectory(src)

# TRANSLATIONS

ki18n_install(po)

# BINARY DESTINATION

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
