//#
//# FILE: gobit.cc -- Gobit module
//#
//# $Id$
//#

#ifdef __GNUG__
#pragma implementation "gobit.h"
#endif   // __GNUG__

#include "gambitio.h"
#include "normal.h"
#include "normiter.h"
#include "rational.h"
#include "gmatrix.h"
#include "gpvector.h"
#include "solution.h"
#include "gfunct.h"
#include "gobit.h"

GobitParams::GobitParams(void) 
  : plev(0), nequilib(1), minLam(.01), maxLam(30),delLam(1.01), 
    tolDFP(1.0e-10)
{ }

class BaseGobit {
public:
  virtual int Gobit(int) = 0;
  virtual int Nevals(void) = 0;
  virtual int Nits(void) = 0;
  virtual ~BaseGobit() {}
};

template <class T> class GobitModule
: public gBC2FunctMin<T>, public BaseGobit, public SolutionModule {
private:
  const NormalForm<T> &rep;
  T Lambda;
  gPVector<T> p;
  GobitParams params;
  int maxits;
  
public:
  GobitModule(const NormalForm<T> &N,gOutput &ofile,gOutput &efile,
	      const GobitParams &params)
    :  SolutionModule(ofile,efile,params.plev), rep(N), Lambda(0),
  p(rep.Dimensionality()), params(params),
  maxits(500), gBC2FunctMin<T>(N.ProfileLength()){ }
  virtual ~GobitModule() {}
  
  T GobitDerivValue(int i1, int j1, const gPVector<T> &p) const;
  T GobitValue(void) const;
  void get_grad(const gPVector<T> &p, gVector<T> &dp);
  
  T Value(const gVector<T> &x);
  int Deriv(const gVector<T> &p, gVector<T> &d);
  int Hess(const gVector<T> &p, gMatrix<T> &d) {return 1;}
  int Gobit(int);
  int Nevals(void) {return nevals;}
  int Nits(void) {return nits;}

};

template <class T> int GobitModule<T>::Gobit(int number)
{
  for(int i=1;i<=rep.NumPlayers();i++)
    for(int j=1;j<=rep.NumStrats(i);j++)
      p(i,j)=((T)(1)/(T)(rep.NumStrats(i)));
  
  int iter=0;
  
  Lambda=(T) (params.minLam);
  T value;
  
  int nit=0;
  while(nit < maxits && Lambda<=(T)(params.maxLam)) {
    nit++;
    DFP(p, (T)(params.tolDFP), iter, value);
    gout << "\nLam = " << Lambda << " nits= " << iter;
    gout << " val = " << value << " p = " << p;
    gout << " evals = " << Nevals();
    if(value>=10.0)return nit;
    Lambda = Lambda * ((T)(params.delLam));
  }
  return nit;
};

template <class T>
T GobitModule<T>::Value(const gVector<T> &v)
{
//  gout << " in GobitModule::Operator()";
  assert(v.Length()==p.Length());
  
  p = v;
  return GobitValue();
};

template <class T> int GobitModule<T>::
Deriv(const gVector<T> &v, gVector<T> &d)
{
  p=v;
  get_grad(p,d);
//  for(i=1;i<=rep.ProfileLength();i++)x[i]=dp[i];
//  gout << "\n in Deriv()";
//  gout << "\n p = " << p;
//  gout << "\n d = " << d;
  
  return 1;
};

template <class T> void GobitModule<T>::
get_grad(const gPVector<T> &p, gVector<T> &dp)
{
  int i1,j1,ii;
  T avg;
  
//  gout << "\n in get_grad()";
  
  for(i1=1,ii=1;i1<=rep.NumPlayers();i1++) {
    avg=(T)(0);
    for(j1=1;j1<=rep.NumStrats(i1);j1++) {
      dp[ii]=GobitDerivValue(i1,j1,p);
      avg+=dp[ii];
      ii++;
    }
    avg/=(T)rep.NumStrats(i1);
    
    ii-=rep.NumStrats(i1);
    for(j1=1;j1<=rep.NumStrats(i1);j1++) {
      dp[ii]-=avg;
      ii++;
    }
  }
}


#define BIG1 ((T) 100)
#define BIG2 ((T) 100)

template <class T> T GobitModule<T>::
GobitValue(void) const
{
  int i,j;
  T v,psum,z;
  
  v=(T)(0);
//  gout << "\nGobitValue(): p = " << p;
  for(i=1;i<=rep.NumPlayers();i++) {
//    psum=p(i,1);
    for(j=2;j<=rep.NumStrats(i);j++) {
      z=log(p(i,1))-log(p(i,j))
	- Lambda*(rep.Payoff(i,i,1,p)-rep.Payoff(i,i,j,p));
      v+=(z*z);
//      if(p(i,j)<(double)0.0)v+=(p(i,j)*p(i,j));
//      psum+=p(i,j);
    }
//    z=(T)(1)-psum;
//    v+=(z*z);
  }
  return v;
}

template <class T> T GobitModule<T>::
GobitDerivValue(int i1, int j1, const gPVector<T> &p) const
{
  int i, j;
  T x, x1,dv;
  
  x=(T)(0);
//  gout << "\nGobitDerivValue(): p = " << p;
  for(i=1;i<=rep.NumPlayers();i++) {
    for(j=2;j<=rep.NumStrats(i);j++) {
      dv=log(p(i,1))-log(p(i,j))
	- Lambda*(rep.Payoff(i,i,1,p)-rep.Payoff(i,i,j,p));
      if(i==i1) {
	if(j1==1)  x+=dv/p(i,1);
	if(j1==j)  x-=dv/p(i,j);
      }
      if(i!=i1)
	x-=dv*Lambda*(rep.Payoff(i,i,1,i1,j1,p)-rep.Payoff(i,i,j,i1,j1,p));
    }
  }
//  if(p(i1,j1)<(T)(0))x+=p(i1,j1);
  
  return ((T)(2))*x;
}


int GobitSolver::Gobit(void)
// int NormalForm::Gobit(int number,int plev, gOutput &out, gOutput &err,
//		      int &nevals, int &nits)
{
  BaseGobit *T;
  gOutput *outfile = &gout, *errfile = &gerr;
  
  if (params.outfile != "")
    outfile = new gFileOutput((char *) params.outfile);
  if (params.errfile != "" && params.errfile != params.outfile)
    errfile = new gFileOutput((char *) params.errfile);
  if (params.errfile != "" && params.errfile == params.outfile)
    errfile = outfile;
  
  
  switch (nf.Type())  {
  case DOUBLE:
    T = new GobitModule<double>((NormalForm<double> &) nf, *outfile,
				*errfile, params);
    break;
/*
 case RATIONAL:
    T = new GobitModule<Rational>((NFRep<Rational> &) *data, gout, gerr,0);
		break;
    */
  }
  T->Gobit(params.nequilib);
  
  if (params.outfile != "")
    delete outfile;
  if (params.errfile != "" && params.errfile != params.outfile)
		delete errfile;
  
  delete T;
  return 1;
}



