//
// FILE: seqform.h -- Interface to Sequence Form solution module
//
// $Id$
//

#ifndef SEQFORM_H
#define SEQFORM_H

#include "glist.h"
#include "gstatus.h"
#include "behavsol.h"

class SeqFormParams     {
  public:
    int trace, stopAfter, maxdepth;
    gPrecision precision;
    gOutput *tracefile;
    gStatus &status;
    
    SeqFormParams(gStatus &status_ = gstatus);
};

#include "subsolve.h"

class efgLcpSolve : public SubgameSolver  {
private:
  int npivots;
  SeqFormParams params;
  gArray<gNumber> values;

  void SolveSubgame(const Efg &, const EFSupport &, gList<BehavSolution> &);
  EfgAlgType AlgorithmID(void) const { return algorithmEfg_LCP_EFG; }    

public:
  efgLcpSolve(const EFSupport &, const SeqFormParams &, int max = 0);
  virtual ~efgLcpSolve();
  
  int NumPivots(void) const  { return npivots; }
};

#endif    // SEQFORM_H





