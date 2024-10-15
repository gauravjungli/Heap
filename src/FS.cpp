#include "gauravlib.h"

FS FS::operator+ (FS w){
    FS temp;
    temp.p=this->p+w.p;
    temp.q=this->q+w.q;
    return temp;
  }

  FS FS::operator- (FS w){
    FS temp;
    temp.p=this->p-w.p;
    temp.q=this->q-w.q;
    return temp;
  }

  FS FS::operator* (double w){
    FS temp;
    temp.p=w*this->p;
    temp.q=w*this->q;
    return temp;
  }
  FS FS::operator/ (double w){
    FS temp;
    temp.p=this->p/w;
    temp.q=this->q/w;
    return temp;
  }
  FS::FS(CV w)
  {
	this->p=w.p; this->q=w.q; 
  }
