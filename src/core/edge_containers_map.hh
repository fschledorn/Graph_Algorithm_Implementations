#ifndef EDGE_CONTAINER_MAP
#define EDGE_CONTAINER_MAP

#endif // !EDGE_CONTAINER_MAP

#include "abstract_edge_container.hh"
#include "edge.hh"
#include <unordered_map>

template <NumericOrMonostate T>
struct EdgeMapContainer : AbstractEdgeContainer {
  std::shared_ptr<std::unordered_map<EdgeUID, std::shared_ptr<Edge>>>
      outgoing_edges;
  EdgeMapContainer(
      std::shared_ptr<std::unordered_map<EdgeUID, std::shared_ptr<Edge>>> e);

  std::shared_ptr<Edge> getEdge(const EdgeUID &EdgeUid) const override;

  std::size_t size() const override;

  void addEdge(const std::shared_ptr<Edge> &edge) override;

  void removeEdge(const EdgeUID &EdgeUid) override;

  void removeEdge(const std::shared_ptr<Edge> &edge) override;

  void findAllOutgoingEdges(
      const std::shared_ptr<std::unordered_map<EdgeUID, Edge>> edge_list)
      override;
};
