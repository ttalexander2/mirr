# Documentation for mirr {#mainpage}

Mirr aims to be a simple runtime reflection library for c++.
It was written for use in the chroma engine, and also takes ideas from the entt reflection library.

### Usage

The following is a simple example of the reflection system.

```c++

#include <mirr/mirr.h>

class vector
{
    float x, y, z;
    float magnitude();
};

int main()
{
    // To register a type
    mirr::register_type<vector>("vector")
            .data<&vector::x>("x") // Register our data members
            .data<&vector::y>("y")
            .data<&vector::z>("z")
            .function<&vector::magnitude>("magnitude"); // Register the magnitude function
            
    // Use the reflection system
    mirr::type type = mirr::resolve<vector>("vector");
    
    vector my_vector{};
    
    // Set a data member using the reflection system
    type.data("x").set(my_vector, 4.2f);
    
    // Call a function from its name using the reflection system
    float magnitude = type.function("magnitude").invoke(my_vector);
    
    return 0;
}
```