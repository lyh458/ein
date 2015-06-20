#include "eePose.h"
#include <iostream>

using namespace std;

ostream & operator<<(ostream & os, const _eePose& toPrint)
{
  FileStorage st;
  st.open("tmp.yml", FileStorage::WRITE | FileStorage::MEMORY);
  st << "eePose"; 
  toPrint.writeToFileStorage(st);
  string result = st.releaseAndGetString();
  os << result.substr(10, result.size());
  return os;
} 

_eePose _eePose::zero() {
  _eePose zeroOut = {.px = 0.0, .py = 0.0, .pz = 0.0,
		     .qx = 0.0, .qy = 0.0, .qz = 0.0, .qw = 0.0};
  return zeroOut;
}

void _eePose::print(eePose toPrint) {
  cout << toPrint << endl;
}

double _eePose::squareDistance(eePose pose1, eePose pose2) {
  double dx = (pose1.px - pose2.px);
  double dy = (pose1.py - pose2.py);
  double dz = (pose1.pz - pose2.pz);
  double squareDistance = dx*dx + dy*dy + dz*dz;

  return squareDistance;
}

eePose _eePose::fromRectCentroid(Rect rect) {
  eePose result;
  result.px = rect.x + rect.width * 0.5;
  result.py = rect.y + rect.width * 0.5;
  return result;
}

_eePose _eePose::plusP(const Vector3d& a) const {
  _eePose toReturn;
  toReturn.px = px + a.x();
  toReturn.py = py + a.y();
  toReturn.pz = pz + a.z();
  toReturn.qx = qx;
  toReturn.qy = qy;
  toReturn.qz = qz;
  toReturn.qw = qw;
  return toReturn;
}

_eePose _eePose::minusP(const Vector3d& a) const {
  _eePose toReturn;
  toReturn.px = px - a.x();
  toReturn.py = py - a.y();
  toReturn.pz = pz - a.z();
  toReturn.qx = qx;
  toReturn.qy = qy;
  toReturn.qz = qz;
  toReturn.qw = qw;
  return toReturn;
}

_eePose _eePose::plusP(const _eePose& a) const {
  _eePose toReturn;
  toReturn.px = px + a.px;
  toReturn.py = py + a.py;
  toReturn.pz = pz + a.pz;
  toReturn.qx = qx;
  toReturn.qy = qy;
  toReturn.qz = qz;
  toReturn.qw = qw;
  return toReturn;
}

_eePose _eePose::minusP(const _eePose& a) const {
  _eePose toReturn;
  toReturn.px = px - a.px;
  toReturn.py = py - a.py;
  toReturn.pz = pz - a.pz;
  toReturn.qx = qx;
  toReturn.qy = qy;
  toReturn.qz = qz;
  toReturn.qw = qw;
  return toReturn;
}

_eePose _eePose::negativeP() const {
  _eePose toReturn;
  toReturn.px = -px;
  toReturn.py = -py;
  toReturn.pz = -pz;
  toReturn.qx = qx;
  toReturn.qy = qy;
  toReturn.qz = qz;
  toReturn.qw = qw;
  return toReturn;
}

_eePose _eePose::negativeQ() const {
  _eePose toReturn;
  toReturn.px = px;
  toReturn.py = py;
  toReturn.pz = pz;
  toReturn.qx = -qx;
  toReturn.qy = -qy;
  toReturn.qz = -qz;
  toReturn.qw = -qw;
  return toReturn;
}

_eePose _eePose::multQ(const _eePose& a) const {
  Quaternionf thisQ(qw, qx, qy, qz); 
  Quaternionf aQ(a.qw, a.qx, a.qy, a.qz); 
  Quaternionf oQ = thisQ * aQ; 
  
  _eePose toReturn;
  toReturn.px = px;
  toReturn.py = py;
  toReturn.pz = pz;
  toReturn.qx = oQ.x();
  toReturn.qy = oQ.y();
  toReturn.qz = oQ.z();
  toReturn.qw = oQ.w();
  return toReturn;
}

_eePose _eePose::invQ() const {
  Quaternionf thisQ(qw, qx, qy, qz); 
  Quaternionf oQ = thisQ.inverse(); 
  
  _eePose toReturn;
  toReturn.px = px;
  toReturn.py = py;
  toReturn.pz = pz;
  toReturn.qx = oQ.x();
  toReturn.qy = oQ.y();
  toReturn.qz = oQ.z();
  toReturn.qw = oQ.w();
  return toReturn;
}

void _eePose::copyP(_eePose src) {
  px = src.px;
  py = src.py;
  pz = src.pz;
}

void _eePose::copyQ(_eePose src) {
  qx = src.qx;
  qy = src.qy;
  qz = src.qz;
  qw = src.qw;
}

_eePose _eePose::applyQTo(_eePose in) const {
  // Eigen says that when txing more than one point it is more efficient to first convert to Matrix3 then apply
  Quaterniond thisQ(qw, qx, qy, qz); 
  Vector3d inP(in.px, in.py, in.pz);
  Vector3d result = thisQ._transformVector(inP);

  _eePose outPose = eePose::zero();
  outPose.px = result.x();
  outPose.py = result.y();
  outPose.pz = result.z();
  return outPose;
}

void _eePose::writeToFileStorage(FileStorage& fsvO) const {
  fsvO << "{:";
  fsvO << "px" << px;
  fsvO << "py" << py;
  fsvO << "pz" << pz;
  fsvO << "qw" << qw;
  fsvO << "qx" << qx;
  fsvO << "qy" << qy;
  fsvO << "qz" << qz;
  fsvO << "}";
}

void _eePose::readFromFileNodeIterator(FileNodeIterator& it) {
  px = (double)(*it)["px"];
  py = (double)(*it)["py"];
  pz = (double)(*it)["pz"];
  qw = (double)(*it)["qw"];
  qx = (double)(*it)["qx"];
  qy = (double)(*it)["qy"];
  qz = (double)(*it)["qz"];
}

void _eePose::readFromFileNode(FileNode& it) {
  px = (double)(it)["px"];
  py = (double)(it)["py"];
  pz = (double)(it)["pz"];
  qw = (double)(it)["qw"];
  qx = (double)(it)["qx"];
  qy = (double)(it)["qy"];
  qz = (double)(it)["qz"];
}



bool _eePose::equals(_eePose pose)
{
  if (pose.px == this->px &&
      pose.py == this->py &&
      pose.pz == this->pz &&
      pose.qx == this->qx &&
      pose.qy == this->qy &&
      pose.qz == this->qz && 
      pose.qw == this->qw) {
    return true;
  } else {
    return false;
  }
}
