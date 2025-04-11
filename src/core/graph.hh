#ifndef CORE_GRAPH_H
#define CORE_GRAPH_H
#include "edge.hh"
#include "node.hh"
#include "typenames.hh"
#include <memory>
#include <unordered_set>
#include <vector>

template <typename T>
concept EdgeType = std::is_same_v<T, EdgeWeight>;

/*
 * How can I make this better? I want to be able to specify multiple types of
 * graphs. Some should be able to be combined with each other based on a
 * predefined logic. For example, when a graph is cyclic, it must be directed.
 * The goal is to create a flexible and extensible way to define and combine
 * different graph properties.
 */
enum class SingleGraphTrait {
  UNDETERMINED,
  DIRECTED,
  UNDIRECTED,
  WEIGHTED,
  UNWEIGHTED,
  CYCLIC,
  ACYCLIC,
  CONNECTED,
  DISCONNECTED,
  EMPTY
};

struct GraphTraits {
  std::unordered_set<SingleGraphTrait> data;
  // TOdo : Add a constructor to initialize the traits and perform checking
  // logic for the traits.
};

/*
 * The Graph struct represents a graph, which is a collection of nodes and
 * edges.
 *
 * Members:
 * - nodes: A vector of shared pointers to Node objects representing the nodes
 * in the graph.
 * - edges: A vector of shared pointers to Edge objects representing the edges
 * in the graph.
 *
 * Valid Inputs:
 * - The nodes vector must contain valid shared pointers to Node objects.
 * - The edges vector must contain valid shared pointers to Edge objects.
 */

struct Graph {
  GraphTraits traits;
  std::unordered_set<NodeUID, std::shared_ptr<Node>> node_list;
  std::unordered_set<EdgeUID, std::shared_ptr<Edge>> edge_list;

  /* [TODO: implement constructors and class methods]*/
  Graph();
  Graph(const std::vector<std::shared_ptr<Node>> &,
        const std::vector<std::shared_ptr<Edge>> &);

  /* I would ideally like to read from a setup/config file. Maybe in JSON or
  yaml or some other well structured type of format. The parsing process should
  be multithreaded to minimize I/O lag*/
  Graph(const std::fstream &);

  Graph(const Graph &);
  Graph(Graph &&) noexcept;
  Graph &operator=(const Graph &);

  void addNode(const std::shared_ptr<Node> &);
  void addEdge(const std::shared_ptr<Edge> &);
  void removeNode(const NodeUID &);
  void removeEdge(const EdgeUID &);
  void removeNode(const std::shared_ptr<Node> &);
  void removeEdge(const std::shared_ptr<Edge> &);
  bool hasTrait(const SingleGraphTrait &) const;
};

#endif // CORE_GRAPH_H
