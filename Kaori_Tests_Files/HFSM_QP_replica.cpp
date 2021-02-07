#include "HFSM_QP_replica.hpp"
#include <iostream>


void s2::exit() {
    if( ! _hfsm->m_foo){
        _hfsm->m_foo = true ;
    }
    std::cout<< "s2-EXIT ;" ;
}

Handling_Result s2::handler(const Event* const event) {
    switch (event->_sig) {
        case Signal::I:
            std::cout << "s2-I ;" ;
            if(!_hfsm->m_foo) _hfsm->m_foo = true ;
            return Handling_Result::HANDLED;
        case Signal::C:
            std::cout << "s2-C ;" ;
            return trigger_transition<s1>();
        case Signal::F:
            std::cout << "s2-F ;" ;
            return trigger_transition<s11>();
        default:
            return Handling_Result::IGNORED ;
    }
}

void s2::init() {
    initial_transition_to_state<s211>();
    std::cout<< "s2-INIT ;" ;
}

void Top_State::init() {
    std::cout<< "top-INIT ;" ;
    _hfsm->m_foo = false;
    initial_transition_to_state<s2>();
}

void s::exit() {
    if(_hfsm->m_foo){
        _hfsm->m_foo = false ;
    }
    std::cout<< "s-EXIT ;" ;}

void s::init() {
    std::cout<< "s-INIT ;" ;
    initial_transition_to_state<s11>();

}

Handling_Result s::handler(const Event* const event) {
    switch (event->_sig) {
        case Signal::I:
            std::cout << "s-I ;" ;
            if(_hfsm->m_foo)_hfsm->m_foo = false;
            return Handling_Result::HANDLED;
        default:
            return Handling_Result::IGNORED ;
    }
}

Handling_Result s1::handler(const Event* const event) {
    switch (event->_sig) {
        case Signal::A:
            std::cout << "s1-A ;" ;
            return trigger_transition<s1>();
        case Signal::B:
            std::cout << "s1-B ;" ;
            return trigger_transition<s11>();
        case Signal::C:
            std::cout << "s1-C ;" ;
            return trigger_transition<s2>();
        case Signal::D:
            if (!_hfsm->m_foo) _hfsm->m_foo = true;
            std::cout << "s1-D ;" ;
            return trigger_transition<s>();
        case Signal::F:
            std::cout << "s1-F ;" ;
            return trigger_transition<s211>();
        case Signal::I:
            std::cout << "s1-I ;" ;
            return Handling_Result::HANDLED;
        default:
            return Handling_Result::IGNORED ;
    }
}

void s1::init() {
    std::cout << "s1-INIT ;";
    initial_transition_to_state<s11>();
}

Handling_Result s11::handler(const Event* const event) {
    switch (event->_sig) {
        case Signal::D:
            std::cout << "s11-D ;" ;
            if (_hfsm->m_foo) _hfsm->m_foo = false ;
            return trigger_transition<s1>();
        case Signal::H:
            std::cout << "s11-H ;" ;
            return trigger_transition<s>();
        case Signal::G:
            std::cout << "s11-G ;" ;
            return trigger_transition<s211>();
        default:
            return Handling_Result::IGNORED ;
    }
}

void s21::init() {
    std::cout<< "s21-INIT ;" ;
    initial_transition_to_state<s211>();
}

Handling_Result s21::handler(const Event* const event) {
    switch (event->_sig) {
        case Signal::A:
            std::cout << "s21-A ;" ;
            return trigger_transition<s21>();
        case Signal::B:
            std::cout << "s21-B ;" ;
            return trigger_transition<s211>();
        case Signal::G:
            std::cout << "s21-G ;" ;
            return trigger_transition<s11>();
        default:
            return Handling_Result::IGNORED ;
    }
}

Handling_Result s211::handler(const Event* const event) {
    switch (event->_sig) {
        case Signal::D:
            std::cout << "s221-D ;" ;
            return trigger_transition<s21>();
        case Signal::H:
            std::cout << "s221-H ;" ;
            return trigger_transition<s>();
        default:
            return Handling_Result::IGNORED ;
    }
}
