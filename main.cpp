#include <iostream>
#include "HFSM.hpp"
#include "HFSM_QP_replica.hpp"
#include <array>
#include <chrono>

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

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i<1000;i++) {
        std::array<Event, 11> ev_array = {G_ev, I_ev, A_ev, D_ev, D_ev, C_ev, E_ev, E_ev, G_ev, I_ev, I_ev};


        for (Event &ev :ev_array) {
            std::cout << std::endl;
            h.dispatch(&ev);
        }
        std::cout << std::endl << "END" << std::endl ;



    }
    auto elapsed = std::chrono::high_resolution_clock::now() - start;

    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    microseconds/=1000 ;
    std::cout << "Operation took " << microseconds << std::endl;


/*
    Event ev = {Signal::CS_SIG};
    //Setup_Struct str = {5};
    //h.state_setup<User_State_0_bis>(&str);
    h.dispatch(&ev);
*/


}
