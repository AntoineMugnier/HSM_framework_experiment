//
// Created by MugnierPC-N202 on 12/11/2020.
//

#ifndef H_FSM_CUSTOM_HSFM_BIS_HPP
#define H_FSM_CUSTOM_HSFM_BIS_HPP


#include <HFSM.hpp>
#include <cstdint>
#include "string"
enum struct Signal;


struct Custom_Event : Event{
    uint32_t data;
};
/*********State Hierarchy***********/

/*
 *
 * Top_State_bis ---------- User_State_0_bis ------ User_State_1_bis
 *                            |____________ User_State_1_bis
 *
 */
class Top_State_bis;

class User_State_0_bis;

class User_State_2_bis : public Custom_State_Base<
        Top_State_bis, //TOP State Type,
        User_State_0_bis,// Parent state Type
        User_State_2_bis> // Me
{
public:
    User_State_2_bis(){};
    Handling_Result handler(Event* event){
        return Handling_Result::IGNORED;
    };

    void init(){
        std::cout<< "Init of customized state <User_State_0_bis,User_State_2_bis>\n";
        std::cout<< S ;
    };
    void entry(){};
    void exit(){};
private:
    std::string S = "String triggered !\n" ;
};



class User_State_1_bis : public  Custom_State_Base<
        Top_State_bis, //TOP State Type,
        User_State_0_bis,// Parent state Type
        User_State_1_bis> // Me
{
public:
    User_State_1_bis(){};
    Handling_Result handler(Event* event){
        return Handling_Result::IGNORED;
    };
    void init(){
        std::cout<< "Init of customized state <User_State_0_bis,User_State_1_bis>";

    };
    void entry(){};
    void exit(){};
};

struct Setup_Struct {
    int data;
};

class User_State_0_bis : public Custom_State_Base<
        Top_State_bis, //TOP State Type
        Top_State_bis, // Parent state Type
        User_State_0_bis, // Me
        User_State_1_bis, // Child 0
        User_State_2_bis>{ //Child 1
public:
    User_State_0_bis(){};
    Handling_Result handler(Event* event){
        return Handling_Result::IGNORED;
    };
    void init(){
        std::cout<< "Init of customized state <User_State_0_bis>\n";

        std::cout<< _data ;
    };
    void entry(){};
    void exit(){};
    void setup(Setup_Struct* struct_ptr){
        _data = struct_ptr->data ;
    };
private:
    int _data;
};


class Top_State_bis : public Top_State_Base<Top_State_bis,
        User_State_0_bis>// Child 0
{
public:
Top_State_bis(){};
    //Initial transition
    void init(){
        set_as_current_state<User_State_0_bis>();
    }
};

class Hfsm_bis : public HFSM_Base<Top_State_bis>{
public:
    Hfsm_bis(){};


};





#endif //H_FSM_CUSTOM_HSFM_BIS_HPP
