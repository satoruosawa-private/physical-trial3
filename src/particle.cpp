//
//  particle.cpp
//  physical-trial3
//
//  Created by OSAWASatoru on 6/3/17.
//
//

#include "./particle.h"

Particle::Particle() {
  friction_ = 0.001;
  gravity_.set(0, 0.2);
  mass_ = 3;
  radius_ = 3;
  bFixed_ = false;
}

Particle::Particle(float f, const ofVec2f &g, float m, float r)
:friction_(f),
gravity_(g),
mass_(m),
radius_(r) {
  bFixed_ = false;
}

void Particle::setup(const ofVec2f &p, const ofVec2f &v) {
  position_ = p;
  velocity_ = v;
}

void Particle::update() {
  addForce(gravity_);
  updateForce();
  updatePos();
  bounceOfWalls();

}

void Particle::draw() {
  ofDrawCircle(position_, radius_);
}

void Particle::resetForce() {
  force_.set(0, 0);
}

void Particle::addForce(const ofVec2f &f) {
  force_ += f;
}

void Particle::updateForce() {
  force_ -= velocity_ * friction_;
}

void Particle::updatePos() {
  if (!bFixed_) {
    velocity_ += force_;
    position_ += velocity_;
  }
}

void Particle::addAttractionForce(const ofVec2f &posOfForce,
                                  float radius, float scale) {
  ofVec2f diff = position_ - posOfForce;
  float length = diff.length();
  bool bAmCloseEnough = true;
  if (radius > 0){
    if (length > radius){
      bAmCloseEnough = false;
    }
  }
  if (bAmCloseEnough == true){
    float pct = 1 - (length / radius);
    diff.normalize();
    force_ -= diff * scale * pct;
  }
}

void Particle::addAttractionForce(Particle * targetP,
                                  float radius, float scale) {
  ofVec2f diff = position_ - targetP->getPosition();
  float length = diff.length();
  bool bAmCloseEnough = true;
  if (radius > 0){
    if (length > radius){
      bAmCloseEnough = false;
    }
  }
  if (bAmCloseEnough == true){
    float pct = 1 - (length / radius);
    diff.normalize();
    ofVec2f frcToAdd = diff * scale * pct;
    force_ -= frcToAdd;
    targetP->addForce(frcToAdd);
  }
}

void Particle::addRepulsionForce(const ofVec2f &posOfForce,
                                 float radius, float scale) {
  ofVec2f diff = position_ - posOfForce;
  float length = diff.length();
  bool bAmCloseEnough = true;
  if (radius > 0){
    if (length > radius){
      bAmCloseEnough = false;
    }
  }
  if (bAmCloseEnough == true){
    float pct = 1 - (length / radius);
    diff.normalize();
    force_ += diff * scale * pct;
  }
}

void Particle::addRepulsionForce(const ofVec2f &posOfFrcLineA,
                                 const ofVec2f &posOfFrcLineB,
                                 float radius, float scale) {
  ofVec2f normalOfLine = (posOfFrcLineA - posOfFrcLineB).getPerpendicular();
  float diff_length = abs((posOfFrcLineA - position_).dot(normalOfLine));
  bool bAmCloseEnough = true;
  if (radius > 0){
    if (diff_length > radius){
      bAmCloseEnough = false;
    }
  }
  if (bAmCloseEnough == true){
    float pct = 1 - (diff_length / radius);
    force_ += normalOfLine * scale * pct;
  }
}


void Particle::addRepulsionForce(Particle * targetP,
                                 float radius, float scale) {
  ofVec2f diff = position_ - targetP->getPosition();
  float length = diff.length();
  bool bAmCloseEnough = true;
  if (radius > 0){
    if (length > radius){
      bAmCloseEnough = false;
    }
  }
  if (bAmCloseEnough == true){
    float pct = 1 - (length / radius);
    diff.normalize();
    ofVec2f frcToAdd = diff * scale * pct;
    force_ += frcToAdd;
    targetP->addForce(-frcToAdd);
  }
}

void Particle::bounceOfWalls() {
  bool bDidICollide = false;
  
  float xmin = 0;
  float xmax = ofGetWidth();
  float ymin = 0;
  float ymax = ofGetHeight();
  
  if (position_.x < xmin) {
    position_.x = xmin + (xmin - position_.x);
    velocity_.x *= -1;
    bDidICollide = true;
  } else if (position_.x > xmax) {
    position_.x = xmax - (position_.x - xmax);
    velocity_.x *= -1;
    bDidICollide = true;
  }
  if (position_.y < ymin) {
    position_.y = ymin + (ymin - position_.y);
    velocity_.y *= -1;
    bDidICollide = true;
  } else if (position_.y > ymax) {
    position_.y = ymax - (position_.y - ymax);
    velocity_.y *= -1;
    bDidICollide = true;
  }
  if (bDidICollide == true) {
    velocity_ *= 1.0;
  }
}
