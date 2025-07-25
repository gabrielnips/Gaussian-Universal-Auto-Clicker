# Gaussian Universal Auto Clicker

![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

A modern, console-based auto-clicker for Windows written in C++. This application uses a Gaussian (normal) distribution to randomize the clicks per second (CPS) between a user-defined minimum and maximum, providing a more "human-like" clicking pattern.

## Features

- **Custom Activation Key**: Set any keyboard key or mouse button (including Mouse4/Mouse5) as the clicker's trigger.
- **Gaussian Randomization**: Clicks are not timed uniformly. They follow a normal distribution based on your configured min/max CPS.
- **Window Targeting**: The auto-clicker will only activate when a specific, user-selected window is in the foreground.
- **Modern C++ Architecture**: Built using dependency injection principles for a clean, organized, and extensible codebase.
- **Fixed Console UI**: The console window has a fixed size and cannot be resized, ensuring a consistent user interface.

## Building from Source

### Prerequisites
- A C++ compiler supporting C++17 or later.
- Windows SDK.
- The project is structured for a build environment like Visual Studio that supports precompiled headers (`Stdafx.hpp`).
