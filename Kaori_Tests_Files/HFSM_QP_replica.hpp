#pragma once
#include "HFSM.hpp"

class Top_State;
class s;
class s1;
class s11;
class s2;
class s21;
class s211;

class s211 : public Custom_State_Base<
        Top_State, //TOP State Type
        s21, // Parent state Type
        s211> // Me
{
public:

    Handling_Result handler(Event* event) {
        switch (event->_sig) {
            default:
                return Handling_Result::IGNORED ;
        }
    }

    void init(){ std::cout<< "s211_INIT ;" ;}
    void entry(){ std::cout<< "s211_ENTRY ;" ;}
    void exit(){ std::cout<< "s211_EXIT ;" ;}
};

class s21 : public Custom_State_Base<
        Top_State, //TOP State Type
        s2, // Parent state Type
        s21, // Me
        s211> // First child
{
public:

    Handling_Result handler(Event* event) {
        switch (event->_sig) {
            default:
                return Handling_Result::IGNORED ;
        }
    }

    void init(){ std::cout<< "s21_INIT ;" ;}
    void entry(){ std::cout<< "s21_ENTRY ;" ;}
    void exit(){ std::cout<< "s21_EXIT ;" ;}
};

class s2 : public Custom_State_Base<
        Top_State, //TOP State Type
        s, // Parent state Type
        s2, // Me
        s21> // First child
{
public:

    Handling_Result handler(Event* event) {
        switch (event->_sig) {
            default:
                return Handling_Result::IGNORED ;
        }
    }

    void init(){ std::cout<< "s2_INIT ;" ;}
    void entry(){ std::cout<< "s2_ENTRY ;" ;}
    void exit(){ std::cout<< "s2_EXIT ;" ;}
};

class s11 : public Custom_State_Base<
        Top_State, //TOP State Type
        s1, // Parent state Type
        s11> // Me
{
public:

    Handling_Result handler(Event* event) {
        switch (event->_sig) {
            default:
                return Handling_Result::IGNORED ;
        }

    }

    void init(){ std::cout<< "s11_INIT ;" ;}
    void entry(){ std::cout<< "s11_ENTRY ;" ;}
    void exit(){ std::cout<< "s11_EXIT ;" ;}
};

class s1 : public Custom_State_Base<
        Top_State, //TOP State Type
        s, // Parent state Type
        s1, // Me
        s11> // First child
{
public:

    Handling_Result handler(Event* event) {
        switch (event->_sig) {
            default:
                return Handling_Result::IGNORED ;
        }
    }

    void init(){ std::cout<< "s1_INIT ;" ;}
    void entry(){ std::cout<< "s1_ENTRY ;" ;}
    void exit(){ std::cout<< "s1_EXIT ;" ;}
};

class s : public Custom_State_Base<
        Top_State, //TOP State Type
        Top_State, // Parent state Type
        s, // Me
        s1, // Child 0
        s2> //Child 1
{
public:

    Handling_Result handler(Event* event) {
        switch (event->_sig) {
            default:
                return Handling_Result::IGNORED ;
        }
    }

    void init(){ std::cout<< "s_INIT ;" ;}
    void entry(){ std::cout<< "s_ENTRY ;" ;}
    void exit(){ std::cout<< "s_EXIT ;" ;}
};

class Top_State : public Top_State_Base<
        Top_State, //Me = Top_State
        s >// Child 0
{
public:
    Top_State(){};

    //Initial transition
    void init(){
        std::cout<< "top_INIT ;" ;
        initial_transition_to_state<s2>();
    }

};

class HFSM : public HFSM_Base<Top_State>{
public:
    HFSM(){};
protected:
    bool m_foo;
};