#include <iostream>
#include "include/FFD_algorithm.h"

int main() {

    std::string instance_name = "instance";
    std::string instance_file = "instance.vbp";
    bool shuffle_items = true;
    Instance inst(instance_name, instance_file, shuffle_items);
    BaseAlgorithm *base_algorithm = new FFD_algorithm("FFD_algorithm", inst);
    int solution = base_algorithm->solveInstance();
    delete base_algorithm;
    std::cout<<solution<<std::endl;
    return 0;
}
