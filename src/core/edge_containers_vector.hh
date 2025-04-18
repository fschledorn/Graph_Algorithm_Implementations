#ifndef EDGE_CONTAINERS
#define EDGE_CONTAINERS

#include "abstract_edge_container.hh"
#include "edge.hh"
#include <vector>

template <NumericOrMonostate T>
struct EdgeVectorContainer : AbstractEdgeContainer {
  std::shared_ptr<std::vector<std::shared_ptr<Edge>>> outgoing_edges;

  EdgeVectorContainer(std::shared_ptr<std::vector<std::shared_ptr<Edge>>> e);

  std::shared_ptr<Edge> getEdge(const int index) const override;

  std::size_t size() const override;

  void addEdge(const std::shared_ptr<Edge> &edge) override;

  void removeEdge(const EdgeUID &EdgeUid) override;

  void removeEdge(const std::shared_ptr<Edge> &edge) override;

  void findAllOutgoingEdges(
      const std::shared_ptr<std::unordered_map<EdgeUID, Edge>> edge_list)
      override;
};

#endif // !EDGE_CONTAINERS
