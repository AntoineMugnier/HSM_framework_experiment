#include <iostream>
//#include "Custom_Hsfm2.hpp"
#include "HFSM.hpp"
#include "HFSM_QP_replica.hpp"
#include <array>
int main() {

    HFSM h ;
    Event A_ev = {Signal::A};
    //Event B_ev = {Signal::B};
    Event C_ev = {Signal::C};
    Event D_ev = {Signal::D};
    Event E_ev = {Signal::E};
    //Event F_ev = {Signal::F};
    Event G_ev = {Signal::G};
    //Event H_ev = {Signal::H};
    Event I_ev = {Signal::I};

    std::array<Event, 11> ev_array = {G_ev, I_ev, A_ev, D_ev, D_ev, C_ev, E_ev, E_ev, G_ev, I_ev, I_ev};
    for(Event& ev :ev_array ){
        std::cout << std::endl ;
        h.dispatch(&ev);
    }
    //


/*
    Event ev = {Signal::CS_SIG};
    //Setup_Struct str = {5};
    //h.state_setup<User_State_0_bis>(&str);
    h.dispatch(&ev);
*/


std::cout << std::endl << "END" ;
}
