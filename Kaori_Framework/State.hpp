
#ifndef H_FSM_STATES_H
#define H_FSM_STATES_H

#include "tuple"
#include "functional"
#include "Events.hpp"

template<typename TOP_STATE_T >
class HFSM_Base;

template< class TOP_STATE_T, class PARENT_STATE_T, class CUSTOM_STATE_T, typename ...SUB_STATES_T >
class Custom_State_Base;

struct Event;

/*----------------BASE STATE CLASS---------------- */
template<class TOP_STATE_T, class STATE_CLASS_CHILD_T, typename ...SUB_STATES_T >
class State  {
    using Substate_Tuple_T = std::tuple<SUB_STATES_T...>;

public:

     Handler_Func state_handler= [this]( Event* event) -> Handling_Result {
        Handling_Result result =  static_cast<STATE_CLASS_CHILD_T*>(this) -> custom_state_base_handler(event);
        return result;
    };

    template<typename HEAD_STATE_T, typename NECK_STATE_T, typename... OTHER_STATE_T, typename SETUP_PTR_T>
    void state_setup(SETUP_PTR_T setup_ptr){
        HEAD_STATE_T&  tuple_elmt = std::get<HEAD_STATE_T>(_substates);
        tuple_elmt.template state_setup<NECK_STATE_T, OTHER_STATE_T...>(setup_ptr);
    };

    template<typename TAIL_STATE_T, typename SETUP_PTR_T>
    void state_setup(SETUP_PTR_T setup_ptr){
        TAIL_STATE_T&  tuple_elmt = std::get<TAIL_STATE_T>(_substates);
        tuple_elmt. template custom_state_base_setup(setup_ptr);
    };

    /**
     * @brief Overloaded method, allows to drill into the state hierarchy to find the state corresponding to the position passed as template parameters.
     * @tparam HEAD_STATE_T Substate position in tuple . We call its set_as_current_state() method
     * @tparam NECK_STATE_T Subsubstate position in substate tuple at pos HEAD_STATE_T. This parameter is just used to allow templated method overload.
     * Otherwise, determining which function to call becomes ambiguous for the compiler.
     * @tparam OTHER_STATE_T
     */
    template<typename HEAD_STATE_T, typename NECK_STATE_T, typename... OTHER_STATE_T>
    constexpr void set_as_current_state();

    /**
     * @brief Last call of this overloaded method, which sets the state at position STATE_POS as the current state of the FSM.
     * @tparam STATE_POS
     */
    template<typename STATE_POS>
    constexpr void set_as_current_state();

/**
 * @brief Method called after the last recursive call of the namesake template method.
 * It does nothing, it serves just to end this series of calls.
 * @param hfsm Pointer to the final state machine object
 */
    void pass_hfsm_ptr_to_substates (HFSM_Base<TOP_STATE_T>* hfsm , std::integral_constant<int, std::tuple_size<std::tuple<SUB_STATES_T...>>::value>);

    /**
     * @brief Recursive method, allows to dispatch the hfsm pointer to all the substates of the state.
     * @tparam index Index of the tuple element in tuple
     * @param hfsm Pointer to the final state machine object
     */
    template<int index = 0>
    void pass_hfsm_ptr_to_substates (HFSM_Base<TOP_STATE_T>* hfsm, std::integral_constant<int, index> = std::integral_constant<int, 0>());


    Handler_Func* get_handler(){
        return  &state_handler;
    }



protected:
    State(){};
    Substate_Tuple_T _substates;

};


template<class TOP_STATE_T, class STATE_CLASS_CHILD_T, typename... SUB_STATES_T>
template<typename HEAD_STATE_T, typename NECK_STATE_T, typename... OTHER_STATE_T>
constexpr void State<TOP_STATE_T, STATE_CLASS_CHILD_T, SUB_STATES_T...>::set_as_current_state() {
    HEAD_STATE_T&  tuple_elmt = std::get<HEAD_STATE_T>(_substates);
    tuple_elmt.template set_as_current_state<NECK_STATE_T, OTHER_STATE_T...>();
}

template<class TOP_STATE_T, class STATE_CLASS_CHILD_T, typename... SUB_STATES_T>
template<typename TAIL_STATE_T>
constexpr void State<TOP_STATE_T, STATE_CLASS_CHILD_T, SUB_STATES_T...>::set_as_current_state() {
    //TODO Static assert to check if this is not the top state that we are targeting
    TAIL_STATE_T&  tuple_elmt = std::get<TAIL_STATE_T>(_substates);
    tuple_elmt.set_as_current_state_custom();
}


template<class TOP_STATE_T, class STATE_CLASS_CHILD_T, typename... SUB_STATES_T>
void State<TOP_STATE_T, STATE_CLASS_CHILD_T, SUB_STATES_T...>::pass_hfsm_ptr_to_substates(HFSM_Base<TOP_STATE_T>* hfsm,
        std::integral_constant<int, std::tuple_size<std::tuple<SUB_STATES_T...>>::value>) {
// Do nothing, all tuple elements have received the pointer
}

template<class TOP_STATE_T, class STATE_CLASS_CHILD_T, typename... SUB_STATES_T>
template<int index>
void State<TOP_STATE_T, STATE_CLASS_CHILD_T, SUB_STATES_T...>::pass_hfsm_ptr_to_substates(HFSM_Base<TOP_STATE_T>* hfsm, std::integral_constant<int, index>) {
    typename std::tuple_element<index, Substate_Tuple_T>::type& substate = std::get<index>(_substates);
    substate.pass_hfsm_ptr_to_state(hfsm);
    pass_hfsm_ptr_to_substates(hfsm, std::integral_constant<int, index+1>());

}



/*----------------BASE CLASS FOR USER STATES---------------- */
template< class TOP_STATE_T, class PARENT_STATE_T, class CUSTOM_STATE_T, typename ...SUB_STATES_T >
class Custom_State_Base : public State<TOP_STATE_T, Custom_State_Base<TOP_STATE_T,PARENT_STATE_T, CUSTOM_STATE_T, SUB_STATES_T... >,  SUB_STATES_T ...>{

    friend class HFSM_Base <TOP_STATE_T>;
    using Parent_State_T = State<TOP_STATE_T, Custom_State_Base<TOP_STATE_T,PARENT_STATE_T, CUSTOM_STATE_T, SUB_STATES_T... >,  SUB_STATES_T ...>;
    using Substate_Tuple_T = std::tuple<SUB_STATES_T...>;
    
public:

    Custom_State_Base(){};

    Handling_Result custom_state_base_handler( Event* event);

    template<typename ...STATE_TYPE_HIERARCHY_TO_LEAF>
    void trigger_transition();

    void set_as_current_state_custom(){
        _hfsm->_current_state_h = & (Parent_State_T::state_handler) ;
    }

    template<typename SETUP_PTR_T>
    void custom_state_base_setup(SETUP_PTR_T setup_ptr){
        static_cast<CUSTOM_STATE_T*>(this) -> setup(setup_ptr);
    };

    void pass_hfsm_ptr_to_state(HFSM_Base<TOP_STATE_T>* hfsm){
        _hfsm = hfsm;

        // Pass hfsm ptr to substates only if they does exist
        if constexpr (std::tuple_size<Substate_Tuple_T>::value >0) {
            Parent_State_T::pass_hfsm_ptr_to_substates(hfsm);
        }
    };

protected:
    PARENT_STATE_T* _parent_state;

private:

    HFSM_Base<TOP_STATE_T>* _hfsm;
    void exit_to_parent();
};


template<class TOP_STATE_T, class PARENT_STATE_T, class CUSTOM_STATE_T, typename... SUB_STATES_T>
template<typename ...STATE_TYPE_HIERARCHY_TO_LEAF>
void Custom_State_Base<TOP_STATE_T, PARENT_STATE_T, CUSTOM_STATE_T, SUB_STATES_T...>::trigger_transition() {
    //_hfsm->_top_state_h.template  set_as_current_state<STATE_TYPE_HIERARCHY_TO_LEAF...>();
    
}


template<class TOP_STATE_T, class PARENT_STATE_T, class CUSTOM_STATE_T, typename... SUB_STATES_T>
Handling_Result
Custom_State_Base<TOP_STATE_T, PARENT_STATE_T, CUSTOM_STATE_T, SUB_STATES_T...>::custom_state_base_handler( Event *event) {



    switch (event->_sig) {
        case Signal::INIT_SIG:
            static_cast<CUSTOM_STATE_T*>(this) ->init();
            return Handling_Result::HANDLED;
            break;
        case Signal::ENTRY_SIG:
            static_cast<CUSTOM_STATE_T*>(this) ->entry();
            return Handling_Result::HANDLED;
            break;
        case Signal::EXIT_SIG :
            static_cast<CUSTOM_STATE_T*>(this) ->exit();
            exit_to_parent();
            return Handling_Result::HANDLED;
            break;
        default:
            return static_cast<CUSTOM_STATE_T*>(this) -> handler(event);
    }

}

template<class TOP_STATE_T, class PARENT_STATE_T, class CUSTOM_STATE_T, typename... SUB_STATES_T>
void Custom_State_Base<TOP_STATE_T, PARENT_STATE_T, CUSTOM_STATE_T, SUB_STATES_T...>::exit_to_parent() {
    _hfsm->_current_state_h = _parent_state->get_handler();
}


/*----------------BASE CLASS FOR TOP STATE---------------- */
template<typename TOP_STATE_T, typename ...SUB_STATES_T >
class Top_State_Base : public State<TOP_STATE_T, Top_State_Base<TOP_STATE_T,SUB_STATES_T...>, SUB_STATES_T...> {
    using Parent_State_T = State<TOP_STATE_T, Top_State_Base<TOP_STATE_T,SUB_STATES_T...>, SUB_STATES_T...>;
    using Substate_Tuple_T = std::tuple<SUB_STATES_T...>;

public:
    Top_State_Base(){};

    Handling_Result custom_state_base_handler( Event* event) {
        return Handling_Result::IGNORED;
    }

    void pass_hfsm_ptr_to_state(HFSM_Base<TOP_STATE_T>* hfsm){
        // Pass hfsm ptr to substates only if they does exist
        if constexpr (std::tuple_size<Substate_Tuple_T>::value >0) {
            Parent_State_T::pass_hfsm_ptr_to_substates(hfsm);
        }

        Parent_State_T::pass_hfsm_ptr_to_substates(hfsm);
    }
};
#endif //H_FSM_STATES_H