#pragma once

#include "global.h"
#include "node.h"
#include <memory>

class Edge
{
    long unsigned id;
    EdgeType type;
    double weight;
    std::unique_ptr<Node> source;
    std::unique_ptr<Node> target;

    Edge() : id(0), type(INPUT) {}

public:
    Edge(const Node& s, const Node& t) : id(0), type(INPUT), source(std::make_unique<Node>(s)), target(std::make_unique<Node>(t)) {}
    Edge(const Node& s, const Node& t, long unsigned _id) : id(_id), type(INPUT), source(std::make_unique<Node>(s)), target(std::make_unique<Node>(t)) {}
    Edge(const Node& s, const Node& t, long unsigned _id, EdgeType _type) : id(_id), type(_type), source(std::make_unique<Node>(s)), target(std::make_unique<Node>(t)) {}

    void set_id(long unsigned _id);
    long unsigned get_id() const;

    const std::unique_ptr<Node>& get_source() const;
    const std::unique_ptr<Node>& get_target() const;

    bool operator==(const Edge&) const;
    ~Edge() = default;
};

class EdgeHash {
public:
    long unsigned operator()(const Edge&) const;
};