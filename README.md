# Lua Script Manager

A C++ library for managing Lua scripts and facilitating interactions with Lua functions.

## Introduction

The Lua Script Manager is a C++ library that provides a convenient interface for managing Lua scripts and interacting with Lua functions from C++ code. It offers a set of functionalities to compile and execute Lua scripts, register C++ functions as Lua functions, and handle Lua values.

## Dependencies

- [lua](https://www.lua.org/docs.html)

## Features

- Easy integration of Lua scripts into C++ applications
- Registration of C++ functions as Lua functions
- Conversion functions for various Lua value types
- Dynamic handling of Lua tables
- Support for multiple Lua libraries

## Installation

To use the Lua Script Manager library in your C++ project, follow these steps:

1. Clone the repository to your local machine.
2. Include the necessary header files in your C++ project.
3. Utilize the provided classes and functions according to the documentation.

## Usage

The Lua Script Manager is easy to use. After including the required headers, create an instance of the `LuaScript` class and use its methods to load Lua scripts, register functions, and execute Lua code from C++.

Example:

```cpp
// Include necessary headers
#include "lua_script.h"

// Create an instance of LuaScript
LuaScript script("path/to/your/script.lua");

// Register a C++ function as a Lua function
script.regFunc([](LuaScript& lua) -> int {
    // Your function logic here
    return 0; // Return value
}, "yourLuaFunction");

// Compile and execute the Lua script
script.compile();
```

For more detailed usage instructions, please refer to the documentation and code examples in the repository.

## Contributing

Contributions to the Lua Script Manager are welcome. If you find any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request.

## License

The Lua Script Manager is open-source and distributed under the MIT License. See the [LICENSE](./LICENSE) file for more information.

## Contact

For any questions or inquiries, please contact me.