
DOSI_HW::DOSI_HW() { };

DOSI_HW::~DOSI_HW() { };

void DOSI_HW::selectAPD( int apd_no) { };

void DOSI_HW::setSpectro( int spec_state) { };

void DOSI_HW::enableActiveLasers( bool* which) { };

void DOSI_HW::disableAllLasers() { };

void DOSI_HW::runDOSI( std::complex<double> *sysResp) { };
