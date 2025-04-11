/*
 * This file defines the core components for representing nodes within a graph
 * structure. It includes the definition of the Node struct, which serves as the
 * fundamental building block for vertices in a graph. Additionally, it defines
 * the AbstractEdgeContainer interface, providing a flexible mechanism for
 * managing the edges connected to a node without having to specify their types
 * at compile time. This also allows for the use of different underlying data
 * structures (e.g., vector, map) for storing edges.
 *
 * Key components:
 * - NodeUID, EdgeUID: Type definitions for unique identifiers for nodes and
 * edges, respectively.
 * - AbstractEdgeContainer: An abstract base class defining the interface for
 * containers that hold edges connected to a node. This allows for different
 * underlying storage mechanisms (e.g., vector, map) for edges.
 * - Node: Represents a node (or vertex) in the graph. Each node has a unique ID
 * (uid), optional associated data (data) stored using std::any for type
 * erasure, and pointers to containers for its incoming and outgoing edges.
 * - EdgeVectorContainer and EdgeMapContainer: Concrete implementations of the
 * AbstractEdgeContainer interface, using std::vector and std::unordered_map,
 * respectively, to store edges. These classes provide methods for adding,
 * removing, and retrieving edges, as well as finding all outgoing edges from
 * the node.
 *
 *  TODO: ADD Time and Space Complexity Guarantees
 */
#ifndef CORE_NODE_H
#define CORE_NODE_H

#include "abstract_edge_container.hh"
#include "typenames.hh"
#include <any>
#include <optional>

struct Node {
  NodeUID uid;
  std::optional<std::any> data;
  std::shared_ptr<AbstractEdgeContainer> outgoing_edges;
  std::shared_ptr<AbstractEdgeContainer> incoming_edges;

  Node();
  Node(const NodeUID &nodeuid);
  Node(const NodeUID &nodeuid, std::any data,
       std::shared_ptr<AbstractEdgeContainer> outgoing_edges_list);
  Node(const Node &other);
  Node(Node &&other) noexcept;
  Node &operator=(const Node &other);
  Node &operator=(Node &&other) noexcept;
  bool operator==(const Node &other) const;

  void findAllOutgoingEdges(
      const std::shared_ptr<std::unordered_map<EdgeUID, Edge>> edge_list);
  void findAllIncomingEdges(
      const std::shared_ptr<std::unordered_map<EdgeUID, Edge>> edge_list) {
    incoming_edges->findAllOutgoingEdges(edge_list);
  }
};

#endif // !CORE_NODE_H
