#pragma once

#include<iostream>
#include<cstring>

class Node
{
	long unsigned id; //static can change during execution
	std::string label; //fixed, can't change during execution
	bool to_be_removed;
	bool is_maximum;
	bool is_root;

public:
	Node() : id(0), label(""), to_be_removed(false), is_maximum(false), is_root(false) {}
	Node(long unsigned _id) : id(_id), to_be_removed(false), is_maximum(false), is_root(false) { label = std::to_string(_id); }
	Node(long unsigned _id, std::string _label) : id(_id), label(_label), to_be_removed(false), is_maximum(false), is_root(false) {}
	Node(long unsigned _id, bool _is_root, bool _to_be_removed) : id(_id), to_be_removed(_to_be_removed), is_maximum(false), is_root(_is_root) {}
	Node(long unsigned _id, bool _is_root, bool _to_be_removed, bool _is_maximum) : id(_id), to_be_removed(_to_be_removed), is_maximum(_is_maximum), is_root(_is_root) {}

	void set_id(long unsigned _id);
	long unsigned get_id() const;
	bool operator==(const Node&) const;
	bool operator<(const Node&) const;
};


class NodeHash {
public:
    long unsigned operator()(const Node& n) const;
};