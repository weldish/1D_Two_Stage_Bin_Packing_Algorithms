//
// Created by weldeab solomon on 11/04/2023.
//

#include "../include/utils.h"


#include <sstream>
#include <stdexcept> // For throwing stuff


// a method for calling any item-centric algorithms
BaseAlgorithm* createItemCentricAlgo(std::string algo_name, const Instance &instance)
{
//    std::vector<std::string> v = splitString(algo_name);
//    const std::string& s = v[0];

    try {
        // Item-centric algos
        if (algo_name == "FFD_algorithm")
        {
            return new FFD_algorithm(algo_name, instance);
        }
        else if (algo_name == "BFD_algorithm")
        {
            return new BFD_algorithm(algo_name, instance);
        }
        else if (algo_name == "WFD_algorithm")
        {
            return new WFD_algorithm(algo_name, instance);
        }

    } // end try
    catch (...)
    {
        std::string err = "Incorrect measure or weight name. Could not create algorithm \'" + algo_name + "\'";
        throw std::runtime_error(err);
    }

    std::string err_string = "Unknown algorithm name \'" + algo_name + "\'";
    throw std::runtime_error(err_string);

}


// a  method for calling single stage multibin algorithms
BaseAlgorithm * createSingleMultiBinAlgo(std::string algo_name, const Instance &instance)
{

    try
    {

        if (algo_name == "S_Pairing_PRODUCT")
        {
            return new S_Pairing_PRODUCT(algo_name, instance);
        }
        else if (algo_name == "S_Pairing_SLACK")
        {
            return new S_Pairing_SLACK(algo_name, instance);
        }
        else if (algo_name == "S_Pairing_TIGHT_FILL")
        {
            return new S_Pairing_TIGHT_FILL(algo_name, instance);
        }
    }
    catch(...)
    {
        std::string err = "Incorrect measure or weight name. Could not create algorithm \'" + algo_name + "\'";
        throw std::runtime_error(err);

    }

    std::string err_string = "Unknown algorithm name \'" + algo_name + "\'";
    throw std::runtime_error(err_string);

}


// a  method for calling a two-stage algorithms

BaseAlgorithm* createTwoStageMultiBinAlgo(std::string algo_name, const Instance &instance)
{

    std::string type_algo = algo_name.substr(0, 5);

    try
    {
        if (type_algo == "FFD_P")
        {
            if (algo_name == "FFD_Pairing_PRODUCT")
            {
                return new FFD_Pairing_PRODUCT(algo_name, instance);
            }
            else if (algo_name == "FFD_Pairing_SLACK")
            {
                return new FFD_Pairing_SLACK(algo_name, instance);
            }
            else if (algo_name == "FFD_Pairing_TIGHT_FILL")
            {
                return new FFD_Pairing_TIGHT_FILL(algo_name, instance);
            }
        }

        else if (type_algo == "BFD_P")
        {
            if (algo_name == "BFD_Pairing_PRODUCT")
            {
                return new BFD_Pairing_PRODUCT(algo_name, instance);
            }
            else if (algo_name == "BFD_Pairing_SLACK")
            {
                return new BFD_Pairing_SLACK(algo_name, instance);
            }
            else if (algo_name == "BFD_Pairing_TIGHT_FILL")
            {
                return new BFD_Pairing_TIGHT_FILL(algo_name, instance);
            }

        }

        else if (type_algo == "WFD_P")
        {
            if (algo_name == "WFD_Pairing_PRODUCT")
            {
                return new WFD_Pairing_PRODUCT(algo_name, instance);
            }
            else if (algo_name == "WFD_Pairing_SLACK")
            {
                return new WFD_Pairing_SLACK(algo_name, instance);
            }
            else if (algo_name == "WFD_Pairing_TIGHT_FILL")
            {
                return new WFD_Pairing_TIGHT_FILL(algo_name, instance);
            }
        }

        else if (type_algo == "Pairi")
        {

            std::string type_algorithm = algo_name.substr(0, 10);
            if (type_algorithm == "Pairing_PR")
            {
                if (algo_name == "Pairing_PRODUCT_To_FFD")
                {
                    return new Pairing_PRODUCT_To_FFD(algo_name, instance);
                }
                else if (algo_name == "Pairing_PRODUCT_To_BFD")
                {
                    return new Pairing_PRODUCT_To_BFD(algo_name, instance);
                }
                else if (algo_name == "Pairing_PRODUCT_To_WFD")
                {
                    return new Pairing_PRODUCT_To_WFD(algo_name, instance);
                }
            }
            else if (type_algorithm == "Pairing_SL")
            {
                if (algo_name == "Pairing_SLACK_To_FFD")
                {
                    return new Pairing_SLACK_To_FFD(algo_name, instance);
                }
                else if (algo_name == "Pairing_SLACK_To_BFD")
                {
                    return new Pairing_SLACK_To_BFD(algo_name, instance);
                }
                else if (algo_name == "Pairing_SLACK_To_WFD")
                {
                    return new Pairing_SLACK_To_WFD(algo_name, instance);
                }

            }
            else if (type_algorithm == "Pairing_TI")
            {
                if (algo_name == "Pairing_TIGHT_FILL_To_FFD")
                {
                    return new Pairing_TIGHT_FILL_To_FFD(algo_name, instance);
                }
                else if (algo_name == "Pairing_TIGHT_FILL_To_BFD")
                {
                    return new Pairing_TIGHT_FILL_To_BFD(algo_name, instance);
                }
                else if (algo_name == "Pairing_TIGHT_FILL_To_WFD")
                {
                    return new Pairing_TIGHT_FILL_To_WFD(algo_name, instance);
                }
            }
            else if (type_algorithm == "Pairing_P_")
            {
                if (algo_name == "Pairing_P_To_P")
                {
                    return new Pairing_P_To_P(algo_name, instance);
                }
                else if (algo_name == "Pairing_P_To_S")
                {
                    return new Pairing_P_To_S(algo_name, instance);
                }
                else if (algo_name == "Pairing_P_To_T")
                {
                    return new Pairing_P_To_T(algo_name, instance);
                }
            }
            else if(type_algorithm == "Pairing_S_")
            {
                if (algo_name == "Pairing_S_To_P")
                {
                    return new Pairing_S_To_P(algo_name, instance);
                }
                else if (algo_name == "Pairing_S_To_S")
                {
                    return new Pairing_S_To_S(algo_name, instance);
                }
                else if (algo_name == "Pairing_S_To_T")
                {
                    return new Pairing_S_To_T(algo_name, instance);
                }

            }
            else if(type_algorithm == "Pairing_T_")
            {
                if (algo_name == "Pairing_T_To_P")
                {
                    return new Pairing_T_To_P(algo_name, instance);
                }
                else if (algo_name == "Pairing_T_To_S")
                {
                    return new Pairing_T_To_S(algo_name, instance);
                }
                else if (algo_name == "Pairing_T_To_T")
                {
                    return new Pairing_T_To_T(algo_name, instance);
                }
            }


        }

    }
    catch(...)
    {
        std::string err = "Could not create algorithm \'" + algo_name + "\'";
        throw std::runtime_error(err);

    }

    std::string err_string = "Unknown algorithm name \'" + algo_name + "\'";
    throw std::runtime_error(err_string);

}
