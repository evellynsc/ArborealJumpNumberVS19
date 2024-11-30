/*
 * problem_data.h
 *
 *  Created on: 15 de fev de 2021
 *      Author: evellyn
 */

#ifndef PREPROCESSING_PROBLEM_DATA_H_
#define PREPROCESSING_PROBLEM_DATA_H_

#include <vector>
#include <string>

namespace ajns {
	struct problem_data {
		std::string id = "";
		size_t num_vertices = 0;
		std::vector<std::string> vertex_labels = std::vector<std::string>();;
		std::vector<std::vector<bool>> adjacency_matrix = std::vector<std::vector<bool>>();


		problem_data() {
		}

		problem_data(std::string id,
				size_t num_vertices,
				std::vector<std::string> vertex_labels,
				std::vector<std::vector<bool>> adjacency_matrix) :
				id(id),
				num_vertices(num_vertices),
				vertex_labels (vertex_labels),
				adjacency_matrix (adjacency_matrix) {}
	};
}



#endif /* PREPROCESSING_PROBLEM_DATA_H_ */
