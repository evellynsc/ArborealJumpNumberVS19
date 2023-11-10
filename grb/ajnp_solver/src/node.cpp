#include "headers/node.h"

void Node::set_id(long unsigned _id)
{
    this->id = _id;
}

long unsigned Node::get_id() const 
{
    return this->id;
}

bool Node::operator==(const Node& n) const
{
    return id == n.get_id();
}

bool Node::operator<(const Node& n) const
{
    return id < n.get_id();
}

long unsigned NodeHash::operator()(const Node& n) const
{
    return n.get_id();
}
