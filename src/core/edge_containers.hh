#ifndef EDGE_CONTAINERS
#define EDGE_CONTAINERS

#include "edge.hh"
#include "node.hh"

template <typename T>
concept NumericOrMonostate =
    std::is_arithmetic_v<T> || std::is_same_v<T, std::monostate>;

template <NumericOrMonostate T>
struct EdgeVectorContainer : AbstractEdgeContainer {
  std::shared_ptr<std::vector<std::shared_ptr<Edge>>> outgoing_edges;
  EdgeVectorContainer(std::shared_ptr<std::vector<std::shared_ptr<Edge>>> e)
      : outgoing_edges(
            e ? e : std::make_shared<std::vector<std::shared_ptr<Edge>>>()) {}

  std::shared_ptr<Edge> getEdge(const int index) const override {
    if (index >= 0 && index < outgoing_edges->size()) {
      return (*outgoing_edges)[index];
    }
    throw std::out_of_range("Index out of range");
  }

  std::size_t size() const override { return outgoing_edges->size(); }

  void addEdge(const std::shared_ptr<Edge> &edge) override {
    if (!outgoing_edges) {
      outgoing_edges = std::make_shared<std::vector<std::shared_ptr<Edge>>>();
    }
    outgoing_edges->push_back(edge);
  }

  void removeEdge(const EdgeUID &EdgeUid) override {
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

  void removeEdge(const std::shared_ptr<Edge> &edge) override {
    if (!outgoing_edges || !edge) {
      return;
    }
    auto it = std::find(outgoing_edges->begin(), outgoing_edges->end(), edge);
    if (it != outgoing_edges->end()) {
      outgoing_edges->erase(it);
    }
  }

  void findAllOutgoingEdges(
      const std::shared_ptr<std::unordered_map<EdgeUID, Edge>> edge_list)
      override {
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
};

template <NumericOrMonostate T>
struct EdgeMapContainer : AbstractEdgeContainer {
  std::shared_ptr<std::unordered_map<EdgeUID, std::shared_ptr<Edge>>>
      outgoing_edges;
  EdgeMapContainer(
      std::shared_ptr<std::unordered_map<EdgeUID, std::shared_ptr<Edge>>> e)
      : outgoing_edges(
            e ? e
              : std::make_shared<
                    std::unordered_map<EdgeUID, std::shared_ptr<Edge>>>()) {}

  std::shared_ptr<Edge> getEdge(const EdgeUID &EdgeUid) const override {
    auto it = outgoing_edges->find(EdgeUid);
    if (it != outgoing_edges->end()) {
      return it->second;
    }
    throw std::out_of_range("Edge not found");
  }

  std::size_t size() const override { return outgoing_edges->size(); }

  void addEdge(const std::shared_ptr<Edge> &edge) override {
    if (!outgoing_edges) {
      outgoing_edges = std::make_shared<
          std::unordered_map<EdgeUID, std::shared_ptr<Edge>>>();
    }
    outgoing_edges->insert({edge->uid, edge});
  }

  void removeEdge(const EdgeUID &EdgeUid) override {
    if (!outgoing_edges) {
      return;
    }
    auto it = outgoing_edges->find(EdgeUid);
    if (it != outgoing_edges->end()) {
      outgoing_edges->erase(it);
    }
  }

  void removeEdge(const std::shared_ptr<Edge> &edge) override {
    if (!outgoing_edges || !edge) {
      return;
    }
    auto it = std::find(outgoing_edges->begin(), outgoing_edges->end(), edge);
    if (it != outgoing_edges->end()) {
      outgoing_edges->erase(it);
    }
  }

  void findAllOutgoingEdges(
      const std::shared_ptr<std::unordered_map<EdgeUID, Edge>> edge_list)
      override {
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
};

#endif // !EDGE_CONTAINERS
