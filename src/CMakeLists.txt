# Source files - start with a simple example
set(COMMON_SOURCES
    # Add source files as you create them
)

set(STORAGE_SOURCES
    # Add source files as you create them
)

# Create a simple main file for now
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp "
#include <iostream>

int main() {
    std::cout << \"Mokshith DB v1.0 - Starting...\" << std::endl;
    std::cout << \"Database engine initialized successfully!\" << std::endl;
    return 0;
}
")

# Create executable
add_executable(mokshith_db main.cpp)

# Link libraries
target_link_libraries(mokshith_db Threads::Threads)
