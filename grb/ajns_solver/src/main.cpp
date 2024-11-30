#include "headers/instance.h"
#include "headers/global.h"
#include "headers/solver.h"
#include "headers/feasibility_solver.h"
#include "headers/feasibility_solver_relaxed.h"
#include "ortools/gurobi/environment.h"
#include "ortools/base/logging.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <memory>
#include <string>


int main(int argc, char *argv[])
{
    try {
        // FLAGS_logtostderr = true;
        std::vector<std::string> dirs;

        
        absl::SetFlag(&FLAGS_logtostderr, false);
        // absl::SetFlag(&FLAGS_log_dir, "/ajns/");
        google::InitGoogleLogging(argv[0]);
        // absl::InitializeLog();
        //ALERT: as duas próximas linhas só precisam ser colocadas no Docker
        std::vector<std::string> gurobi_path { "/opt/gurobi/linux64/lib/libgurobi110.so" };
        operations_research::LoadGurobiDynamicLibrary(gurobi_path);
        std::cout << "number of args: " << argc << std::endl;
        if (argc < 3 || argc > 6) {
            std::cout << "Wrong number of arguments.\n";
            throw(0);
        }


        int algo = 0;
        int nparts = 1;
        bool relaxed = false;
        bool asc = true;
	// FIXME: change to switch
        // algorithm id
        if (argc >= 3 && strcmp(argv[2], "0") != 0) {
            algo = atoi(argv[2]);
            LOG(INFO) << "Id algorithm: " << algo << std::endl;
            std::cout << "Id algorithm: " << algo << std::endl;
        }
        // relaxed model
        if (argc >= 4 && strcmp(argv[3], "0") != 0) {
            relaxed = true;
            LOG(INFO) << "Running relaxed model." << std::endl;
            std::cout << "Running relaxed model." << std::endl;
        }
        // max number of parts to be tested
        if (argc >= 5) {            
            nparts = atoi(argv[4]);
            LOG(INFO) << "Total number of parts: " << nparts << std::endl;
            std::cout << "Total number of parts: " << nparts << std::endl;

        }
        // running in ascending order
	    if (argc == 6 && strcmp(argv[5], "0") == 0)
            asc = false;
        if (algo > 4) {
            LOG(ERROR) << "This algorithm does not exist. Choose from 0 to 3.";
            throw(1);
        }

        std::cout   << "number of parts: " << nparts << std::endl
                    << "relaxed: " << relaxed << std::endl
                    << "algorithm id: " << algo << std::endl
                    << "asceding order: " << asc << std::endl;

    bool found;
    auto instance = std::make_unique<Instance>(argv[1], TXT, false);

	if (not asc) {
        std::cout << "descending order\n";
    	for (int i = nparts; i >= 1; i--)
    	{
    	    LOG(INFO) << "Number of parts: " << i << std::endl;
            std::cout << "Number of parts: " << i << std::endl;
    	    if (relaxed) {
        		auto solver = operations_research::FeasibilitySolverRelaxed("characterization", std::move(instance), i, GUROBI);
        		solver.create_model();
        		solver.solve_model();
    	    } else {
        		auto solver = operations_research::FeasibilitySolver("characterization", std::move(instance), i, GUROBI);
        		solver.create_model();
        		found = solver.solve_model();
    	    }
            std::cout << "== " << found << std::endl;
            if (found) {
                std::cout << "======== Jump number is " << nparts-1 << ". ========\n";
                break;
            }
    	}
	} else {
        std::cout << "ascending order\n";
        for (int i = 1; i <= nparts; i++)
    	{
    	    std::cout << "Number of parts: " << i << std::endl;
    	    if (relaxed) {
        		auto solver = operations_research::FeasibilitySolverRelaxed("characterization", std::move(instance), i, GUROBI);
        		solver.create_model();
        		solver.solve_model();
    	    } else {
        		auto solver = operations_research::FeasibilitySolver("characterization", std::move(instance), i, GUROBI);
        		solver.create_model();
        		found = solver.solve_model();
    	    }
            std::cout << "== " << found << std::endl;
            if (found) {
                std::cout << "======== Jump number is " << nparts << ". ========\n";
                break;
            }
            std::cout << "== " << found << std::endl;
    	}
    }

        return EXIT_SUCCESS;
    } catch (int error_type) {
        switch (error_type) {
            case 0:
                std::cerr << "Wrong number of arguments. Provide at least one arguments" << std::endl;
                std::cerr << "Usage: ./ajns <file_name> [algorithm_id|default:0] ";
                std::cerr << "[relaxed|default:0] [max_num_jumps] []" << std::endl;
                return 1;
            case 1:
                std::cerr << "Wrong algorithm id. Choose a number between 0 and 4." << std::endl;
                return 1;
        }
    }
    return 0;

  // try {
  //
  //   // Create an environment
  //   GRBEnv env = GRBEnv(true);
  //   env.set("LogFile", "mip1.log");
  //   env.start();
  //
  //   // Create an empty model
  //   GRBModel model = GRBModel(env);
  //
  //   // Create variables
  //   GRBVar x = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x");
  //   GRBVar y = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y");
  //   GRBVar z = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "z");
  //
  //   // Set objective: maximize x + y + 2 z
  //   model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);
  //
  //   // Add constraint: x + 2 y + 3 z <= 4
  //   model.addConstr(x + 2 * y + 3 * z <= 4, "c0");
  //
  //   // Add constraint: x + y >= 1
  //   model.addConstr(x + y >= 1, "c1");
  //
  //   // Optimize model
  //   model.optimize();
  //
  //   std::cout << x.get(GRB_StringAttr_VarName) << " "
  //        << x.get(GRB_DoubleAttr_X) << std::endl;
  //   std::cout << y.get(GRB_StringAttr_VarName) << " "
  //        << y.get(GRB_DoubleAttr_X) << std::endl;
  //   std::cout << z.get(GRB_StringAttr_VarName) << " "
  //        << z.get(GRB_DoubleAttr_X) << std::endl;
  //
  //   std::cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;
  //
  // } catch(GRBException e) {
  //   std::cout << "Error code = " << e.getErrorCode() << std::endl;
  //   std::cout << e.getMessage() << std::endl;
  // } catch(...) {
  //   std::cout << "Exception during optimization" << std::endl;
  // }return 0;
}
