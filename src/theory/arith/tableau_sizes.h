/*********************                                                        */
/*! \file tableau_sizes.h
 ** \verbatim
 ** Original author: Tim King
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2014  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/


#include "cvc4_private.h"

#pragma once

#include <stdint.h>
#include "theory/arith/arithvar.h"

namespace CVC4 {
namespace theory {
namespace arith {

class Tableau;

class TableauSizes {
private:
  const Tableau* d_tab;
public:
  TableauSizes(const Tableau* tab): d_tab(tab){}

  uint32_t getRowLength(ArithVar b) const;
  uint32_t getColumnLength(ArithVar x) const;
}; /* TableauSizes */

}/* CVC4::theory::arith namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */

