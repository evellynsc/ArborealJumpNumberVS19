/*
 * reader.cpp
 *
 *  Created on: 12 de fev de 2021
 *      Author: evellyn
 */

#include "reader.h"

namespace ajns {

	reader::reader(std::string path_file_name) {
		this->path_file_name = path_file_name;
		this->file_name = "";
		this->num_elements = 0;
	}

	problem_data ajns::reader::mtx_format_reader(std::ifstream& inFile) {
		auto file_line = std::string();
		inFile >> file_line;

		while (file_line[0] == '%') {
			inFile >> file_line;
		}

		auto first_line = split_string_by_char(file_line, ' ');
		this->num_elements = size_t(std::stoi(first_line[0]));
		auto num_edges = size_t(std::stoi(first_line[2]));
		
		auto element_names = std::vector<std::string>(num_elements);
		for (auto i = 0u; i < num_elements; i++) {
			element_names[i] = std::to_string(i+1);
		}

		auto matrix = std::vector<std::vector<bool>>(num_elements, std::vector<bool>(num_elements, false));
		for (auto i = 0u; i < num_edges; i++) {
			inFile >> file_line;
			auto edge = split_string_by_char(file_line, ' ');
			auto v = size_t(std::stoi(edge[0]));
			auto u = size_t(std::stoi(edge[1]));
			matrix[u][v] = true;
		}

		return problem_data(file_name, num_elements, element_names, matrix);
	}

	problem_data ajns::reader::read() {
		std::ifstream inFile;
		inFile.open(path_file_name);

		if (!inFile) {
			std::cerr << "Unable to open file\n" << path_file_name << std::endl;
			exit(1); // terminate with error
		}

		this->file_name = get_filename_from_path(path_file_name);
		auto file_line = std::string();

		inFile >> file_line;

		auto first_line = split_string_by_char(file_line, ',');
		

		this->num_elements = size_t(std::stoi(first_line[0]));
		auto read_vertices = first_line[1];



		std::cout << "reading elements ............" << std::endl;

		auto element_names = std::vector<std::string>(num_elements);
		if (read_vertices.compare("y") == 0) {
			for (auto i = 0u; i < num_elements; i++) {
				inFile >> file_line;
				element_names[i] = file_line;
			}
		} else {
			for (auto i = 0u; i < num_elements; i++) {
				element_names[i] = std::to_string(i);
			}
		}

		std::cout << "reading adjacency matrix ............" << std::endl;
		auto matrix = std::vector<std::vector<bool>>(num_elements, std::vector<bool>(num_elements, false));
		for (auto i = 0u; i < num_elements; i++) {
			for (auto j = 0u; j < num_elements; j++) {
				inFile >> file_line;
				if (file_line.compare("1") == 0)
					matrix[i][j] = true;
			}
		}
		std::cout << "finish reading adjacency matrix ............" << std::endl;

		this->adjacency_matrix = matrix;
		return problem_data(file_name, num_elements, element_names, matrix);
	}

	reader::~reader() {
		// TODO Auto-generated destructor stub
	}
}

