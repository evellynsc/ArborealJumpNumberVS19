/*
 * elementary.h
 *
 *  Created on: 14 de fev de 2021
 *      Author: evellyn
 */

#ifndef BASE_ELEMENTARY_H_
#define BASE_ELEMENTARY_H_

#include <string>
#include <boost/config.hpp>
#include <boost/graph/graph_utility.hpp>

namespace my_graph {
enum edge_type {
	ORIGINAL, ARTIFICIAL, FLOW, USINK
};

struct vertex_info {
	std::string label;
	size_t id = 0;
	size_t index = 0;
//	https://stackoverflow.com/questions/45925625/accessing-specific-vertices-in-boostgraph
//	TODO: separate temporary vertex properties out
//	boost::default_color_type color = boost::green_color;
	bool is_maximum = false;
	bool is_root = false;
	size_t level = 0;
	bool remove = false;

	vertex_info() {
		label = "";
		id = index = 0;
		remove = false;
	}
	vertex_info(std::string label) :
			label(label), id(0), index(0) {
	}
	vertex_info(std::string label, size_t id) :
			label(label), id(id), index(id) {
	}
	vertex_info(std::string label, size_t id, bool remove) :
		label(label), id(id), index(id) {
	}
	bool operator==(const vertex_info &v2) const {
		return (label == v2.label and id == v2.id);
	}
	bool operator<(const vertex_info &v2) const {
		return (id < v2.id);
	}
	bool operator>(const vertex_info &v2) const {
		return (id > v2.id);
	}
	friend std::ostream& operator<<(std::ostream& os, const vertex_info& vertex) {
	    os << "(" << vertex.id << ',' << vertex.index << "," << vertex.label << ')';
	    return os;
	}
};

struct edge_info {
	int id;
	size_t source_id;
	size_t target_id;
//	int reverse_id = -1;
	edge_type type;
	bool value_set = false;
	size_t value = 1;

//	TODO: remove this properties from here and insert into flow.h
	double capacity = 0.0;
//	double residual_capacity = 0.0;

	edge_info() {
		id = -1;
		source_id = 0;
		target_id = 0;
		type = ORIGINAL;
		value_set = false;
		value = 1;
	}
	edge_info(size_t id, size_t source_id, size_t target_id, edge_type type) :
			id(id), source_id(source_id), target_id(target_id), type(type) { }

	edge_info(size_t id, size_t source_id, size_t target_id, edge_type type, bool value_set, size_t value) :
		id(id), source_id(source_id), target_id(target_id), type(type), value_set(value_set), value(value) { }

	edge_info(size_t id, size_t source_id, size_t target_id, edge_type type,
			double capacity) :
			id(id), source_id(source_id), target_id(target_id), type(type), capacity(
					capacity) {	}

	edge_info(size_t source_id, size_t target_id, edge_type type) :
			source_id(source_id), target_id(target_id), type(type) { id = 0; }

	bool operator==(const edge_info &e2) const {
		return (id == e2.id and source_id == e2.source_id
				and target_id == e2.target_id);
	}

	friend std::ostream& operator<<(std::ostream& os, const edge_info& edge) {
	    os << "(" << edge.id << ',' << edge.source_id << "," << edge.target_id << ')';
	    return os;
	}
};
}
#endif /* BASE_ELEMENTARY_H_ */
