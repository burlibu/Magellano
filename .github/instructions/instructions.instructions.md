---
applyTo: "**"
---

Provide project context and coding guidelines that AI should follow when generating code, answering questions, or reviewing changes.

# Project Instructions

This project is called Magellano, an amateur project network scanning and enumeration tool written in C++. It is designed to assist security professionals in identifying vulnerabilities and gathering information about target systems.

## Coding Guidelines

1. **Language**: The primary programming language used in this project is C++. Ensure that all code adheres to C++ standards.

Always follow a structured approach for everything.

The codebase is thought to be organized and containerized as follows:

The folder lib contains general functions in functions.h and functions.cpp,
Functions must ALWAYS be declared in functions.h. and implemented in functions.cpp
Crypto utils/crypto_utils.h and crypto_utils.cpp contain all cryptography-related functions.
custom_colors.h and custom_colors.cpp contain all color-related functions and variables/constants.

Settings are mostly global variables declared in settings.h and implemented in settings.cpp

The main application logic resides in main.cpp.

The gui folder contains a file gui.h for declarations and gui.cpp for every single class implementations.

DO CHECK MAKE FILE EVERYTIME. If you added new files, please update the Makefile accordingly.

2. **Code Style**: Follow consistent coding styles, including indentation, naming conventions, and commenting. Use meaningful variable and function names.

3. **Documentation**: Maintain clear and comprehensive documentation in the header files SIMILAR TO THE ONE ALREADY PRESENT for all functions, classes, and modules. Use comments to explain complex logic and decisions.

<general guidelines>

NEVER use meta-phrases (e.g., "let me help you", "I can see that").

NEVER refer to "screenshot" needed. "image" refer to it as "the screen" if needed

ALWAYS be specific, detailed, and accurate.

ALWAYS acknowledge uncertainty when present.

ALWAYS use markdown formatting.

Only acknowledge ambiguity and offer a clearly labeled guess if appropriate.

</general guidelines> 4. **Version Control**: Use Git for version control. Commit changes with clear and descriptive messages. Follow branching strategies for feature development and bug fixes.

Be thorough and comprehensive in technical explanations.

Ensure all instructions are unambiguous and actionable. Provide sufficient detail that
responses are immediately useful. Maintain consistent formatting throughout.
You MUST NEVER just summarize what's on the screen unless you are explicitly
asked to

DO NOT BREAK OR DRAMATICALLY CHANGE EXISTING FUNCTIONALITY unless explicitly instructed to.

5. **Testing**: Implement unit tests for critical components and functionalities. Ensure that new features do not break existing functionality.
