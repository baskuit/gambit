//
// FILE: gnumber.h -- Number class for Gambit project. 
//
// $Id$
//

#ifndef GNUMBER_H
#define GNUMBER_H

#include "gmisc.h"
#include "rational.h"
#include <math.h>

class gOutput;

class gNumber  {
protected:
  Precision rep;
  gRational *rval;
  double dval;

public:
  class DivideByZero : public gException  {
  public:
    DivideByZero(int, char *);

    virtual ~DivideByZero()  { }
    gText Description(void) const;
  };

  gNumber(void);
  gNumber(double);
  gNumber(int n);
  gNumber(long n);
  gNumber(const gInteger& y);
  gNumber(const gRational& y);
  gNumber(const gNumber& y);

  ~gNumber();

  gNumber &operator =  (const gNumber& y);

  friend bool      operator == (const gNumber& x, const gNumber& y);
  friend bool      operator != (const gNumber& x, const gNumber& y);
  friend bool      operator <  (const gNumber& x, const gNumber& y);
  friend bool      operator <= (const gNumber& x, const gNumber& y);
  friend bool      operator >  (const gNumber& x, const gNumber& y);
  friend bool      operator >= (const gNumber& x, const gNumber& y);

  friend gNumber   operator +  (const gNumber& x, const gNumber& y);
  friend gNumber   operator -  (const gNumber& x, const gNumber& y);
  friend gNumber   operator *  (const gNumber& x, const gNumber& y);
  friend gNumber   operator /  (const gNumber& x, const gNumber& y);

  gNumber &        operator += (const gNumber& y);
  gNumber &        operator -= (const gNumber& y);
  gNumber &        operator *= (const gNumber& y);
  gNumber &        operator /= (const gNumber& y);

  friend gNumber  operator - (const gNumber& x);
  friend gNumber pow(const gNumber&,long);

  friend gOutput&    operator << (gOutput& s, const gNumber& y);
  friend gInput&     operator >> (gInput& s, gNumber& y);

  operator double() const;
  operator gRational() const;
  Precision GetPrecision(void) const;

  bool IsInteger(void) const;
};

gOutput &operator<<(gOutput &, const gNumber &);

#endif 
