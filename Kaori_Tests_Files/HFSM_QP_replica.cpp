#include "HFSM_QP_replica.hpp"
#include <iostream>


void s2::exit() {
    if( ! _hfsm->m_foo){
        _hfsm->m_foo = true ;
    }
    std::cout<< "s2_EXIT ;" ;
}

void Top_State::init() {
    std::cout<< "top_INIT ;" ;
    _hfsm->m_foo = false;
    initial_transition_to_state<s2>();
}

void s::exit() {
    if(_hfsm->m_foo){
        _hfsm->m_foo = false ;
    }
    std::cout<< "s_EXIT ;" ;}

void s::init() {
    std::cout<< "s_INIT ;" ;
    initial_transition_to_state<s11>();

}

Handling_Result s::handler(Event *event) {
    switch (event->_sig) {
        case Signal::I:
            std::cout << "s-I" ;
            if(_hfsm->m_foo)_hfsm->m_foo = false;
        default:
            return Handling_Result::IGNORED ;
    }
}

Handling_Result s1::handler(Event *event) {
    switch (event->_sig) {
        case Signal::A:
            std::cout << "s-A" ;
            return trigger_transition<s1>();
        case Signal::B:
            std::cout << "s-B" ;
            return trigger_transition<s11>();
        case Signal::C:
            std::cout << "s-C" ;
            return trigger_transition<s2>();
        case Signal::D:
            if (!_hfsm->m_foo) _hfsm->m_foo = true;
            std::cout << "s-D" ;
            return trigger_transition<s>();
        case Signal::F:
            std::cout << "s-F" ;
            return trigger_transition<s211>();
        default:
            return Handling_Result::IGNORED ;
    }
}

void s1::init() {
    std::cout << "s1_INIT ;";
    initial_transition_to_state<s11>();
}

Handling_Result s11::handler(Event *event) {
    switch (event->_sig) {
        case Signal::D:
            std::cout << "s11-D" ;
            if (_hfsm->m_foo) _hfsm->m_foo = false ;
        case Signal::H:
            std::cout << "s11-H" ;
            return trigger_transition<s>();
        case Signal::G:
            std::cout << "s11-G" ;
            return trigger_transition<s211>();
        default:
            return Handling_Result::IGNORED ;
    }

}
