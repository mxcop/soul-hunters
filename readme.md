# C++ Premake Game <sub>`OpenGL & GLFW`</sub>

## Cloning
Make sure to clone using the following command :
```
$ git clone --recurse-submodules <git:url>
```

## Updating submodules
```
$ git submodule update --recursive --init
```

<br>

## Development

### Premake
To get started first download the premake binary from <https://premake.github.io/download/><br>
Then add the files from the `.zip` into the `~/premake` directory within the project.

<br>

### Generating project files
To generate the project files run the following commands :
```
$ cd premake

$ premake5 vs2022
```

<br>

### Codestyle
Our style guidelines look like the code snippet visible below:
```cpp
/*
 * '#define' and 'const' variables are UPPERCASE.
 */
#define MACRO 17
const float PI = 3.14;

/*
 * 'class' and 'struct' are CamelCase.
 */
class Class 
{
    int id;
};
struct Struct {...};

/*
 * functions, variables, and namespaces are snake_case.
 */
void my_func(int func_arg) 
{
    bool is_enabled = true;

    if (is_enabled) {
        /* 'if' statement use same-line curly brackets. */
    }
};

namespace my_space {...};
```
