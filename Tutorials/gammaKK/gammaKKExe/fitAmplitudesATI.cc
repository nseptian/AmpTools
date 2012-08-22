#include <iostream>
#include <fstream>
#include <complex>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include "IUAmpTools/ConfigFileParser.h"
#include "IUAmpTools/ConfigurationInfo.h"
#include "IUAmpTools/AmpToolsInterface.h"
#include "gammaKKDataIO/gammaKKDataReader.h"
#include "gammaKKAmp/gammaKKHelicityAmp.h"
#include "gammaKKAmp/TwoPiAngles.h"
#include "gammaKKAmp/MultipoleAmps.h"

using std::complex;
using namespace std;
using namespace CLHEP;

int main( int argc, char* argv[] ){


    // ************************
    // usage
    // ************************

  cout << endl << " *** Performing the Fit *** " << endl << endl;

  if (argc <= 1){
    cout << "Usage:" << endl << endl;
    cout << "\tfitAmplitudes <config file name>" << endl << endl;
    return 0;
  }


    // ************************
    // parse the command line parameters
    // ************************

  string cfgname(argv[1]);

  cout << "Config file name = " << cfgname << endl << endl;


    // ************************
    // parse the config file
    // ************************

  ConfigFileParser parser(cfgname);
  ConfigurationInfo* cfgInfo = parser.getConfigurationInfo();
  cfgInfo->display();


    // ************************
    // AmpToolsInterface
    // ************************

  AmpToolsInterface::registerAmplitude(gammaKKHelicityAmp());
  AmpToolsInterface::registerAmplitude(TwoPiAngles());
  AmpToolsInterface::registerAmplitude(MultipoleAmps());

  AmpToolsInterface::registerDataReader(gammaKKDataReader());

  AmpToolsInterface ATI(cfgInfo);

  cout << "LIKELIHOOD BEFORE MINIMIZATION:  " << ATI.likelihood() << endl;

  MinuitMinimizationManager* fitManager = ATI.minuitMinimizationManager();
  fitManager->setPrecision(1E-13);
  fitManager->setStrategy(1);

  fitManager->migradMinimization();

  if( fitManager->status() != 0 && fitManager->eMatrixStatus() != 3 ){
    cout << "ERROR: fit failed..." << endl;
  }

  cout << "LIKELIHOOD AFTER MINIMIZATION:  " << ATI.likelihood() << endl;

  ATI.finalizeFit();

  return 0;

}


