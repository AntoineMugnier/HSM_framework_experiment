
#ifndef H_FSM_STATES_H
#define H_FSM_STATES_H

#include "tuple"
#include "functional"
#include "Events.hpp"
#include "HFSM.hpp"
#include <type_traits>

template<class TOP_STATE_T >
class HFSM_Base;

template< class TOP_STATE_T, class PARENT_STATE_T, class USER_STATE_T, class ...SUB_STATE_T >
class Custom_State_Base;

struct Event;

/*----------------BASE STATE CLASS---------------- */
template<class TOP_STATE_T, class USER_STATE_BASE_T, class USER_STATE_T, class ...SUB_STATE_T >
class State  {
    using Substates_Tuple_T = std::tuple<SUB_STATE_T...>;

public:

     Handler_Func state_handler= [this]( Event* event) {
         static_cast<USER_STATE_BASE_T*>(this) -> custom_state_base_handler(event);
    };

    template<class HEAD_STATE_T, class NECK_STATE_T, class... OTHER_STATE_T, class SETUP_PTR_T>
    void state_setup(SETUP_PTR_T setup_ptr){
        HEAD_STATE_T&  tuple_elmt = std::get<HEAD_STATE_T>(_substates);
        tuple_elmt.template state_setup<NECK_STATE_T, OTHER_STATE_T...>(setup_ptr);
    };

    template<class TAIL_STATE_T, class SETUP_PTR_T>
    void state_setup(SETUP_PTR_T setup_ptr){
        TAIL_STATE_T&  tuple_elmt = std::get<TAIL_STATE_T>(_substates);
        tuple_elmt. template custom_state_base_setup(setup_ptr);
    };

    template<class CUSTOM_STATE, class SETUP_PTR_T>
    void state_setup2(SETUP_PTR_T setup_ptr){
        if constexpr ( seek_state<CUSTOM_STATE>()){
            std::cout << "here";
        }
    };




    template<class CUSTOM_STATE_T>
    inline void drill_state_hierarchy(){
        if constexpr (std::tuple_size<Substates_Tuple_T>::value >0) { // "If" statement not necessary?
            drill_into_substates<CUSTOM_STATE_T>();
        }

    }
     bool static constexpr has_substates(){
        if constexpr (std::tuple_size<Substates_Tuple_T>::value > 0){
            return true;
        }
        else{
            return false;
        }
    }

/**
 * @brief Method called after the last recursive call of the namesake template method.
 * It does nothing, it serves just to end this series of calls.
 * @param hfsm Pointer to the final state machine object
 */
    void pass_ptrs_to_substates (HFSM_Base<TOP_STATE_T>* hfsm , std::integral_constant<int, std::tuple_size<std::tuple<SUB_STATE_T...>>::value>);

    /**
     * @brief Recursive method, allows to dispatch the hfsm pointer to all the substates of the state.
     * @tparam index Index of the tuple element in tuple
     * @param hfsm Pointer to the final state machine object
     */
    template<int index = 0>
    void pass_ptrs_to_substates (HFSM_Base<TOP_STATE_T>* hfsm, std::integral_constant<int, index> = std::integral_constant<int, 0>());

    template<class CUSTOM_STATE_T>
    static inline constexpr bool seek_state_in_substates (std::integral_constant<int, std::tuple_size<std::tuple<SUB_STATE_T...>>::value>);

    template<class CUSTOM_STATE_T, int index = 0>
    static inline constexpr bool seek_state_in_substates(std::integral_constant<int, index> = std::integral_constant<int, 0>());

    template<class CUSTOM_STATE_T>
    inline void drill_into_substates (std::integral_constant<int, std::tuple_size<std::tuple<SUB_STATE_T...>>::value>){
        // Error, we should never reach this point
    };

    template<class CUSTOM_STATE_T, int index = 0>
    inline void drill_into_substates (std::integral_constant<int, index> = std::integral_constant<int, 0>()){

            // Catch state at index in the tuple
            typedef class std::tuple_element<index, Substates_Tuple_T>::type substate_t;

            substate_t& substate = std::get<index>(_substates);

            //If this state is the state we seek
            if constexpr (std::is_same<substate_t, CUSTOM_STATE_T>::value){
                // if the target state has substates, enter it and start initial transition
                if constexpr (substate_t::has_substates()) {
                    substate.entry();
                    substate.init();
                    substate._init_transition_cb();
                }
                else {
                    //if the target state does not have substates, enter it and set it as the current state
                    substate.set_as_current_state();
                    substate.entry();
                }
            }

            // If the state we seek is a substate of this state, then enter it and continue drilling
            else if constexpr(substate.template seek_state<CUSTOM_STATE_T>()) {
                substate.entry();
                substate. template drill_state_hierarchy<CUSTOM_STATE_T>();
            }

            // if this state does not have the state we seek, let's move to the next one in the tuple
            else  {
                drill_into_substates<CUSTOM_STATE_T>(std::integral_constant<int, index + 1>());
            }


    };


    template<class CUSTOM_STATE_T>
    static inline constexpr bool seek_state(){
        if constexpr (std::tuple_size<Substates_Tuple_T>::value > 0) {
            return seek_state_in_substates<CUSTOM_STATE_T>();
        }
        else return false;
    }

    void handle_event_from_substate_b(Event* event, State_Exit_Func* substate_exit){
        static_cast<USER_STATE_BASE_T*>(this) -> handle_event_from_substate(event, substate_exit);
    }



protected:
    State(){};
    Substates_Tuple_T _substates;

};



template<class TOP_STATE_T, class USER_STATE_BASE_T, class USER_STATE_T, class... SUB_STATE_T>
void State<TOP_STATE_T, USER_STATE_BASE_T, USER_STATE_T, SUB_STATE_T...>::pass_ptrs_to_substates(HFSM_Base<TOP_STATE_T>* hfsm,
        std::integral_constant<int, std::tuple_size<std::tuple<SUB_STATE_T...>>::value>) {
// Do nothing, all tuple elements have received the pointer
}

template<class TOP_STATE_T, class USER_STATE_BASE_T, class USER_STATE_T, class... SUB_STATE_T>
template<int index>
void State<TOP_STATE_T, USER_STATE_BASE_T, USER_STATE_T, SUB_STATE_T...>::pass_ptrs_to_substates(HFSM_Base<TOP_STATE_T>* hfsm, std::integral_constant<int, index>) {

    std::get<index>(_substates).pass_ptrs_to_state(hfsm, static_cast<USER_STATE_T*>(this));
    pass_ptrs_to_substates(hfsm, std::integral_constant<int, index+1>());

}


template<class TOP_STATE_T, class USER_STATE_BASE_T, class USER_STATE_T, class... SUB_STATE_T>
template<class CUSTOM_STATE_T>
constexpr bool State<TOP_STATE_T, USER_STATE_BASE_T, USER_STATE_T, SUB_STATE_T...>::seek_state_in_substates(std::integral_constant<int, std::tuple_size<std::tuple<SUB_STATE_T...>>::value>) {
// Do nothing, all tuple elements have been visited
    return false;
}

template<class TOP_STATE_T, class USER_STATE_BASE_T, class USER_STATE_T, class... SUB_STATE_T>
template<class CUSTOM_STATE_T, int index>
 constexpr bool State<TOP_STATE_T, USER_STATE_BASE_T, USER_STATE_T, SUB_STATE_T...>::seek_state_in_substates(std::integral_constant<int, index>) {

    // Catch substate at index in the tuple
    //class std::tuple_element<index, Substates_Tuple_T>::type& substate = std::get<index>(_substates);

    //Determinate if this substate is the state type we seek
    if constexpr (std::is_same<class std::tuple_element<index, Substates_Tuple_T>::type, CUSTOM_STATE_T>::value){
        return true;
    }

    // If not, we search for the state type in all of its sub(n)states
    else if constexpr (std::tuple_element<index, Substates_Tuple_T>::type::template seek_state<CUSTOM_STATE_T>()) return true;

    else  {
        return seek_state_in_substates<CUSTOM_STATE_T>(std::integral_constant<int, index + 1>());
    }



}


/*----------------BASE CLASS FOR USER STATES---------------- */
template< class TOP_STATE_T, class PARENT_STATE_T, class USER_STATE_T, class ...SUB_STATE_T >
class Custom_State_Base : public State<TOP_STATE_T, Custom_State_Base<TOP_STATE_T,PARENT_STATE_T, USER_STATE_T, SUB_STATE_T... >, USER_STATE_T,  SUB_STATE_T ...>{

    friend class HFSM_Base <TOP_STATE_T>;
    using Super = State<TOP_STATE_T, Custom_State_Base<TOP_STATE_T,PARENT_STATE_T, USER_STATE_T, SUB_STATE_T... >, USER_STATE_T, SUB_STATE_T ...>;
    using Substates_Tuple_T = std::tuple<SUB_STATE_T...>;
    using Self = Custom_State_Base<TOP_STATE_T, PARENT_STATE_T, USER_STATE_T, SUB_STATE_T...>;

public:

    Custom_State_Base(){};

    void custom_state_base_handler( Event* event);

    template<class CUSTOM_STATE_T>
     constexpr Handling_Result trigger_transition();

    void set_as_current_state(){
        _hfsm->_current_state_h = & (Super::state_handler) ;
    }

    template<class SETUP_PTR_T>
    void custom_state_base_setup(SETUP_PTR_T setup_ptr){
        static_cast<USER_STATE_T*>(this) -> setup(setup_ptr);
    };

    void pass_ptrs_to_state(HFSM_Base<TOP_STATE_T>* hfsm, PARENT_STATE_T* parent_state);;

    void handle_event_from_substate(Event* event, State_Exit_Func* substate_exit){

        _subnstate_exit = substate_exit;

        Super::state_handler(event);

    }

    template<class CUSTOM_STATE_T>
    inline void ascend_state_hierarchy(){
        // If this substate type is the state type we seek
        if constexpr  (std::is_same<CUSTOM_STATE_T, USER_STATE_T>::value){
            static_cast<USER_STATE_T*>(this)->init();
            _init_transition_cb();
        }
        else if constexpr (Super::template seek_state<CUSTOM_STATE_T>()){
            Super::template drill_state_hierarchy<CUSTOM_STATE_T>();
        }
        else{
            static_cast<USER_STATE_T*>(this) -> exit();
            _parent_state-> ascend_state_hierarchy();
        }
    }

    template<class CUSTOM_STATE_T>
    constexpr Handling_Result initial_transition_to_state();

//protected: TODO remove ?
    PARENT_STATE_T* _parent_state;
    State_Exit_Func _state_exit = [=](){static_cast<USER_STATE_T*>(this) -> exit();};
    Subnstate_Exit_Func* _subnstate_exit;
    Trigger_Transition_Func _trigger_transition_cb ;
    Trigger_Transition_Func _init_transition_cb ;

    HFSM_Base<TOP_STATE_T>* _hfsm;
};

template<class TOP_STATE_T, class PARENT_STATE_T, class USER_STATE_T, class... SUB_STATE_T>
template<class CUSTOM_STATE_T>
constexpr  Handling_Result Custom_State_Base<TOP_STATE_T, PARENT_STATE_T, USER_STATE_T, SUB_STATE_T...>::initial_transition_to_state(){
    _init_transition_cb = [this]() {

        //Check statically if this state is in our fsm
        static_assert(((_hfsm->_top_state).template seek_state<CUSTOM_STATE_T>()), "The state targeted by this transition does not belong to this state machine");

        //If the transition target is self
        static_assert(! std::is_same<CUSTOM_STATE_T, USER_STATE_T>::value, "Initial transition cannot target the state emitting it");
        //If the transition target is one of the substates
        static_assert(Super::template seek_state<CUSTOM_STATE_T>(), "Initial transition cannot target a parent state of the state emitting it");

        // Drill state hierarchy to find the target of the transition
        Super:: template  drill_state_hierarchy<CUSTOM_STATE_T>();
    };

    return Handling_Result::TRANSITION;

}


template<class TOP_STATE_T, class PARENT_STATE_T, class USER_STATE_T, class... SUB_STATE_T>
template<class CUSTOM_STATE_T>
constexpr  Handling_Result Custom_State_Base<TOP_STATE_T, PARENT_STATE_T, USER_STATE_T, SUB_STATE_T...>::trigger_transition() {
    // A lambda is used to store the transition callback. This allows to empty the stack filled by user code before engaging the transition.

    _trigger_transition_cb = [this]() {

        //Check statically if this state is in our fsm
        static_assert(((_hfsm->_top_state).template seek_state<CUSTOM_STATE_T>()), "The state targeted by this transition does not belong to this state machine");

        //If the transition target is self
        if constexpr (std::is_same<CUSTOM_STATE_T, USER_STATE_T>::value){

            static_cast<USER_STATE_T*>(this)->exit();
            static_cast<USER_STATE_T*>(this)->entry();

            if constexpr (Super::has_substates()) {
                static_cast<USER_STATE_T *>(this)->init();
                _init_transition_cb();
            }
            else{
                set_as_current_state();
            }
        }
        //If the transition target is one of the substates
        else if constexpr (Super::template seek_state<CUSTOM_STATE_T>()){
            Super:: template  drill_state_hierarchy<CUSTOM_STATE_T>();
        }
        //Else, then we ascend state hierarchy to find the state
        else {
            _parent_state->template ascend_state_hierarchy<CUSTOM_STATE_T>();
        }
        };

    return Handling_Result::TRANSITION;
}


template<class TOP_STATE_T, class PARENT_STATE_T, class USER_STATE_T, class... SUB_STATE_T>
void
Custom_State_Base<TOP_STATE_T, PARENT_STATE_T, USER_STATE_T, SUB_STATE_T...>::custom_state_base_handler( Event *event) {

    Handling_Result&& h_result = static_cast<USER_STATE_T*>(this) -> handler(event);

    if(h_result == Handling_Result::TRANSITION){
        //Exit all substates if there are ones
        if constexpr (Super::has_substates()){
            (*_subnstate_exit)();
        }
        _trigger_transition_cb();
    }

    else if(  h_result == Handling_Result::IGNORED){
        _state_exit = [this](){
            if constexpr (Super::has_substates()) {
                (*_subnstate_exit)();
            }
            static_cast<USER_STATE_T*>(this) -> exit();
        };
        _parent_state->handle_event_from_substate_b(event, &_state_exit);
    }


}

template<class TOP_STATE_T, class PARENT_STATE_T, class USER_STATE_T, class... SUB_STATE_T>
void Custom_State_Base<TOP_STATE_T, PARENT_STATE_T, USER_STATE_T, SUB_STATE_T...>::pass_ptrs_to_state(
        HFSM_Base<TOP_STATE_T> *hfsm, PARENT_STATE_T *parent_state) {
    _hfsm = hfsm;
    _parent_state = parent_state;

    // Pass hfsm ptr to substates only if they does exist
    if constexpr (std::tuple_size<Substates_Tuple_T>::value >0) {
        static_cast<Super*>(this)->pass_ptrs_to_substates(hfsm);
    }
}

/*----------------BASE CLASS FOR TOP STATE---------------- */
template<class TOP_STATE_T, class ...SUB_STATE_T >
class Top_State_Base : public State<TOP_STATE_T, Top_State_Base<TOP_STATE_T,SUB_STATE_T...>, TOP_STATE_T, SUB_STATE_T...> {
    using Super = State<TOP_STATE_T, Top_State_Base<TOP_STATE_T,SUB_STATE_T...>, TOP_STATE_T, SUB_STATE_T...>;
    using Substates_Tuple_T = std::tuple<SUB_STATE_T...>;

public:
    Top_State_Base(){};

    static Handling_Result custom_state_base_handler( Event* event) {
        return Handling_Result::IGNORED;
    }

    void handle_event_from_substate(Event* event, State_Exit_Func* substate_exit){
        Super::state_handler(event);
    }

    template<class CUSTOM_STATE_T>
     Handling_Result inline initial_transition_to_state(){
        static_assert(Super::template seek_state<CUSTOM_STATE_T>(), "The state targeted by this transition does not belong to this state machine");
        static_assert(!std::is_same<TOP_STATE_T,CUSTOM_STATE_T>::value, "Top state cannot be a target of state transition");

        Super:: template drill_state_hierarchy<CUSTOM_STATE_T>();
        return Handling_Result::TRANSITION;

    };

     void pass_ptrs_to_state(HFSM_Base<TOP_STATE_T>* hfsm){
        // Pass hfsm ptr to substates only if they does exist
        if constexpr (std::tuple_size<Substates_Tuple_T>::value >0) {
            Super ::pass_ptrs_to_substates(hfsm);
        }
    }

    template<class CUSTOM_STATE_T>
    inline void ascend_state_hierarchy(){
    Super:: template drill_state_hierarchy<CUSTOM_STATE_T>();
     }

};
#endif //H_FSM_STATES_H