//
// Created by MugnierPC-N202 on 12/11/2020.
//

#ifndef H_FSM_CUSTOM_HSFM_HPP
#define H_FSM_CUSTOM_HSFM_HPP


#include <HFSM.hpp>
#include <cstdint>

enum struct Signal;


/*********State Hierarchy***********/
/*
 *
 * Top_State ---------- User_State_0 ------ User_State_1
 *                            |____________ User_State_1
 *
 */

template<class TOP_STATE_T, class PARENT_STATE_T>
class User_State_1 : public Custom_State_Base<
        TOP_STATE_T, //TOP State Type,
        PARENT_STATE_T,// Parent state Type
        User_State_1<TOP_STATE_T,PARENT_STATE_T>> // Me
{
public:
    User_State_1(){};
    Event_handling_result handler(Event* event){
        return Event_handling_result::IGNORED;
    };
    void init(){};
    void entry(){};
    void exit(){};
};

template<class TOP_STATE_T, class PARENT_STATE_T>
class User_State_0 : public Custom_State_Base<
        TOP_STATE_T, //TOP State Type
        PARENT_STATE_T, // Parent state Type
        User_State_0<TOP_STATE_T,PARENT_STATE_T>, // Me
        User_State_1< //Child 0
                TOP_STATE_T,
                User_State_0<TOP_STATE_T,PARENT_STATE_T>
        >,
        User_State_1< // Child 1
                TOP_STATE_T,
                User_State_0<TOP_STATE_T,PARENT_STATE_T>
        >
>{
public:
    User_State_0(){};
    Event_handling_result handler(Event* event){
        return Event_handling_result::IGNORED;
    };
    void init(){};
    void entry(){};
    void exit(){};
};

class Top_State : public Top_State_Base<
                     User_State_0<Top_State,Top_State>>// Child 0
{
public:
    Top_State(){};
};

class Hfsm : public HFSM_Base<Top_State>{
public:
    Hfsm(){};

};
#endif //H_FSM_CUSTOM_HSFM_HPP
