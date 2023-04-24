//
// Created by weldeab solomon on 12/03/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_TO_PAIRING_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_TO_PAIRING_H




#include "base_algorithm.h"
#include "pairing_algo_variants.h"

// a base class for two stage algorithms that construct a solution by invoking one of Multi-bin Pairing
// algorithms first , followed by one of Multi-bin algorithms.


class PairingToPairing : public BaseAlgorithm
{
public:
    PairingToPairing(std::string algo_name, const Instance &instance);

    virtual unsigned long solveInstWithIC(int hint_nb_bins = 0);
    virtual bool tryToSolve(int m_bins);

    // This method generates a list of breakpoints and calls the trySolveTwoStage algorithm  for every breakpoint.
    // it keeps track of the best solution
    // it returns the number of bins in the best solution

    virtual int solveInstWith2SA(float b_factor, int LB, int UB);

    // returns the number of bins used
    // search for a feasible solution between the LB and UP via binary search.
    virtual int solveInstWithMBP(int breakpoint, int LB, int UB);
protected:
    // given a breakpoint and m number of bins, this method tries to solve the instance
    // the paramater m_bins is changed with every call to trySolveTwoStage
    virtual bool trySolveTwoStage(int breakpoint, int m_bins);

    //  uses multi bin pairing algorithm to pack items until the breakpoint is reached
    bool solve_First_Pairing(int breakpoint,int m_bins, std::vector<Item*>::iterator start_it, std::vector<Item*>::iterator end_it);

    //  uses multi-bin pairing algorithm to pack the rest items
    bool solve_Second_Pairing(int breakpoint, int m_bins, std::vector<Item*>::iterator last_it, std::vector<Item*>::iterator end_it, SCORE score);

    void updateScores(Bin* bin, std::vector<Item*>::iterator first_item, std::vector<Item*>::iterator end_it, SCORE score);

    float computeItemBinScore(Item* item, Bin* bin, SCORE score);

    void updateBestSolutionBins(const std::vector<Bin*>& activated_bins);





protected:

    SCORE m_score_first;
    SCORE m_score_second;
    int bin_increment;
    int limit_increment;

    std::vector<Bin*> m_best_solution_bins;
    std::vector<std::vector<float>> m_bin_item_scores;





};


/* ================================================ */
/* ================================================ */


class Pairing_P_To_P : public PairingToPairing
{
public:
    Pairing_P_To_P(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_P_To_S : public PairingToPairing
{
public:
    Pairing_P_To_S(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_P_To_T : public PairingToPairing
{
public:
    Pairing_P_To_T (std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_S_To_P : public PairingToPairing
{
public:
    Pairing_S_To_P(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_S_To_S : public PairingToPairing
{
public:
    Pairing_S_To_S(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_S_To_T : public PairingToPairing
{
public:
    Pairing_S_To_T (std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_T_To_P : public PairingToPairing
{
public:
    Pairing_T_To_P(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_T_To_S : public PairingToPairing
{
public:
    Pairing_T_To_S(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_T_To_T : public PairingToPairing
{
public:
    Pairing_T_To_T (std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */



#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_TO_PAIRING_H
