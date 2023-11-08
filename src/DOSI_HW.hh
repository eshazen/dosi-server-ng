#ifndef DOSI_HW_HH
#define DOSI_HW_HH

#include <complex>
#include "DebugLog.hh"
#include "adc.hh"
#include "dds_hl.hh"
#include "gpio.hh"


//
// DOSI_HW - control the hardware
//
// dummy class - replace with Lina's
//

class DOSI_HW : public ADC, public GPIO, public DDS_hl{

public:
  DOSI_HW();
  ~DOSI_HW();
  void Initialize();
  void selectAPD( int apd_no);
  void setSpectro( int spec_state);
  void enableActiveLasers( bool* which);
  void disableAllLasers();
  void runDOSI( std::complex<double> *sysResp);
};


#endif
