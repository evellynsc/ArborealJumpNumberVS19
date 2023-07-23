#include "../headers/formulations.h"

#include "../headers/characterization_based_formulation.h"

const string Formulations::characterizationBasedFormulation = "characterizationbased";


ExactSolver* Formulations::GetFormulation(Instance instance, string formulation) {
    ExactSolver* ptr = NULL;
    if (formulation == characterizationBasedFormulation) {
        ptr = new CharacterizationBasedFormulation(instance);
    }
   /* else if (formulation == kFlowBasedRelaxed) {
        ptr = new FlowBasedRelaxed(internalS);
    }
    else if (formulation == kFlowBasedFormulation) {
        ptr = new FlowBased(internalS);
    }
    else if (formulation == kRootedBasedFormulation) {
        ptr = new RootedBased(internalS);
    }
    else {
        throw formulationNotFound();
    }*/
    return ptr;
}