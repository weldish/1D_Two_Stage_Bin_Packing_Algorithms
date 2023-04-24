
#include "include/utils.h"
#include "include/instance_name_generators.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <stdexcept>
#include <math.h>

using namespace std;
using namespace onedpacking;
using namespace std::chrono;

/*
 * Simple program to solve the one-dimensional bin packing problem
 * with the provided algorithms
 * Inputs:
 *  - the instance file (.vbp format)
 *  - the name of the packing algorithm to run
 * Output:
 *  - prints the solution (number of bins)
 */


struct stat{
    int nb_of_bins_required;
    float running_time;
    int nb_of_items;
    int bin_capacity;
    int lower_bound;
    string class_name;
};


struct class_Average{
    string algo_name;  // for_example = BFD_algorithm
    string class_name; // for_example = binpack1
    int nb_of_instances; // for_example = 20
    int nb_of_items;
    int bin_capacity;
    float average_percent_error;
    float average_run_time;

};


ofstream myfile;

stat solveCurrentInstance (string gen_instance_file, string algorithm_name);
void write_to_csv_file(const class_Average& class_avg);
void compute_average(const vector<stat>& stats, const string& instance_name, string algo_name);

void run_algos(string algorithm_name);

int main(int argc, char** argv)
{

    vector<std::string> algo_names =
            {
                    "FFD_algorithm",
                    "BFD_algorithm",
                    "FFD_Pairing_PRODUCT",
                    "BFD_Pairing_PRODUCT",
                    "WFD_Pairing_PRODUCT",
                    "FFD_Pairing_SLACK",
                    "BFD_Pairing_SLACK",
                    "WFD_Pairing_SLACK",

                    "Pairing_PRODUCT_To_FFD",
                    "Pairing_PRODUCT_To_BFD",
                    "Pairing_PRODUCT_To_WFD",
                    "Pairing_SLACK_To_FFD",
                    "Pairing_SLACK_To_BFD",
                    "Pairing_SLACK_To_WFD",

                    "Pairing_P_To_S",
                    "Pairing_S_To_P"

            };

    string algorithm_name;
    for (int i = 0; i < 16; i++)
    {
        algorithm_name = algo_names[i];
        run_algos(algorithm_name);
    }

    return 0;
}


stat solveCurrentInstance (string gen_instance_file, string algorithm_name)
{
    string instance_file(gen_instance_file);
    size_t last_index = instance_file.find_first_of("0123456789");
    string class_name = instance_file.substr(0, last_index + 1);
    string instance_name = instance_file.substr(instance_file.find_last_of("/\\") + 1);

    string algo_name(algorithm_name);

    /* ================================================ */
    /* ================================================ */

    bool shuffle_items = true;
    bool show_time = true;


    /* ================================================ */
    /* ================================================ */
    // creating an instance
    Instance inst(instance_name, instance_file, shuffle_items);

    time_point<high_resolution_clock> start;
    time_point<high_resolution_clock> stop;

    /* ================================================ */
    /* ================================================ */

    // figure out what class of algorithm it is

    BaseAlgorithm * algo;
    string type_algo = algo_name.substr(0, 5);
    bool is_multibin = false;
    bool is_two_stage = false;
    int solution;
    stat stats;  // stores time and number of bins required to solve the instance

    if ((type_algo == "S_Pai") || (type_algo == "WFDm_"))
    {
        //algo = createSingleMultiBinAlgo(algo_name, inst, score);
        is_multibin = true;
        is_two_stage = false;
    }

    else if ((type_algo == "FFD_P") || (type_algo == "BFD_P") || (type_algo == "WFD_P") || (type_algo == "Pairi"))
    {
        algo = createTwoStageMultiBinAlgo(algo_name, inst );
        is_multibin = true;
        is_two_stage = true;
    }
    else
    {
        algo = createItemCentricAlgo(algo_name, inst);
    }

    /* ================================================ */
    /* ================================================ */

    // get the solution by calling the methods that solve the instance
    // if it is item-centric algorithm
    if (!is_multibin)
    {
        // compute the lower bound
        int lower_bound = LB(inst);
        // creating an object of an algorithm of some type.
        algo = createItemCentricAlgo(algo_name, inst);

        // using the created object to generate a call on it.
        start = high_resolution_clock::now();
        solution = algo->solveInstWithIC();
        stop = high_resolution_clock::now();

        // save results after solving an instance
        stats.nb_of_items = algo->getNumOfItems();
        stats.bin_capacity = algo->getBinCapacity();
        stats.nb_of_bins_required = solution;
        stats.lower_bound = lower_bound;
        stats.class_name = class_name;
    }
    // if it is a mutlibin algorithm but single stage
    if (is_multibin and !is_two_stage)
    {
        // compute the lower bound
        int lower_bound = LB(inst);
        // compute the upper bound as one you would get from FF algorithm
        BaseAlgorithm *FF_algo = createItemCentricAlgo("FF_algorithm", inst);
        int upper_bound= FF_algo->solveInstWithIC();

        // create the object that solves the instance
        algo = createSingleMultiBinAlgo(algo_name, inst);

        // solve instance
        start = high_resolution_clock::now();
        solution = algo->solveInstWithMBP(lower_bound, upper_bound);
        stop = high_resolution_clock::now();
        // free FF_algo
        delete FF_algo;

        if (solution == -1)
        {
            // save results after solving an instance
            stats.nb_of_bins_required = 0;
            stats.nb_of_items = algo->getNumOfItems();
            stats.bin_capacity = algo->getBinCapacity();
            stats.lower_bound = 0;
            stats.class_name = class_name;


        }
        else
        {
            // save results after solving an instance
            stats.nb_of_bins_required = solution;
            stats.nb_of_items = algo->getNumOfItems();
            stats.bin_capacity = algo->getBinCapacity();
            stats.lower_bound = lower_bound;
            stats.class_name = class_name;
        }

    }
    // if it is a two stage algorithm

    if (is_multibin and is_two_stage)
    {
        // set the b_factor and compute the lowerbound
        int lower_bound = LB(inst);
        float b_factor = 0.13; // equals 8 breakpoints
        // compute the upper bound as one you would get from FF algorithm
        BaseAlgorithm *FF_algo = createItemCentricAlgo("FF_algorithm", inst);
        int upper_bound= FF_algo->solveInstWithIC();

        // create the object that solves the instance
        algo = createTwoStageMultiBinAlgo(algo_name, inst);

        // solve instance
        start = high_resolution_clock::now();
        solution = algo->solveInstWith2SA(b_factor, lower_bound, upper_bound);
        stop = high_resolution_clock::now();

        // free FF_algo
        delete FF_algo;

        if (solution == -1)
        {
            // save results after solving an instance
            stats.nb_of_bins_required = 0;
            stats.nb_of_items = algo->getNumOfItems();
            stats.bin_capacity = algo->getBinCapacity();
            stats.lower_bound = 0;
            stats.class_name = class_name;
        }
        else
        {
            // save results after solving an instance
            stats.nb_of_bins_required = solution;
            stats.nb_of_items = algo->getNumOfItems();
            stats.bin_capacity = algo->getBinCapacity();
            stats.lower_bound = lower_bound;
            stats.class_name = class_name;

        }

    }

    /* ================================================ */
    /* ================================================ */


    string s = algo_name + ": " + to_string(solution) + " bins";

    if (show_time)
    {
        float dur1 = (duration_cast<milliseconds>(stop - start)).count();
        float dur = (float)dur1 / 1000;


        s+= " in " + to_string(dur) + " seconds";

        stats.running_time = dur;

    }


    std::cout << s << std::endl; // Print solution to stdout


    delete algo;

    return stats;
}


void write_to_csv_file(const class_Average& class_avg)
{


    myfile << class_avg.class_name << "," << class_avg.average_percent_error << "," << class_avg.average_run_time << "," << class_avg.nb_of_items << "," << class_avg.bin_capacity<<"\n";


    myfile.flush();


}


void compute_average(const vector<stat>& stats, const string& instance_name, string algo_name)
{

    // average is computed over 20 problem instances whose number of items and bin sizes is the same. they only differ in their item sizes.


    const int nb_of_insts_for_every_class = 20;
    const int nb_of_classes = 8;

    vector<class_Average> average_perc_error(nb_of_classes);
    vector<class_Average> average_run_time(nb_of_classes);

    // total number of soltions;
    int total_stats = int(stats.size());
    // a stat/solution for every instance
    int stat_index = 0;

    for (int i = 0; i < nb_of_classes; i++)
    {

        float sum_errors = 0.0;
        float sum_runtime = 0.0;
        struct class_Average class_avg;
        int bin_capacity = 0;
        int nb_of_items = 0;
        float per_avg_err = 0.0;
        float avg_runtime = 0.0;
        string class_name;

        for (int j = 0; j < nb_of_insts_for_every_class; j++)
        {

            if(stat_index == total_stats)
            {
                break;
            }
            struct stat st = stats[stat_index];

            // sum up the solutions errors

            int lb = st.lower_bound;
            int nb_of_bins = st.nb_of_bins_required;
            float run_time = st.running_time;

            bin_capacity = st.bin_capacity;
            nb_of_items = st.nb_of_items;
            class_name = st.class_name;

            // solution error = (number of bins required - lowerbound) / lowerbond
            float error;

            if (nb_of_bins == 0 and lb == 0)
            {
                error = 0.0;

            }
            else
            {
                error = (nb_of_bins - lb)/(float)lb;

            }

            sum_errors = sum_errors + error;

            // sum runtimes in miliseconds over 20 instances
            sum_runtime = sum_runtime + run_time;

            stat_index++;
        }
        // compute an average percentage error over 20 problem instances with repect to the lower bound

        per_avg_err = (sum_errors/float(nb_of_insts_for_every_class))*100;
        // rounded to two decimal places
        float per_avg_err_rounded = roundf(per_avg_err * 100) / 100;

        // average run time over certain number of instance problems.
        avg_runtime = (sum_runtime)/float(nb_of_insts_for_every_class);

        // add it to the vector

        class_avg.average_percent_error = per_avg_err_rounded;

        class_avg.algo_name = algo_name;
        class_avg.bin_capacity = bin_capacity;
        class_avg.nb_of_items = nb_of_items;
        class_avg.average_run_time = avg_runtime;
        class_avg.class_name = class_name;
        class_avg.nb_of_instances = nb_of_insts_for_every_class;

        // add it to a list of class_averges
        average_perc_error.push_back(class_avg);

        // write every average to a file
        write_to_csv_file(class_avg);

    }


}



void run_algos(string algorithm_name)
{

    // generates a list of instance names for bpp_falkenauer
    vector<string> instance_file_names = generate_insts_For_bpp_falkenauer();


    // vector<string> instance_file_names = generate_insts_For_shewrem();
    /*====================================================================*/
    /*====================================================================*/



    vector<string>::iterator first_it = instance_file_names.begin();
    vector<string>::iterator end_it  = instance_file_names.end();




    string instance_file;
    string path = algorithm_name + ".csv";
    stat stats;
    /* ====================================================*/
    /* ====================================================*/
    // open the file to which you write the results
    myfile.open(path, ios::trunc);
    if (!myfile.is_open())
    {

        string s("Cannot write file to instance.csv");
        throw std::runtime_error(s);
    }
    myfile << algorithm_name << "\n";
    myfile << "Class_name, Average_percent_error, Average_run_time(s) , Nb_of_Items, Bin_Capacity " << "\n";
    myfile.flush();

    /* ====================================================*/
    /* ====================================================*/

    vector<stat> list_of_solutions;

    while (first_it < end_it)
    {
        instance_file = *first_it;

        // we are feeding this method  instances one by one
        stats = solveCurrentInstance (instance_file, algorithm_name);
        // record resutls for a given instance into a csv file

        // add the solution to list of solutions

        list_of_solutions.push_back(stats);

        //write_to_csv_file(stats, instance_file);

        first_it++;
    }

    // call average_compute


    compute_average(list_of_solutions, instance_file, algorithm_name );

    myfile.close();


}
