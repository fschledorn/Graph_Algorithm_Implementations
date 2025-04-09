#include "core/graph.hh" // Include your graph header
#include <iostream>


int main() {
    // Instantiate the graph with a concrete type for the EdgeWeight template parameter.
    // Using 'double' as an example weight type. Adjust if needed.
    // Note: The Graph template requires an EdgeType, which is defined via a concept
    // expecting EdgeWeight. EdgeWeight itself is a std::variant.
    // We need to provide the specific variant type here.
    Graph myGraph;

    std::cout << "Compilation test successful: Graph object created." << std::endl;

    return 0;
}