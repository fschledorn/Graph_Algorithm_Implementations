#ifndef ABSTRACT_EDGE_CONTAINER
#define ABSTRACT_EDGE_CONTAINER

#include "typenames.hh"
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <variant>

template <typename T>
concept NumericOrMonostate =
    std::is_arithmetic_v<T> || std::is_same_v<T, std::monostate>;

struct Edge;

// Abstract class for edges container (type erasure)
struct AbstractEdgeContainer {
    virtual ~AbstractEdgeContainer() = default;

    virtual std::size_t size() const = 0;

    virtual void addEdge(const std::shared_ptr<Edge> &) = 0;

    virtual std::shared_ptr<Edge> getEdge(const EdgeUID) const = 0;

    virtual void removeEdge(const EdgeUID &) = 0;
    virtual void removeEdge(const std::shared_ptr<Edge> &) = 0;

    virtual void findAllOutgoingEdges(
            const std::shared_ptr<std::unordered_map<EdgeUID, Edge>>) = 0;
};

#endif // !ABSTRACT_EDGE_CONTAINER
