#include "graph.hh"

#include <iostream>

struct Graph {
  GraphTraits traits;
  std::unordered_set<NodeUID, std::shared_ptr<Node>> node_list;
  std::unordered_set<EdgeUID, std::shared_ptr<Edge>> edge_list;

  /* [TODO: implement constructors and class methods]*/
  Graph();
  Graph(const std::vector<std::shared_ptr<Node>>&,
        const std::vector<std::shared_ptr<Edge>>&);

  /* I would ideally like to read from a setup/config file. Maybe in JSON or
  yaml or some other well structured type of format. The parsing process should
  be multithreaded to minimize I/O lag*/
  Graph(const std::fstream&);

  Graph(const Graph&);
  Graph(Graph&&) noexcept;
  Graph& operator=(const Graph&);

  void addNode(const std::shared_ptr<Node>&);
  void addEdge(const std::shared_ptr<Edge>&);
  void removeNode(const NodeUID&);
  void removeEdge(const EdgeUID&);
  void removeNode(const std::shared_ptr<Node>&);
  void removeEdge(const std::shared_ptr<Edge>&);
  bool hasTrait(const SingleGraphTrait&) const;
};

Graph::Graph()
    : traits(std::unordered_set<SingleGraphTrait>{SingleGraphTrait::EMPTY}) {}

Graph::Graph(const std::vector<std::shared_ptr<Node>>& nodes,
             const std::vector<std::shared_ptr<Edge>>& edges)
    : node_list(nodes.begin(), nodes.end()), edge_list(edges.begin(), edges.end())
    { /* [TODO]: add logic to deduct graph traits from edges and nodes*/}
