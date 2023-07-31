#include "headers/edge.h"

void Edge::set_id(long unsigned _id)
{
    id = _id;
}

const std::unique_ptr<Node>& Edge::get_source() const
{
    return this->source;
}

const std::unique_ptr<Node>& Edge::get_target() const
{
    return this->target;
}

bool Edge::operator==(const Edge& e) const 
{
    return (e.get_source()->get_id() == source->get_id()) and (e.get_target()->get_id() == target->get_id());
}

long unsigned Edge::get_id() const
{
    return this->id;
}

long unsigned EdgeHash::operator()(const Edge& e) const
{
    return e.get_id();
}