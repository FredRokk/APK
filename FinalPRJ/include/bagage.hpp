/*
 * Filename: /home/stud/APK/FinalPRJ/include/bagage.hpp
 * Path: /home/stud/APK/FinalPRJ/include
 * Created Date: Wednesday, November 25th 2020, 2:23:31 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 *
 * Copyright (c) 2020 APK - Group 4
 */
#ifndef _BAGAGE_
#define _BAGAGE_
class bagage
{
private:
  int bagageId_;
  int bagageWeight_;

public:
  bagage(const int id, const int weight)
      : bagageId_(id), bagageWeight_(weight){};
  ~bagage(){};
  int  getWeight() const { return bagageWeight_; };
  int  getId() const { return bagageId_; };
  void setWeight(int weight) { bagageWeight_ = weight; };
  void setId(int id) { bagageId_ = id; };
};
#endif /* _BAGAGE_ */