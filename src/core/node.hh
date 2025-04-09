/*
 * This file defines the core components for representing nodes within a graph structure.
 * It includes the definition of the Node struct, which serves as the fundamental building 
 * block for vertices in a graph. Additionally, it defines the AbstractEdgeContainer interface, 
 * providing a flexible mechanism for managing the edges connected to a node without having
 * to specify their types at compile time. This also allows for the use of different
 * underlying data structures (e.g., vector, map) for storing edges.
 * 
 * Key components:
 * - NodeUID, EdgeUID: Type definitions for unique identifiers for nodes and edges, respectively.
 * - AbstractEdgeContainer: An abstract base class defining the interface for containers that hold 
 *   edges connected to a node. This allows for different underlying storage mechanisms (e.g., vector, map)
 *   for edges.
 * - Node: Represents a node (or vertex) in the graph. Each node has a unique ID (uid), optional 
 *   associated data (data) stored using std::any for type erasure, and pointers to containers for 
 *   its incoming and outgoing edges.
 * - EdgeVectorContainer and EdgeMapContainer: Concrete implementations of the AbstractEdgeContainer
 *   interface, using std::vector and std::unordered_map, respectively, to store edges. These classes
 *   provide methods for adding, removing, and retrieving edges, as well as finding all outgoing edges
 *   from the node. 
 *
 *  TODO: ADD Time and Space Complexity Guarantees 
 */
#ifndef CORE_NODE_H
#define CORE_NODE_H

#include <any>
#include <exception>
#include <future>
#include <memory>
#include <optional>
#include <type_traits>
#include <unordered_map>
#include <vector>


typedef size_t NodeUID;
typedef size_t EdgeUID;

struct Edge;

// Abstract class for edges container (type erasure)
struct AbstractEdgeContainer {
  virtual ~AbstractEdgeContainer() = default;
  virtual std::size_t size() const = 0;
  virtual std::shared_ptr<Edge> getEdge(const std::size_t&) const = 0;
  virtual void addEdge(const std::shared_ptr<Edge>&) = 0;
  virtual void removeEdge(const EdgeUID&) = 0;
  virtual void removeEdge(const std::shared_ptr<Edge>&) = 0;
  virtual void findAllOutgoingEdges(
      const std::shared_ptr<std::unordered_map<EdgeUID, Edge>>) = 0; //todo: arbitrary type
};

struct Node {
  NodeUID uid;
  std::optional<std::any> data;
  std::shared_ptr<AbstractEdgeContainer> outgoing_edges;
  std::shared_ptr<AbstractEdgeContainer> incoming_edges;

  Node();
  Node(const NodeUID& nodeuid);
  Node(const NodeUID& nodeuid, std::any data,
       std::shared_ptr<AbstractEdgeContainer> outgoing_edges_list);
  Node(const Node& other);
  Node(Node&& other) noexcept;
  Node& operator=(const Node& other);
  Node& operator=(Node&& other) noexcept;
  bool operator==(const Node& other) const;

  void findAllOutgoingEdges(
      const std::shared_ptr<std::unordered_map<EdgeUID, Edge>> edge_list);
};

Node::Node() : uid(std::numeric_limits<NodeUID>::max()), data(std::nullopt) {}

Node::Node(const NodeUID& nodeuid)
    : uid(nodeuid), data(std::nullopt) {}

Node::Node(const NodeUID& nodeuid, std::any data,
           std::shared_ptr<AbstractEdgeContainer> outgoing_edges_list)
    : uid(nodeuid), data(data), outgoing_edges(outgoing_edges_list) {}

Node::Node(const Node& other)
    : uid(other.uid), data(other.data), outgoing_edges(other.outgoing_edges) {}

Node::Node(Node&& other) noexcept
    : uid(std::move(other.uid)),
      data(std::move(other.data)),
      outgoing_edges(std::move(other.outgoing_edges)) {
  other.uid = std::numeric_limits<NodeUID>::max();
  other.data.reset();
}

Node& Node::operator=(const Node& other) {
  if (this != &other) {
    uid = other.uid;
    data = other.data;
    outgoing_edges = other.outgoing_edges;
  }
  return *this;
}

Node& Node::operator=(Node&& other) noexcept {
  if (this != &other) {
    uid = std::move(other.uid);
    data = std::move(other.data);
    outgoing_edges = std::move(other.outgoing_edges);
    other.uid = std::numeric_limits<NodeUID>::max();
    other.data.reset();
  }
  return *this;
}

bool Node::operator==(const Node& other) const { return uid == other.uid; }

void Node::findAllOutgoingEdges(
    const std::shared_ptr<std::unordered_map<EdgeUID, Edge>> edge_list) {
  outgoing_edges->findAllOutgoingEdges(edge_list);
}

template <typename T>
concept NumericOrMonostate =
    std::is_arithmetic_v<T> || std::is_same_v<T, std::monostate>;

template <NumericOrMonostate T>
struct EdgeVectorContainer : AbstractEdgeContainer {
  std::shared_ptr<std::vector<std::shared_ptr<Edge>>> outgoing_edges;
  EdgeVectorContainer(std::shared_ptr<std::vector<std::shared_ptr<Edge>>> e)
      : outgoing_edges(
            e ? e : std::make_shared<std::vector<std::shared_ptr<Edge>>>()) {}

  std::shared_ptr<Edge> getEdge(int index) const override {
    if (index >= 0 && index < outgoing_edges->size()) {
      return (*outgoing_edges)[index];
    }
    throw std::out_of_range("Index out of range");
  }

  std::size_t size() const override { return outgoing_edges->size(); }

  void addEdge(const std::shared_ptr<Edge>& edge) override {
    if (!outgoing_edges) {
      outgoing_edges = std::make_shared<std::vector<std::shared_ptr<Edge>>>();
    }
    outgoing_edges->push_back(edge);
  }

  void removeEdge(const EdgeUID& EdgeUid) override {
    if (!outgoing_edges) {
      return;
    }
    auto it =
        std::find_if(outgoing_edges->begin(), outgoing_edges->end(),
                     [&EdgeUid](const std::shared_ptr<Edge>& outgoing_edges) {
                       return edge->uid == EdgeUid;
                     });
    if (it != outgoing_edges->end()) {
      outgoing_edges->erase(it);
    }
  }

  void removeEdge(const std::shared_ptr<Edge>& edge) override {
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

    auto add_edge_if_from_this = [&](const auto& edge_pair) {
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

  std::shared_ptr<Edge> getEdge(const EdgeUID& EdgeUid) const override {
    auto it = outgoing_edges->find(EdgeUid);
    if (it != outgoing_edges->end()) {
      return it->second;
    }
    throw std::out_of_range("Edge not found");
  }

  std::size_t size() const override { return outgoing_edges->size(); }

  void addEdge(const std::shared_ptr<Edge>& edge) override {
    if (!outgoing_edges) {
      outgoing_edges = std::make_shared<
          std::unordered_map<EdgeUID, std::shared_ptr<Edge>>>();
    }
    outgoing_edges->insert({edge->uid, edge});
  }

  void removeEdge(const EdgeUID& EdgeUid) override {
    if (!outgoing_edges) {
      return;
    }
    auto it = outgoing_edges->find(EdgeUid);
    if (it != outgoing_edges->end()) {
      outgoing_edges->erase(it);
    }
  }

  void removeEdge(const std::shared_ptr<Edge>& edge) override {
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

    auto add_edge_if_from_this = [&](const auto& edge_pair) {
      if (edge_pair.second->from == this) {
        std::lock_guard<std::mutex> lock(mtx);
        local_edges.push_back({edge_pair.second->uid, edge_pair.second});
      }
    };

    std::for_each(std::execution::par, edge_list->begin(), edge_list->end(),
                  add_edge_if_from_this);

    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& edge : local_edges) {
      outgoing_edges->insert(edge);
    }
  }
};

#endif  // CORE_NODE_H
