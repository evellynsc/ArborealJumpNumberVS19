/*
 * minimal_extension.cpp
 *
 *  Created on: 2 de mai de 2021
 *      Author: evellyn
 */

#include "minimal_extension.h"
#include <boost/graph/transitive_reduction.hpp>
#include <boost/graph/graphviz.hpp>


namespace ajns {

minimal_extension::minimal_extension(instance &problem_instance) {
	this->problem_instance = problem_instance;
	this->vertex_levels = std::unordered_map<my_graph::vertex,unsigned>();
	this->num_pred_violators = std::unordered_map<my_graph::vertex,unsigned>();
	this->num_succ_violators = std::unordered_map<my_graph::vertex,unsigned>();
	this->open_violations = std::map<my_graph::vertex,std::list<my_graph::vertex>>();
	this->added_jumps = std::vector<std::pair<my_graph::vertex, my_graph::vertex>>();
	this->extension = my_graph::digraph();
	boost::copy_graph(problem_instance.covering_graph, extension);
	this->order_extension = my_graph::digraph();
	boost::copy_graph(problem_instance.order_graph, order_extension);
	this->t_order_extension = my_graph::digraph();
	boost::copy_graph(problem_instance.t_order_graph, t_order_extension);
	find_vertex_levels();
	find_num_pred_violators();
	find_violations();
}


void minimal_extension::find_violations() {
	auto violations = violator_visitor();
	auto graph = problem_instance.covering_graph;
	boost::depth_first_search(graph, boost::visitor(violations));

	for (auto v : *violations.targets) {
//		std::cout << graph[v].label << ":\t";
		auto in_edges = boost::in_edges(v, graph);
		auto sources = std::list<my_graph::vertex>();
		for (auto e : boost::make_iterator_range(in_edges)) {
			auto source = boost::source(e, graph);
			sources.push_back(source);
//			std::cout << graph[source].label << "\t";
		}
		open_violations.insert({ v, sources });
//		std::cout << std::endl;
	}
//	print_open_violations();
}


void minimal_extension::print_vertex_levels() {
	for (auto v : vertex_levels) {
		std::cout << "d[" << problem_instance.covering_graph[problem_instance.root].id << "," <<
		problem_instance.covering_graph[v.first].id << "] = " << v.second << std::endl;
	}
}

void minimal_extension::print_num_pred_violators() {
	for (auto it : num_pred_violators)
		std::cout << "p[" << problem_instance.covering_graph[it.first].label << "] = "
		<< it.second << std::endl;
}

void minimal_extension::find_vertex_levels() {
	auto sorted_vertices = std::list<my_graph::vertex>();
	boost::topological_sort(extension, std::front_inserter(sorted_vertices));

	for (auto i : sorted_vertices) {
		if (extension[i].is_root) vertex_levels[i] = 0;
		else {
			auto in_e = boost::in_edges(i, extension);
			auto max_level = 0u;
			for (auto e : boost::make_iterator_range(in_e)) {
				auto head = boost::source(e, extension);
				if (vertex_levels[head] > max_level)
					max_level = vertex_levels[head];
			}
			vertex_levels[i] = max_level+1;
		}
	}

}

void minimal_extension::find_num_pred_violators() {
	for (auto v : boost::make_iterator_range(boost::vertices(problem_instance.covering_graph))) {
		auto sum = 0u;
		auto pred = boost::adjacent_vertices(v, problem_instance.t_order_graph);
		if (boost::in_degree(v, problem_instance.covering_graph) > 1) sum++;
		for (auto u : boost::make_iterator_range(pred)) {
			if (boost::in_degree(u, problem_instance.covering_graph) > 1) {
				sum++;
			}
		}
		this->num_pred_violators[v] = sum;
	}
}

my_graph::vertex minimal_extension::choose_violator() {
	auto violator = open_violations.begin()->first;

	for (auto v : open_violations) {
		if (vertex_levels[v.first] > vertex_levels[violator])
			violator = v.first;
	}
	std::cout << "v = " << violator+1 << std::endl;
	return violator;
}

void minimal_extension::print_open_violations() {
	for (auto op : open_violations) {
		std::cout << extension[op.first].id << ": ";
		for (auto x : op.second) {
			std::cout << extension[x].id << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

my_graph::vertex minimal_extension::choose_vertex_x1(my_graph::vertex c) {
	auto array_x = open_violations[c];
	auto x1 = *array_x.begin();

	for (auto x : array_x) {
		if (num_pred_violators[x] > num_pred_violators[x1])
			x1 = x;
	}
	std::cout << "x1 = " << x1+1 << std::endl;
	return x1;
}

std::list<my_graph::vertex> minimal_extension::get_min_z(std::list<my_graph::vertex>& diff_x1_x2) {
	auto z = diff_x1_x2.front();
	auto min_z = std::list<my_graph::vertex>();
	auto z_level = vertex_levels[z];
	min_z.emplace_back(z);

	for (auto u : diff_x1_x2) {
		if (z_level > vertex_levels[u]) {
			min_z.clear();
			min_z.emplace_back(u);
			z_level = vertex_levels[u];
		} else {
			if (z_level == vertex_levels[u] and z != u) {
				min_z.emplace_back(u);
			}
		}
	}

	return min_z;
}


std::map<my_graph::vertex, std::list<my_graph::vertex>> minimal_extension::find_x2_z(my_graph::vertex c, my_graph::vertex x1) {
	auto array_x2 = open_violations[c];
	auto x2_minz = std::map<my_graph::vertex, std::list<my_graph::vertex>>();

	for (auto x2 : array_x2) {
		if (x2 != x1) {
//			std::cout << "==> x2 = " << x2 << "\t";
			auto diff = difference_x1_x2(x1,x2);
			auto min_z = get_min_z(diff);
			x2_minz[x2] = min_z;
//			for (auto i : min_z) {
//				std::cout << "l" << vertex_levels[i] << "\t";
//			}
		}
	}
	return x2_minz;
}



std::list<std::pair<my_graph::vertex, my_graph::vertex>> minimal_extension::find_edges_to_remove(std::map<my_graph::vertex, std::list<my_graph::vertex>>& map_x2_minz) {
	auto removed_edges = std::list<std::pair<my_graph::vertex, my_graph::vertex>>();
	auto map_x2_z = std::map<std::pair<my_graph::vertex, my_graph::vertex>,
				std::list<std::pair<my_graph::vertex, my_graph::vertex>>>();
	auto vec_num_edges = std::vector<int>();
	
	for (auto it : map_x2_minz) {
		auto x2 = it.first;
//		std::cout << "*x2 = " << x2 << "\t";
		auto array_z = it.second;
		for (auto z : array_z) {
//			std::cout << "*z = " << z << "\t";
			auto cur_removed_edges = std::list<std::pair<my_graph::vertex, my_graph::vertex>>();
			auto removed_jumps = std::list<std::pair<my_graph::vertex, my_graph::vertex>>();
			for (auto j : open_violations) {
				auto violator = j.first;
				auto violator_is_suc = (boost::edge(z,violator, order_extension).second or violator == z);
//				std::cout << violator_is_suc << " ((" << z << "," << violator << "))\t";
				auto array_x = j.second;
				for (auto x : array_x) {
					auto x_is_pred = (boost::edge(x,x2, order_extension).second or x == x2);
					if (x_is_pred and violator_is_suc) {
						removed_jumps.emplace_back(x,violator);
					}
				}
			}

			map_x2_z[{x2,z}]= removed_jumps;
		}
	}
	auto max = map_x2_z.begin()->second.size();
	auto selected_jump = map_x2_z.begin()->first;
	for (auto i : map_x2_z) {
		auto jump = i.first;
		auto edges = i.second;
		if (edges.size() > max) {
			max = edges.size();
			selected_jump = jump;
		}
	}
	added_jumps.push_back(selected_jump);
	curr_added_jump = selected_jump;
	std::cout << "\n(x2 = " << problem_instance.covering_graph[selected_jump.first].id + 1 << "\t z = " <<
			problem_instance.covering_graph[selected_jump.second].id + 1 << ")" << std::endl;
	return map_x2_z[selected_jump];
}


void minimal_extension::update_extension(std::map<my_graph::vertex, std::list<my_graph::vertex>> &map_x2_z) {
	auto edges_to_remove = find_edges_to_remove(map_x2_z);
	update_violations(edges_to_remove);
	boost::add_edge(curr_added_jump.first,curr_added_jump.second,
			my_graph::edge_info(-1,curr_added_jump.first,curr_added_jump.second,my_graph::ARTIFICIAL),extension);
}


void minimal_extension::update_violations(std::list<std::pair<my_graph::vertex, my_graph::vertex>>& edges) {
//	std::cout << "removing edges\n";
	for (auto e : edges) {
		this->open_violations[e.second].remove(e.first);
		boost::remove_edge(e.first, e.second, extension);
		if (this->open_violations[e.second].size() <= 1) {
			update_num_pred_violators(e.second);
			this->open_violations.erase(e.second);
		}
	}
}


std::list<my_graph::vertex> minimal_extension::difference_x1_x2(my_graph::vertex x1, my_graph::vertex x2) {
	auto p_x1 = boost::adjacent_vertices(x1,
			t_order_extension);
	auto p_x2 = boost::adjacent_vertices(x2,
			t_order_extension);
	auto in_x2 = std::map<my_graph::vertex,bool>();
	auto x1_x2 = std::list<my_graph::vertex>();
	x1_x2.emplace_back(x1);
	in_x2[x1] = false;
	x1_x2.emplace_back(x2);
	in_x2[x2] = true;

	for (auto p : boost::make_iterator_range(p_x1)) {
		in_x2[p] = false;
		x1_x2.emplace_back(p);
	}
	for (auto p : boost::make_iterator_range(p_x2)) {
		in_x2[p] = true;
		x1_x2.emplace_back(p);
	}

	auto result = std::list<my_graph::vertex>();
	for (auto x : x1_x2) {
		if (not in_x2[x])
			result.emplace_back(x);
	}
	return result;
}

void minimal_extension::update_vertex_levels() {
//	std::cout << "update_vertex_levels\n" << std::endl;
	auto last_level_head = vertex_levels[curr_added_jump.first];
	auto last_level_tail = vertex_levels[curr_added_jump.second];
	auto succ_tail = boost::adjacent_vertices(curr_added_jump.second, order_extension);

	if (vertex_levels[curr_added_jump.second] < last_level_head+1)
		vertex_levels[curr_added_jump.second] = last_level_head+1;

	for (auto v : boost::make_iterator_range(succ_tail)) {
		auto dist = vertex_levels[v] - last_level_tail;
		if (vertex_levels[v] < vertex_levels[curr_added_jump.second] + dist)
			vertex_levels[v] = vertex_levels[curr_added_jump.second] + dist;
	}
//	find_vertex_levels();
}

void minimal_extension::update_num_pred_violators(my_graph::vertex central) {
//	std::cout << "update_num_pred_violators\n" << std::endl;
	for (auto& it : num_pred_violators) {
		auto is_pred = boost::edge(central, it.first, order_extension).second;
		if (is_pred) {
			it.second--;
		}
	}
}

void minimal_extension::update_num_pred_violators() {
//	std::cout << "update_num_pred_violators2\n" << std::endl;
	auto array_x = std::list<my_graph::vertex>();
	for (auto& i : open_violations) {
		array_x.insert(array_x.end(),i.second.begin(), i.second.end());
	}
	for (auto i : array_x) {
		auto sum = 0u;
		for (auto j : open_violations) {
			auto central = j.first;
			auto is_pred = boost::edge(central, i, order_extension).second;
			if (is_pred)
				sum++;
		}
		if (num_pred_violators[i] != sum)
			num_pred_violators[i] = sum;
	}
}

void minimal_extension::update_order_extension() {
//	std::cout << "update_order_extension\n" << std::endl;
	auto jump = curr_added_jump;
	boost::add_edge(jump.first,jump.second, my_graph::edge_info(-1,jump.first,jump.second, my_graph::ARTIFICIAL),order_extension);
	boost::add_edge(jump.second,jump.first, my_graph::edge_info(-1,jump.second,jump.first, my_graph::ARTIFICIAL),t_order_extension);

	auto copy_order = my_graph::digraph();
	boost::copy_graph(order_extension, copy_order);
	auto copy_t_order = my_graph::digraph();
	boost::copy_graph(t_order_extension, copy_t_order);

	auto succ_tail = boost::adjacent_vertices(jump.second, copy_order);
	auto pred_head = boost::adjacent_vertices(jump.first, copy_t_order);


	for (auto u : boost::make_iterator_range(pred_head)) {
		for (auto v : boost::make_iterator_range(succ_tail)) {
			if (not boost::edge(u,v, order_extension).second) {
				boost::add_edge(u,v, my_graph::edge_info(-1,u,v, my_graph::ARTIFICIAL),order_extension);
				boost::add_edge(v,u, my_graph::edge_info(-1,v,u, my_graph::ARTIFICIAL),t_order_extension);
			}
		}
	}

	for (auto v : boost::make_iterator_range(succ_tail)){
		if (not boost::edge(jump.first,v, order_extension).second) {
			boost::add_edge(jump.first,v, my_graph::edge_info(-1,jump.first,v, my_graph::ARTIFICIAL), order_extension);
			boost::add_edge(v,jump.first, my_graph::edge_info(-1,v,jump.first, my_graph::ARTIFICIAL), t_order_extension);
		}
	}

	for (auto v : boost::make_iterator_range(pred_head)) {
		if (not boost::edge(v, jump.second, order_extension).second) {
			boost::add_edge(v, jump.second, my_graph::edge_info(-1,jump.first,v, my_graph::ARTIFICIAL),order_extension);
			boost::add_edge(jump.second, v, my_graph::edge_info(-1,v,jump.first, my_graph::ARTIFICIAL),t_order_extension);
		}
	}


//	std::cout << "update_order_extension\n" << std::endl;
}

void minimal_extension::run(properties &p) {
	p.num_nodes = boost::num_vertices(extension);
	p.num_arcs = boost::num_edges(extension);
	p.num_violators = open_violations.size();
	run();
	p.num_jumps = added_jumps.size();
}

void minimal_extension::run() {
	std::cout << std::endl;
	std::cout << std::endl << boost::num_vertices(extension) << " ======> vertices \n";
	std::cout << std::endl << boost::num_edges(extension) << " ======> edges \n";
	std::cout << std::endl << open_violations.size() << " ======> VIOLATORS \n";

	auto outFile = std::ofstream();
	auto name_file = problem_instance.id + "_covering.dot";
	outFile.open(name_file);
	boost::write_graphviz(outFile, extension,
			boost::make_label_writer(
					boost::get(&my_graph::vertex_info::id, extension)),
			boost::make_label_writer(
					boost::get(&my_graph::edge_info::type, extension)));
	outFile.close();
	auto dot = "dot -Tpdf ";
	auto command = std::string();
	command = dot + name_file + " -o " + problem_instance.id + "_covering.pdf";
//	std::system(command.c_str());

	while (this->open_violations.size()) {
//		print_open_violations();
//		print_vertex_levels();
		auto c  = choose_violator();
		auto x1 = choose_vertex_x1(c);
		auto map_x2_z = find_x2_z(c,x1);
		update_extension(map_x2_z);
		update_order_extension();
		update_vertex_levels();
		update_num_pred_violators();
	}

	std::cout << std::endl << added_jumps.size() << " ======> JUMPS \n";
	auto aux_graph = my_graph::digraph();
	auto g_to_tr = std::map<my_graph::vertex, my_graph::vertex>();
	boost::transitive_reduction(extension, aux_graph,
			boost::make_assoc_property_map(g_to_tr), boost::get(&my_graph::vertex_info::index, extension));
	auto tr_extension = my_graph::digraph();
	boost::copy_graph(extension, tr_extension);
	for (auto e : boost::make_iterator_range(boost::edges(extension))) {
		auto head = boost::source(e, extension);
		auto tail = boost::target(e, extension);
		if (not boost::edge(g_to_tr[head], g_to_tr[tail], aux_graph).second) {
			boost::remove_edge(head, tail, tr_extension);
		}
	}

	outFile = std::ofstream();
	name_file = problem_instance.id + "_trextension.dot";
	outFile.open(name_file);
	boost::write_graphviz(outFile, tr_extension,
			boost::make_label_writer(
					boost::get(&my_graph::vertex_info::id, tr_extension)),
			boost::make_label_writer(
					boost::get(&my_graph::edge_info::type, tr_extension)));
	outFile.close();

	command = dot + name_file + " -o " + problem_instance.id + "_trextension.pdf";
//	std::system(command.c_str());
	this->solution = tr_extension;
}

my_graph::digraph minimal_extension::get_solution() {
	return this->solution;
}

minimal_extension::~minimal_extension() {
	// TODO Auto-generated destructor stub
}

} /* namespace ajns */
