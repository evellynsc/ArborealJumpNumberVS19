/*
 * writer.h
 *
 *  Created on: 23 de fev de 2021
 *      Author: evellyn
 */

#ifndef OUTPUT_WRITER_H_
#define OUTPUT_WRITER_H_

namespace ajns {
class writer {
public:
	writer();
	void save_solution_to_file();
	virtual ~writer();
};
}

#endif /* OUTPUT_WRITER_H_ */
