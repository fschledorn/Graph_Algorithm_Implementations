#include <gtest/gtest.h>
#include <memory>
#include <any>
#include <vector>
#include <unordered_map>

#include "core/node.hh"
#include "core/edge.hh"
#include "core/edge_containers_vector.hh"
#include "core/edge_containers_map.hh"
#include "core/graph.hh"

// Node tests
TEST(NodeTest, DefaultConstructor) {
    Node n;
    EXPECT_EQ(n.uid, std::numeric_limits<NodeUID>::max());
    EXPECT_FALSE(n.data.has_value());
}

TEST(NodeTest, UIDConstructor) {
    Node n(42);
    EXPECT_EQ(n.uid, 42);
}

TEST(NodeTest, EqualityOperator) {
    Node n1(1);
    Node n2(1);
    Node n3(2);
    EXPECT_TRUE(n1 == n2);
    EXPECT_FALSE(n1 == n3);
}

// Edge tests
TEST(EdgeTest, ConstructionAndTraits) {
    auto n1 = std::make_shared<Node>(1);
    auto n2 = std::make_shared<Node>(2);
    EdgeTraits traits{std::unordered_set<EdgeTypes>{EdgeTypes::DIRECTED}};
    EdgeWeight weight = 3.14;
    Edge e(n1, n2, traits, weight);
    EXPECT_EQ(e.from->uid, 1);
    EXPECT_EQ(e.to->uid, 2);
    EXPECT_TRUE(e.hasTrait(EdgeTypes::DIRECTED));
    EXPECT_FALSE(e.hasTrait(EdgeTypes::UNDIRECTED));
}

// EdgeVectorContainer tests
TEST(EdgeVectorContainerTest, AddAndGetEdge) {
    EdgeVectorContainer<double> container(nullptr);
    auto n1 = std::make_shared<Node>(1);
    auto n2 = std::make_shared<Node>(2);
    EdgeTraits traits{std::unordered_set<EdgeTypes>{EdgeTypes::DIRECTED}};
    auto edge = std::make_shared<Edge>(n1, n2, traits, 1.0);
    container.addEdge(edge);
    EXPECT_EQ(container.size(), 1);
    EXPECT_EQ(container.getEdge(0), edge);
}

TEST(EdgeVectorContainerTest, RemoveEdge) {
    EdgeVectorContainer<double> container(nullptr);
    auto n1 = std::make_shared<Node>(1);
    auto n2 = std::make_shared<Node>(2);
    EdgeTraits traits{std::unordered_set<EdgeTypes>{EdgeTypes::DIRECTED}};
    auto edge = std::make_shared<Edge>(n1, n2, traits, 1.0);
    container.addEdge(edge);
    container.removeEdge(edge->uid);
    EXPECT_EQ(container.size(), 0);
}

// EdgeMapContainer tests
TEST(EdgeMapContainerTest, AddAndGetEdge) {
    EdgeMapContainer<double> container(nullptr);
    auto n1 = std::make_shared<Node>(1);
    auto n2 = std::make_shared<Node>(2);
    EdgeTraits traits{std::unordered_set<EdgeTypes>{EdgeTypes::DIRECTED}};
    auto edge = std::make_shared<Edge>(n1, n2, traits, 2.0);
    container.addEdge(edge);
    EXPECT_EQ(container.size(), 1);
    EXPECT_EQ(container.getEdge(edge->uid), edge);
}

TEST(EdgeMapContainerTest, RemoveEdge) {
    EdgeMapContainer<double> container(nullptr);
    auto n1 = std::make_shared<Node>(1);
    auto n2 = std::make_shared<Node>(2);
    EdgeTraits traits{std::unordered_set<EdgeTypes>{EdgeTypes::DIRECTED}};
    auto edge = std::make_shared<Edge>(n1, n2, traits, 2.0);
    container.addEdge(edge);
    container.removeEdge(edge->uid);
    EXPECT_EQ(container.size(), 0);
}

// Graph tests
TEST(GraphTest, DefaultConstructor) {
    Graph g;
    EXPECT_TRUE(g.hasTrait(SingleGraphTrait::EMPTY));
}

TEST(GraphTest, AddNodeAndEdge) {
    Graph g;
    auto n1 = std::make_shared<Node>(1);
    auto n2 = std::make_shared<Node>(2);
    g.addNode(n1);
    g.addNode(n2);
    EdgeTraits traits{std::unordered_set<EdgeTypes>{EdgeTypes::DIRECTED}};
    auto edge = std::make_shared<Edge>(n1, n2, traits, 1.0);
    g.addEdge(edge);
    // You can add more checks if Graph exposes node/edge counts
}
