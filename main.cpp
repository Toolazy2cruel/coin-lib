// Example of using COIN-OR OSI, building the instance internally
// with sparse matrix object

#include <iostream>
#include "OsiClpSolverInterface.hpp"
#include "CoinPackedMatrix.hpp"
#include "CoinPackedVector.hpp"
#include "symphony.h"
#include "OsiSymSolverInterface.hpp"
#include <sys/malloc.h>


/*
 SYMPHONY is an open-source generic MILP solver, callable library, and
extensible framework for implementing customized solvers for mixed-integer
linear programs (MILPs). SYMPHONY can be built in various sequential and
parallel configurations for either distributed or shared memory architectures
and can be used "out of the box" as a solver for generic mixed-integer
linear programs or customized through a wide variety of user callback
functions and control parameters. SYMPHONY has a number of advanced
capabilities stemming from the research projects discussed above, including
the ability to solve multi-objective MILPs, the ability to warm start its
solution procedure, and the ability to perform basic sensitivity analyses.
SYMPHONY has has been deployed in a variety of application areas, including
computational biology, wireless telecommunications, supply chain management,
transportation services, and air transportation.
*/

//假设有10个dt对

void example() {
    // Create a problem pointer.  We use the base class here.
    OsiSymSolverInterface *si;

    // When we instantiate the object, we need a specific derived class.
    si = new OsiSymSolverInterface;
    //可以限定求解时间

    // Build our own instance from scratch

    /*
     * This section adapted from Matt Galati's example
     * on the COIN-OR Tutorial website.
     *
     * Problem from Bertsimas, Tsitsiklis page 21
     *
     *  optimal solution: x* = (1,1)
     *
     *  minimize -1 x0 - 1 x1
     *  s.t       1 x0 + 2 x1 <= 3
     *            2 x0 + 1 x1 <= 3
     *              x0        >= 0
     *              x1        >= 0
     */

    int n_cols = 2;
    double *objective    = new double[n_cols];//the objective coefficients
    double *col_lb       = new double[n_cols];//the column lower bounds
    double *col_ub       = new double[n_cols];//the column upper bounds

    //Define the objective coefficients.
    //minimize -1 x0 - 1 x1
    objective[0] = -1.0;
    objective[1] = -1.0;

    //Define the variable lower/upper bounds.
    // x0 >= 0   =>  0 <= x0 <= infinity
    // x1 >= 0   =>  0 <= x1 <= infinity
    col_lb[0] = 0.0;
    col_lb[1] = 0.0;
    col_ub[0] = si->getInfinity();
    col_ub[1] = si->getInfinity();

    int n_rows = 2;
    double *row_lb = new double[n_rows]; //the row lower bounds
    double *row_ub = new double[n_rows]; //the row upper bounds

    //Define the constraint matrix.
    CoinPackedMatrix *matrix =  new CoinPackedMatrix(false,0,0);
    matrix->setDimensions(0, n_cols);

    //1 x0 + 2 x1 <= 3  =>  -infinity <= 1 x0 + 2 x1 <= 3
    CoinPackedVector row1;
    row1.insert(0, 1.0);
    row1.insert(1, 2.0);
    row_lb[0] = -1.0 * si->getInfinity();
    row_ub[0] = 3.0;
    matrix->appendRow(row1);

    //2 x0 + 1 x1 <= 3  =>  -infinity <= 2 x0 + 1 x1 <= 3
    CoinPackedVector row2;
    row2.insert(0, 2.0);
    row2.insert(1, 1.0);
    row_lb[1] = -1.0 * si->getInfinity();
    row_ub[1] = 3.0;
    matrix->appendRow(row2);

    //load the problem to OSI
    si->loadProblem(*matrix, col_lb, col_ub, objective, row_lb, row_ub);

    //write the MPS file to a file called example.mps
    si->writeMps("example");



    // Solve the (relaxation of the) problem
    si->initialSolve();

    // Check the solution
    if ( si->isProvenOptimal() ) {
        std::cout << "Found optimal solution!" << std::endl;
        std::cout << "Objective value is " << si->getObjValue() << std::endl;

        int n = si->getNumCols();
        const double* solution = si->getColSolution();

        // We can then print the solution or could examine it.
        for( int i = 0; i < n; ++i )
            std::cout << si->getColName(i) << " = " << solution[i] << std::endl;

    } else {
        std::cout << "Didn't find optimal solution." << std::endl;
        // Could then check other status functions.
    }

    delete[] row_ub;
    delete[] row_lb;
    delete[] objective;
    delete[] col_ub;
    delete[] col_lb;

}

int main(void)
{
    example();
    return 0;
}
