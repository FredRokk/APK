/*
 * Filename: /home/stud/APK/FinalPRJ/include/type.hpp
 * Path: /home/stud/APK/FinalPRJ/include
 * Created Date: Monday, November 23rd 2020, 3:42:40 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 * 
 * Copyright (c) 2020 APK - Group 4
 */
//- capacity_ : int
//- size_ : int
template<int Capacity, int Size>
class type
{
private:
    int capacity_;
    int size_;
public:
    type(/* args */) : capacity_(Capacity), size_(Size);
    ~type();
};

type::type(/* args */)
{
}

type::~type()
{
}

