#include "edge_containers_map.hh"
#include "abstract_edge_container.hh"
#include <algorithm>
#include <execution>
#include <utility>
#include <vector>

template <NumericOrMonostate T>
EdgeMapContainer<T>::EdgeMapContainer(
    std::shared_ptr<std::unordered_map<EdgeUID, std::shared_ptr<Edge>>> e)
    : outgoing_edges(
          e ? e
            : std::make_shared<
                  std::unordered_map<EdgeUID, std::shared_ptr<Edge>>>()) {}

template <NumericOrMonostate T>
std::shared_ptr<Edge>
EdgeMapContainer<T>::getEdge(const EdgeUID &EdgeUid) const {
  auto it = outgoing_edges->find(EdgeUid);
  if (it != outgoing_edges->end()) {
    return it->second;
  }
  throw std::out_of_range("Edge not found");
}

template <NumericOrMonostate T> std::size_t EdgeMapContainer<T>::size() const {
  return outgoing_edges->size();
}

template <NumericOrMonostate T>
void EdgeMapContainer<T>::addEdge(const std::shared_ptr<Edge> &edge) {
  if (!outgoing_edges) {
    outgoing_edges =
        std::make_shared<std::unordered_map<EdgeUID, std::shared_ptr<Edge>>>();
  }
  outgoing_edges->insert({edge->uid, edge});
}

template <NumericOrMonostate T>
void EdgeMapContainer<T>::removeEdge(const EdgeUID &EdgeUid) {
  if (!outgoing_edges) {
    return;
  }
  auto it = outgoing_edges->find(EdgeUid);
  if (it != outgoing_edges->end()) {
    outgoing_edges->erase(it);
  }
}

template <NumericOrMonostate T>
void EdgeMapContainer<T>::removeEdge(const std::shared_ptr<Edge> &edge) {
  if (!outgoing_edges || !edge) {
    return;
  }
  auto it = std::find(outgoing_edges->begin(), outgoing_edges->end(), edge);
  if (it != outgoing_edges->end()) {
    outgoing_edges->erase(it);
  }
}

template <NumericOrMonostate T>
void EdgeMapContainer<T>::findAllOutgoingEdges(
    const std::shared_ptr<std::unordered_map<EdgeUID, Edge>> edge_list) {
  std::mutex mtx;
  std::vector<std::pair<EdgeUID, std::shared_ptr<Edge>>> local_edges;

  auto add_edge_if_from_this = [&](const auto &edge_pair) {
    if (edge_pair.second->from == this) {
      std::lock_guard<std::mutex> lock(mtx);
      local_edges.push_back({edge_pair.second->uid, edge_pair.second});
    }
  };

  std::for_each(std::execution::par, edge_list->begin(), edge_list->end(),
                add_edge_if_from_this);

  std::lock_guard<std::mutex> lock(mtx);
  for (const auto &edge : local_edges) {
    outgoing_edges->insert(edge);
  }
}
