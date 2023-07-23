/*
 * preprocessing.h
 *
 *  Created on: 12 de fev de 2021
 *      Author: evellyn
 */

#ifndef UTILS_PREPROCESSING_H_
#define UTILS_PREPROCESSING_H_

#include <iostream>
#include <vector>


inline std::vector<std::string> split_string_by_char(std::string _string, char _char) {
	std::string buffer = "";
	std::vector<std::string> result;
	for (auto s : _string) {
		if (s != _char) {
			buffer += s;
		} else {
			result.push_back(buffer);
			buffer = "";
		}
	}
	if (buffer != "") result.push_back(buffer);

	return result;
}


inline std::string get_filename_from_path(std::string _path) {
	auto path_vector = split_string_by_char(_path, '/');
	auto filename = path_vector[path_vector.size()-1];

	return filename;
}

#endif /* UTILS_PREPROCESSING_H_ */
