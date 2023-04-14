# mirr

### Minimally Invasive Runtime Reflection

---
Mirr aims to be a simple runtime reflection library for c++.
It was written for use in the chroma engine. The API was written to be similar to the meta reflection system from
the popular ECS library `entt`, while attempting to simplify the API, and make integration into a game engine easier.

### Usage

The following is a simple example of the reflection system.

```c++

#include <mirr/mirr.h>

// We'll use this type for demonstrating usage.
struct vector
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
    mirr::type type = mirr::resolve("vector");
    
    vector my_vector{};
    
    // Set a data member using the reflection system
    type.data("x").set(my_vector, 4.2f);
    
    // Call a function from its name using the reflection system
    float magnitude = type.function("magnitude").invoke(my_vector);
    
    return 0;
}
```