//
// Created by MugnierPC-N202 on 12/11/2020.
//

#ifndef H_FSM_HFSM_H
#define H_FSM_HFSM_H


#include <cstdint>
#include <tuple>
#include <State.hpp>
#define MAX_NEST_DEPTH 6
/**
 * @brief Class representing the state machine of the framework.
 * Important storage roles :
 * -Contains the top state which contains itself all the substates of the state machine.
 * -This class also holds a pointer to the state handler of the current state.
 * Important functionnal roles :
 * -Receive and dispatch all the incoming events
 * -Realize the transition between states
 *
 * @tparam TOP_STATE_T Type of the top state that the state machine contains
 */




template<class USER_HFSM, class TOP_STATE_T>
class HFSM_Base{

    template<class USER_HFSM_T_bis, class USER_STATE_BASE_T, class USER_STATE_T, typename... SUB_STATES_T>
    friend class State;

    template< class USER_HFSM_T_bis, class PARENT_STATE_T, class CUSTOM_STATE_T, typename ...SUB_STATES_T >
    friend class Custom_State_Base;



public:



    /**
     * @brief Dispatch an event to the state handler of the current state
     * @param event Event-derived class
     */
    void dispatch(const Event* const event){
        Handler_Func& a = *_current_state_h;
        a(event);
    };

    bool is_in();

    /**
     *
     * @tparam STATE_TYPE_HIERARCHY_TO_LEAF Series of template parameters defining the position of the state in the hierarchy
     * @param struct_ptr Pointer to a derived type of Setup_Struct which contains all the customs parameter that the state has to integrate
     */
    template<typename ...STATE_TYPE_HIERARCHY_TO_LEAF, typename SETUP_PTR_T >
    void state_setup(SETUP_PTR_T struct_ptr){
        _top_state.template state_setup<STATE_TYPE_HIERARCHY_TO_LEAF...>(struct_ptr);
    };

protected:
    /**
 * @brief The base constructor initializing the state machine. Called by the child
 */
    TOP_STATE_T _top_state;


    Handler_Func* _current_state_h;
    Handler_Func* path[MAX_NEST_DEPTH];
    HFSM_Base();

};

template<class USER_HFSM, class TOP_STATE_T>
HFSM_Base<USER_HFSM, TOP_STATE_T>::HFSM_Base() {
    _top_state.pass_ptrs_to_state(static_cast<USER_HFSM*>(this)); // Pass the this pointer to all substates
    _top_state.init();
};
#endif //H_FSM_HFSM_H
