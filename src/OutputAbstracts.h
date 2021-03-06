/*******************************************************************************
GPU OPTIMIZED MONTE CARLO (GOMC) 2.20
Copyright (C) 2018  GOMC Group
A copy of the GNU General Public License can be found in the COPYRIGHT.txt
along with this program, also can be found at <http://www.gnu.org/licenses/>.
********************************************************************************/
#ifndef OUTPUT_ABSTRACTS_H
#define OUTPUT_ABSTRACTS_H

#include <string>

#include "BasicTypes.h" //For ulong

#include "EnsemblePreprocessor.h" //For BOX_TOTAL, etc.
#include "StaticVals.h"
#include "ConfigSetup.h" //For enables, etc.
#include "PDBSetup.h" //For atoms class

class OutputVars;
class System;

////////////////////////////
///  WRITE
//

class OutputableBase
{
public:
  virtual void Init(pdb_setup::Atoms const& atoms,
                    config_setup::Output const& output) = 0;

  virtual void DoOutput(const ulong step) = 0;

  virtual void Sample(const ulong step) = 0;

  virtual void Output(const ulong step)
  {
    if (!enableOut) {
      return;
    } else {
      Sample(step);
    }
    if ((step + 1) % stepsPerOut == 0) {
      DoOutput(step);
      firstPrint = false;
    }
  }

  void Init(pdb_setup::Atoms const& atoms,
            config_setup::Output const& output,
            const ulong tillEquil,
            const ulong totSteps)
  {
    Init(tillEquil, totSteps, output.statistics.settings.uniqueStr.val);
    Init(atoms, output);
  }

  void Init(const ulong tillEquil, const ulong totSteps,
            std::string const& uniqueForFileIO)
  {
    uniqueName = uniqueForFileIO;
    stepsTillEquil = tillEquil;
    totSimSteps = totSteps;
    firstPrint = true;
  }

//private:
  std::string uniqueName;
  ulong stepsPerOut, stepsTillEquil, totSimSteps;
  bool enableOut, firstPrint;

  //Contains references to various objects.
  OutputVars * var;
};

#endif /*OUTPUT_ABSTRACTS_H*/
