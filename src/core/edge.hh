#ifndef CORE_EDGE_H
#define CORE_EDGE_H
#include <memory>
#include <unordered_set>
#include <utility>
#include <variant>

#include "core/node.hh"

enum class EdgeTypes { UNDIRECTED, DIRECTED, WEIGHTED, UNWEIGHTED };

using EdgeWeight =
    std::variant<double, int, unsigned int,
                 std::monostate>;  // Does this work when the type is not known
                                   // at compile time? Probably not?

struct EdgeTraits {
  std::unordered_set<EdgeTypes> traits;
  // Todo : Add a constructor to initialize the traits and perform checking logic
  // for the traits.
};

/*
 * The Edge struct represents an edge in a graph, which can be directed or
 * undirected, and weighted or unweighted.
 *
 * Members:
 * - from: A shared pointer to the source Node object.
 * - to: A shared pointer to the destination Node object.
 * - direction: An EdgeDirection enum value indicating whether the edge is
 *   directed or undirected.
 * - weight: An EdgeWeight variant that can be a double, int, unsigned int, or
 *   std::monostate (for unweighted edges).
 * - uid: A unique identifier for the edge.
 * - a set of traits that define the edge's properties.
 *
 * Valid Inputs:
 * - The from and to pointers must contain valid shared pointers to Node
 * objects.
 * - The direction must be a valid EdgeDirection value.
 * - The weight must be a valid EdgeWeight type (double, int, unsigned int, or
 * std::monostate).
 */
struct Edge {
  EdgeUID uid;
  std::shared_ptr<Node> from;
  std::shared_ptr<Node> to;
  EdgeDirection direction;
  EdgeWeight weight;
  Edge

  Edge()
      : from(nullptr),
        to(nullptr),
        direction(EdgeTraits::UNDIRECTED),
        weight(std::monostate{}) {}

  Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to,
       const EdgeTraits& direction, const EdgeWeight& weight)
      : from(from ? from : std::make_shared<Node>()),
        to(to ? to : std::make_shared<Node>()),
        direction(direction),
        weight(weight) {}
};
#endif  // CORE_EDGE_H
