/*
 * reader.h
 *
 *  Created on: 12 de fev de 2021
 *      Author: evellyn
 */

#ifndef PREPROCESSING_READER_H_
#define PREPROCESSING_READER_H_

#include <iostream>
#include <fstream>
#include <string>

#include "../utils/preprocessing.h"
#include "problem_data.h"



namespace ajns {
	class reader {
		problem_data mtx_format_reader(std::ifstream&);
	public:
		std::string path_file_name;
		std::string file_name;
		size_t num_elements;
		std::vector<std::vector<bool>> adjacency_matrix;

		reader(std::string path_file_name);
		problem_data read();
		virtual ~reader();
	};
}

#endif /* PREPROCESSING_READER_H_ */
