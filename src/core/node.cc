#include "node.hh"
#include "abstract_edge_container.hh"

Node::Node() : uid(std::numeric_limits<NodeUID>::max()), data(std::nullopt) {}

Node::Node(const NodeUID &nodeuid) : uid(nodeuid), data(std::nullopt) {}

Node::Node(const NodeUID &nodeuid, std::any data,
           std::shared_ptr<AbstractEdgeContainer> outgoing_edges_list)
    : uid(nodeuid), data(data), outgoing_edges(outgoing_edges_list) {}

Node::Node(const Node &other)
    : uid(other.uid), data(other.data), outgoing_edges(other.outgoing_edges) {}

Node::Node(Node &&other) noexcept
    : uid(std::move(other.uid)), data(std::move(other.data)),
      outgoing_edges(std::move(other.outgoing_edges)) {
  other.uid = std::numeric_limits<NodeUID>::max();
  other.data.reset();
}

Node &Node::operator=(const Node &other) {
  if (this != &other) {
    uid = other.uid;
    data = other.data;
    outgoing_edges = other.outgoing_edges;
  }
  return *this;
}

Node &Node::operator=(Node &&other) noexcept {
  if (this != &other) {
    uid = std::move(other.uid);
    data = std::move(other.data);
    outgoing_edges = std::move(other.outgoing_edges);
    other.uid = std::numeric_limits<NodeUID>::max();
    other.data.reset();
  }
  return *this;
}

bool Node::operator==(const Node &other) const { return uid == other.uid; }

void Node::findAllOutgoingEdges(
    const std::shared_ptr<std::unordered_map<EdgeUID, Edge>> edge_list) {
  outgoing_edges->findAllOutgoingEdges(edge_list);
}
