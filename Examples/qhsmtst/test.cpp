#include "original_qhsmtst.hpp"
#include <array>


int main() {

    //Instanciation of the state machine
    HFSM state_machine;

    // Instanciation of the events
    Event A_ev = {Signal::A};
    Event C_ev = {Signal::C};
    Event D_ev = {Signal::D};
    Event E_ev = {Signal::E};
    Event G_ev = {Signal::G};
    Event I_ev = {Signal::I};

    // Events are organized into an array , just for test purposes
    std::array<Event, 11> ev_array = {G_ev, I_ev, A_ev, D_ev, D_ev, C_ev, E_ev, E_ev, G_ev, I_ev, I_ev};

    // Sequential injection of the events
    for (Event &ev :ev_array) {
        std::cout << std::endl;
        state_machine.dispatch(&ev);
        }
    }
