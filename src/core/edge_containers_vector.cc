#include "edge_containers_vector.hh"
#include "abstract_edge_container.hh"
#include "edge.hh"
#include <cstddef>
#include <memory>

template <NumericOrMonostate T>
EdgeVectorContainer<T>::EdgeVectorContainer(
    std::shared_ptr<std::vector<std::shared_ptr<Edge>>> e)
    : outgoing_edges(
          e ? e : std::make_shared<std::vector<std::shared_ptr<Edge>>>()) {}

template <NumericOrMonostate T>
std::shared_ptr<Edge> EdgeVectorContainer<T>::getEdge(const int index) const {
  if (index >= 0 && index < outgoing_edges->size()) {
    return (*outgoing_edges)[index];
  }
  throw std::out_of_range("Index out of range");
}

template <NumericOrMonostate T>
std::size_t EdgeVectorContainer<T>::size() const {
  return outgoing_edges->size();
}

template <NumericOrMonostate T>
void EdgeVectorContainer<T>::addEdge(const std::shared_ptr<Edge> &edge) {
  if (!outgoing_edges) {
    outgoing_edges = std::make_shared<std::vector<std::shared_ptr<Edge>>>();
  }
  outgoing_edges->push_back(edge);
}

template <NumericOrMonostate T>
void EdgeVectorContainer<T>::removeEdge(const EdgeUID &EdgeUid) {
  if (!outgoing_edges) {
    return;
  }
  auto it = std::find_if(outgoing_edges->begin(), outgoing_edges->end(),
                         [&EdgeUid](const std::shared_ptr<Edge> &edge) {
                           return edge->uid == EdgeUid;
                         });
  if (it != outgoing_edges->end()) {
    outgoing_edges->erase(it);
  }
}

template <NumericOrMonostate T>
void EdgeVectorContainer<T>::removeEdge(const std::shared_ptr<Edge> &edge) {
  if (!outgoing_edges || !edge) {
    return;
  }
  auto it = std::find(outgoing_edges->begin(), outgoing_edges->end(), edge);
  if (it != outgoing_edges->end()) {
    outgoing_edges->erase(it);
  }
}

template <NumericOrMonostate T>
void EdgeVectorContainer<T>::findAllOutgoingEdges(
    const std::shared_ptr<std::unordered_map<EdgeUID, Edge>> edge_list) {
  std::vector<std::shared_ptr<Edge>> local_edges;
  std::mutex mtx;

  auto add_edge_if_from_this = [&](const auto &edge_pair) {
    if (edge_pair.second->from == this) {
      std::lock_guard<std::mutex> lock(mtx);
      local_edges.push_back(edge_pair.second);
    }
  };

  std::for_each(std::execution::par, edge_list->begin(), edge_list->end(),
                add_edge_if_from_this);

  std::lock_guard<std::mutex> lock(mtx);
  outgoing_edges->insert(outgoing_edges->end(), local_edges.begin(),
                         local_edges.end());
}
