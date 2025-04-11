#include "graph.hh"

Graph::Graph()
    : traits(std::unordered_set<SingleGraphTrait>{SingleGraphTrait::EMPTY}) {}

Graph::Graph(const std::vector<std::shared_ptr<Node>> &nodes,
             const std::vector<std::shared_ptr<Edge>> &edges)
    : node_list(nodes.begin(), nodes.end()),
      edge_list(edges.begin(),
                edges.end()) { /* [TODO]: add logic to deduct graph traits from
                                  edges and nodes*/
}
