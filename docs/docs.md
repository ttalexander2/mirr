# Documentation for mirr {#mainpage}

Mirr aims to be a simple runtime reflection library for c++.
It was written for use in the chroma engine, and also takes ideas from the entt reflection library.

### Usage

Consider a simple example for the following flass:

```c++

#include <mirr/mirr.h>

class vector
{
    float x, y, z;
    float magnitute();
};

int main()
{
    // To register a type
    mirr::register_type<vector>("vector");
    
    mirr::type type = mirr::resolve<vector>("vector");
    
    return 0;
}
```