#pragma once
#include "HFSM.hpp"
#include <iostream>
class HFSM;
class Top_State;
class s;
class s1;
class s11;
class s2;
class s21;
class s211;

/**********************************************************************************************************************/

class s211 : public Custom_State_Base<
        HFSM, //User HFSM State Type
        s21, // Parent state Type
        s211> // Me
{
public:

    Handling_Result handler(const Event* const event);

    void entry(){ std::cout<< "s211-ENTRY ;" ;}
    void exit(){ std::cout<< "s211-EXIT ;" ;}
};

/**********************************************************************************************************************/

class s21 : public Custom_State_Base<
        HFSM, //User HFSM State Type
        s2, // Parent state Type
        s21, // Me
        s211> // First child
{
public:

    Handling_Result handler(const Event* const event);

    void init();

    void entry(){ std::cout<< "s21-ENTRY ;" ;}
    void exit(){ std::cout<< "s21-EXIT ;" ;}
};

/**********************************************************************************************************************/

class s2 : public Custom_State_Base<
        HFSM, //User HFSM State Type
        s, // Parent state Type
        s2, // Me
        s21> // First child
{
public:

    Handling_Result handler(const Event* const event);

    void init();
    void entry(){ std::cout<< "s2-ENTRY ;" ;}
    void exit();
};

/**********************************************************************************************************************/

class s11 : public Custom_State_Base<
        HFSM, //User HFSM State Type
        s1, // Parent state Type
        s11> // Me
{
public:

    Handling_Result handler(const Event* const event);

    void entry(){ std::cout<< "s11-ENTRY ;" ;}
    void exit(){ std::cout<< "s11-EXIT ;" ;}
};

/**********************************************************************************************************************/

class s1 : public Custom_State_Base<
        HFSM, //User HFSM State Type
        s, // Parent state Type
        s1, // Me
        s11> // First child
{
public:

    Handling_Result handler(const Event* const event);

    void init();
    void entry(){ std::cout<< "s1-ENTRY ;" ;}
    void exit(){ std::cout<< "s1-EXIT ;" ;}
};

/**********************************************************************************************************************/

class s : public Custom_State_Base<
        HFSM, //User HFSM State Type
        Top_State, // Parent state Type
        s, // Me
        s1, // Child 0
        s2> //Child 1
{
public:

    Handling_Result handler(const Event* const event);

    void init();
    void entry(){ std::cout<< "s-ENTRY ;" ;}
    void exit();
};

/**********************************************************************************************************************/

class Top_State : public Top_State_Base<
        HFSM,
        Top_State, //Me = Top_State
        s >// Child 0
{
    friend HFSM;
public:
    Top_State(){};

    //Initial transition
    void init();
};

/**********************************************************************************************************************/

class HFSM : public HFSM_Base<HFSM, Top_State>{
public:
    HFSM(){};
    friend Top_State; friend s; friend s1; friend s11; friend s2; friend s21; friend s211;
protected:
    bool m_foo;
};

/**********************************************************************************************************************/