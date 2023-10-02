#include "gurobi_c++.h"
#include <map>

int main(int argc, char *argv[])
{
    try {
        if (argc < 2) throw(0);

        int algo = 0;
        int num_jumps = 0;
        bool relaxed = false;

        if (argc == 3 && argv[2] != "0")
            algo = atoi(argv[2]);
        if (argc == 4 && argv[3] != "0")
            relaxed = true;
        if (argc == 5)
            num_jumps = atoi(argv[4]);
        if (algo > 4) throw(1);


    } catch (int error_type) {
        switch (error_type) {
            case 0:
                std::cerr << "Wrong number of arguments. Provide at least one arguments" << std::endl;
                std::cerr << "Usage: ./ajns <file_name> [algorithm_id|default:0] ";
                std::cerr << "[relaxed|default:0] [num_jumps]" << std::endl;
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
