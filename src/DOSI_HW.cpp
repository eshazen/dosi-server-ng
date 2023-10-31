// DOSI_HW.cpp - hardware interface
//
// these are placeholders

#include "DOSI_HW.hh"
#include "DebugLog.hh"

DOSI_HW::DOSI_HW() {
  DebugLog::log( LOG_INFO, "DOSI_HW::DOSI_NW()\n");
};

DOSI_HW::~DOSI_HW() {
  DebugLog::log( LOG_INFO, "DOSI_HW::~DOSI_NW()\n");
};

void DOSI_HW::Initialize() {
  DebugLog::log( LOG_INFO, "DOSI_HW::Initialize()\n");
};

void DOSI_HW::selectAPD( int apd_no) {
  DebugLog::log( LOG_INFO, "DOSI_HW::selectAPD( %d)\n", apd_no);
};

void DOSI_HW::setSpectro( int spec_state) {
  DebugLog::log( LOG_INFO, "DOSI_HW::setSpectro( %d)\n", spec_state);
};

void DOSI_HW::enableActiveLasers( bool* which) {
  char* laserz = (char *)"000000";
  for( int i=0; i<6; i++)
    if( which[i]) laserz[5-i]='1';
  DebugLog::log( LOG_INFO, "DOSI_HW::enableActiveLasers( %s)\n", laserz);
};

void DOSI_HW::disableAllLasers() {
  DebugLog::log( LOG_INFO, "DOSI_HW::disableAllLasers()\n");
};

void DOSI_HW::runDOSI( std::complex<double> *sysResp) {
  DebugLog::log( LOG_INFO, "DOSI_HW::runDOSI( %p)\n", sysResp);
};
