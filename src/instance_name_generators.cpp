//
// Created by weldeab solomon on 11/04/2023.
//

#include "include/instance_name_generators.h"


std::vector<std::string> generate_insts_For_bpp_falkenauer()
{
    string filename = "Binpack1.u120_01.vbp";
    string folder_name = "bpp_falkenauer";
    vector<string> instances_names_list;
    struct sameInstances{
        int g_num; // group number
        char bin_cap; // bin capacity
        int nb_items; // number of items
    };

    sameInstances groups[8] ={
            { 1, 'u', 120 },
            { 2, 'u', 250 },
            { 3, 'u', 500 },
            { 4, 'u', 1000 },
            { 5, 't', 60 },
            { 6, 't', 120 },
            { 7, 't', 249 },
            { 8, 't', 501 },
    };


    int instance_number = 0;
    int group_number = 0;


    while (group_number < 8)
    {
        if (group_number == 3 || group_number == 4)
        {
            group_number++;
            continue;
        }

        while (instance_number < 20)
        {
            string instance_num = "";
            if (instance_number < 10)
            {
                instance_num = to_string(0) + to_string(instance_number);
            }
            else
            {
                instance_num = to_string(instance_number);
            }


            string file_name = "Binpack" + to_string(groups[group_number].g_num)
                               + "." + (groups[group_number].bin_cap) +
                               to_string(groups[group_number].nb_items) + "_"+
                               instance_num + ".vbp";

            // add the generated filename to the list
            instances_names_list.push_back(file_name);

            instance_number++;

        }
        instance_number = 0;
        group_number++;

    }

    return instances_names_list;
}


vector<string> generate_insts_For_scholl() {
    vector<string> instance_names;
    for (int i = 1; i <5; i++) {
        for (int j = 1; j < 5; j++) {
            for (int k = 1; k < 4; k++) {
                for (int l = 0; l < 10; l++) {
                    string instance_name = "bin2data.N" + to_string(i) + "W" + to_string(j) + "B" + to_string(k) + "R" + to_string(l) + ".vbp";
                    instance_names.push_back(instance_name);
                }
            }
        }
    }
    return instance_names;
}



vector<string> generate_insts_For_hard28()
{
    vector<string> instance_names;
    for (int i = 1; i < 29; i++) {
        string instance_name = "hard_" + to_string(i) + ".vbp";
        instance_names.push_back(instance_name);
    }
    return instance_names;
}


vector<string> generate_insts_For_shewrem()
{
    vector<string> instance_names;

    for (int i = 1; i < 3; i++)
    {
        for(int j = 1; j < 101; j++)
        {
            string instance_name =  "SCH_WAE" + std::to_string(i) + ".BPP_" + std::to_string(j) + ".vbp";
            instance_names.push_back(instance_name);
        }
    }

    return instance_names;

}
