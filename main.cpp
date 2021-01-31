#include <iostream>
#include "Custom_Hsfm2.hpp"
#include "HFSM.hpp"

int main() {
    Hfsm_bis h = Hfsm_bis();

    Event ev = {Signal::CS_SIG};
    Setup_Struct str = {5};
    //h.state_setup<User_State_0_bis>(&str);
    h.dispatch(&ev);

std::cout<< "END" ;
}
