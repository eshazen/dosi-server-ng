#ifndef DOSI_HW_HH
#define DOSI_HW_HH

//
// DOSI_HW - control the hardware
//
// dummy class - replace with Lina's
//

class DOSI_HW {

public:
  DOSI_HW();
  ~DOSI_HW();
  void selectAPD( int apd_no);
  void setSpectro( int spec_state);
  void enableActiveLasers( bool* which);
  void disableAllLasers();
  void runDOSI( std::complex<double> *sysResp);
};


#endif
