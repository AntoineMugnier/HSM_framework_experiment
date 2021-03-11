#pragma once

#include "tuple"
#include "functional"
#include <type_traits>
#include <cstdint>

enum struct Signal{A,B,C,D,E,F,G,H,I};
enum struct Handling_Result{HANDLED, IGNORED, TRANSITION};


struct Event{
    Event(Signal sig):_sig(sig){};
    Signal _sig;
};

using Handler_Func = std::function <void(const Event* const)>;
using State_Exit_Func = std::function <void(void)>;
using Subnstate_Exit_Func = std::function <void(void)>;
using Trigger_Transition_Func = std::function <void(void)>;

template<class USER_HFSM_T, class PARENT_STATE_T, class USER_STATE_T, class ...SUB_STATE_T>
class Custom_State_Base;

struct Event;

/*----------------BASE STATE CLASS---------------- */
template<class USER_HFSM_T, class USER_STATE_BASE_T, class USER_STATE_T, class ...SUB_STATE_T>
class State {
    using Substates_Tuple_T = std::tuple<SUB_STATE_T...>;

public:

    Handler_Func state_handler = [this](const Event *const event) {
        static_cast<USER_STATE_BASE_T *>(this)->custom_state_base_handler(event);
    };

    template<class HEAD_STATE_T, class NECK_STATE_T, class... OTHER_STATE_T, class SETUP_PTR_T>
    void state_setup(SETUP_PTR_T setup_ptr) {
        HEAD_STATE_T &tuple_elmt = std::get<HEAD_STATE_T>(_substates);
        tuple_elmt.template state_setup<NECK_STATE_T, OTHER_STATE_T...>(setup_ptr);
    };

    template<class TAIL_STATE_T, class SETUP_PTR_T>
    void state_setup(SETUP_PTR_T setup_ptr) {
        TAIL_STATE_T &tuple_elmt = std::get<TAIL_STATE_T>(_substates);
        tuple_elmt.template custom_state_base_setup(setup_ptr);
    };


    template<class CUSTOM_STATE_T>
    inline void drill_state_hierarchy() {
        if constexpr (std::tuple_size<Substates_Tuple_T>::value > 0) { // "If" statement not necessary?
            drill_into_substates<CUSTOM_STATE_T>();
        }

    }

    bool static constexpr has_substates() {
        if constexpr (std::tuple_size<Substates_Tuple_T>::value > 0) {
            return true;
        } else {
            return false;
        }
    }

/**
 * @brief Method called after the last recursive call of the namesake template method.
 * It does nothing, it serves just to end this series of calls.
 * @param hfsm Pointer to the final state machine object
 */
    void pass_ptrs_to_substates(USER_HFSM_T *hfsm,
                                std::integral_constant<int, std::tuple_size<std::tuple<SUB_STATE_T...>>::value>);

    /**
     * @brief Recursive method, allows to dispatch the hfsm pointer to all the substates of the state.
     * @tparam index Index of the tuple element in tuple
     * @param hfsm Pointer to the final state machine object
     */
    template<int index = 0>
    void
    pass_ptrs_to_substates(USER_HFSM_T *hfsm, std::integral_constant<int, index> = std::integral_constant<int, 0>());

    template<class CUSTOM_STATE_T>
    static inline constexpr bool
    seek_state_in_substates(std::integral_constant<int, std::tuple_size<std::tuple<SUB_STATE_T...>>::value>);

    template<class CUSTOM_STATE_T, int index = 0>
    static inline constexpr bool
    seek_state_in_substates(std::integral_constant<int, index> = std::integral_constant<int, 0>());

    template<class CUSTOM_STATE_T>
    inline void drill_into_substates(std::integral_constant<int, std::tuple_size<std::tuple<SUB_STATE_T...>>::value>) {
        // Error, we should never reach this point
    };

    template<class CUSTOM_STATE_T, int index = 0>
    inline void drill_into_substates(std::integral_constant<int, index> = std::integral_constant<int, 0>()) {

        // Catch state at index in the tuple
        typedef class std::tuple_element<index, Substates_Tuple_T>::type substate_t;

        substate_t &substate = std::get<index>(_substates);

        //If this state is the state we seek
        if constexpr (std::is_same<substate_t, CUSTOM_STATE_T>::value) {
            // if the target state has substates, enter it and start initial transition
            if constexpr (substate_t::has_substates()) {
                substate.entry();
                substate.init();
                substate._init_transition_cb();
            } else {
                //if the target state does not have substates, enter it and set it as the current state
                substate.set_as_current_state();
                substate.entry();
            }
        }

            // If the state we seek is a substate of this state, then enter it and continue drilling
        else if constexpr(substate.template seek_state<CUSTOM_STATE_T>()) {
            substate.entry();
            substate.template drill_state_hierarchy<CUSTOM_STATE_T>();
        }

            // if this state does not have the state we seek, let's move to the next one in the tuple
        else {
            drill_into_substates<CUSTOM_STATE_T>(std::integral_constant<int, index + 1>());
        }


    };


    template<class CUSTOM_STATE_T>
    static inline constexpr bool seek_state() {
        if constexpr (std::tuple_size<Substates_Tuple_T>::value > 0) {
            return seek_state_in_substates<CUSTOM_STATE_T>();
        } else return false;
    }

    void handle_event_from_substate_b(const Event *const event, State_Exit_Func *substate_exit) {
        static_cast<USER_STATE_BASE_T *>(this)->handle_event_from_substate(event, substate_exit);
    }


    USER_HFSM_T *_hfsm;

protected:
    State() {};
    Substates_Tuple_T _substates;

};


template<class USER_HFSM_T, class USER_STATE_BASE_T, class USER_STATE_T, class... SUB_STATE_T>
void State<USER_HFSM_T, USER_STATE_BASE_T, USER_STATE_T, SUB_STATE_T...>::pass_ptrs_to_substates(USER_HFSM_T *hfsm,
                                                                                                 std::integral_constant<int, std::tuple_size<std::tuple<SUB_STATE_T...>>::value>) {
// Do nothing, all tuple elements have received the pointer
}

template<class USER_HFSM_T, class USER_STATE_BASE_T, class USER_STATE_T, class... SUB_STATE_T>
template<int index>
void State<USER_HFSM_T, USER_STATE_BASE_T, USER_STATE_T, SUB_STATE_T...>::pass_ptrs_to_substates(USER_HFSM_T *hfsm,
                                                                                                 std::integral_constant<int, index>) {

    std::get<index>(_substates).pass_ptrs_to_state(hfsm, static_cast<USER_STATE_T *>(this));
    pass_ptrs_to_substates(hfsm, std::integral_constant<int, index + 1>());

}


template<class USER_HFSM_T, class USER_STATE_BASE_T, class USER_STATE_T, class... SUB_STATE_T>
template<class CUSTOM_STATE_T>
constexpr bool State<USER_HFSM_T, USER_STATE_BASE_T, USER_STATE_T, SUB_STATE_T...>::seek_state_in_substates(
        std::integral_constant<int, std::tuple_size<std::tuple<SUB_STATE_T...>>::value>) {
// Do nothing, all tuple elements have been visited
    return false;
}

template<class USER_HFSM_T, class USER_STATE_BASE_T, class USER_STATE_T, class... SUB_STATE_T>
template<class CUSTOM_STATE_T, int index>
constexpr bool State<USER_HFSM_T, USER_STATE_BASE_T, USER_STATE_T, SUB_STATE_T...>::seek_state_in_substates(
        std::integral_constant<int, index>) {

    //Determinate if this substate is the state type we seek
    if constexpr (std::is_same<class std::tuple_element<index, Substates_Tuple_T>::type, CUSTOM_STATE_T>::value) {
        return true;
    }

        // If not, we search for the state type in all of its sub(n)states
    else if constexpr (std::tuple_element<index, Substates_Tuple_T>::type::template seek_state<CUSTOM_STATE_T>()) return true;

    else {
        return seek_state_in_substates<CUSTOM_STATE_T>(std::integral_constant<int, index + 1>());
    }
}


/*----------------BASE CLASS FOR USER STATES---------------- */
template<class USER_HFSM_T, class PARENT_STATE_T, class USER_STATE_T, class ...SUB_STATE_T>
class Custom_State_Base
        : public State<USER_HFSM_T, Custom_State_Base<USER_HFSM_T, PARENT_STATE_T, USER_STATE_T, SUB_STATE_T...>, USER_STATE_T, SUB_STATE_T ...> {

    friend USER_HFSM_T;
    using Super = State<USER_HFSM_T, Custom_State_Base<USER_HFSM_T, PARENT_STATE_T, USER_STATE_T, SUB_STATE_T...>, USER_STATE_T, SUB_STATE_T ...>;
    using Substates_Tuple_T = std::tuple<SUB_STATE_T...>;
    using Self = Custom_State_Base<USER_HFSM_T, PARENT_STATE_T, USER_STATE_T, SUB_STATE_T...>;

public:

    Custom_State_Base() {};

    void custom_state_base_handler(const Event *const event);

    template<class CUSTOM_STATE_T>
    constexpr Handling_Result trigger_transition();

    void set_as_current_state() {
        Super::_hfsm->_current_state_h = &(Super::state_handler);
    }

    template<class SETUP_PTR_T>
    void custom_state_base_setup(SETUP_PTR_T setup_ptr) {
        static_cast<USER_STATE_T *>(this)->setup(setup_ptr);
    };

    void pass_ptrs_to_state(USER_HFSM_T *hfsm, PARENT_STATE_T *parent_state);;

    void handle_event_from_substate(const Event *const event, State_Exit_Func *substate_exit) {

        _subnstate_exit = substate_exit;

        Super::state_handler(event);

    }

    template<class CUSTOM_STATE_T>
    inline void ascend_state_hierarchy() {
        // If this substate type is the state type we seek
        if constexpr  (std::is_same<CUSTOM_STATE_T, USER_STATE_T>::value) {
            static_cast<USER_STATE_T *>(this)->init();
            _init_transition_cb();
        } else if constexpr (Super::template seek_state<CUSTOM_STATE_T>()) {
            Super::template drill_state_hierarchy<CUSTOM_STATE_T>();
        } else {
            static_cast<USER_STATE_T *>(this)->exit();
            _parent_state->template ascend_state_hierarchy<CUSTOM_STATE_T>();
        }
    }

    template<class CUSTOM_STATE_T>
    constexpr Handling_Result initial_transition_to_state();

//protected: TODO remove ?
    PARENT_STATE_T *_parent_state;
    State_Exit_Func _state_exit = [=]() { static_cast<USER_STATE_T *>(this)->exit(); };
    Subnstate_Exit_Func *_subnstate_exit;
    Trigger_Transition_Func _trigger_transition_cb;
    Trigger_Transition_Func _init_transition_cb;

};

template<class USER_HFSM_T, class PARENT_STATE_T, class USER_STATE_T, class... SUB_STATE_T>
template<class CUSTOM_STATE_T>
constexpr Handling_Result
Custom_State_Base<USER_HFSM_T, PARENT_STATE_T, USER_STATE_T, SUB_STATE_T...>::initial_transition_to_state() {
    _init_transition_cb = [this]() {

        //Check statically if this state is in our fsm
        static_assert(((Super::_hfsm->_top_state).template seek_state<CUSTOM_STATE_T>()),
                      "The state targeted by this transition does not belong to this state machine");

        //If the transition target is self
        static_assert(!std::is_same<CUSTOM_STATE_T, USER_STATE_T>::value,
                      "Initial transition cannot target the state emitting it");
        //If the transition target is one of the substates
        static_assert(Super::template seek_state<CUSTOM_STATE_T>(),
                      "Initial transition cannot target a parent state of the state emitting it");

        // Drill state hierarchy to find the target of the transition
        Super::template drill_state_hierarchy<CUSTOM_STATE_T>();
    };

    return Handling_Result::TRANSITION;

}


template<class USER_HFSM_T, class PARENT_STATE_T, class USER_STATE_T, class... SUB_STATE_T>
template<class CUSTOM_STATE_T>
constexpr Handling_Result
Custom_State_Base<USER_HFSM_T, PARENT_STATE_T, USER_STATE_T, SUB_STATE_T...>::trigger_transition() {
    // A lambda is used to store the transition callback. This allows to empty the stack filled by user code before engaging the transition.

    _trigger_transition_cb = [this]() {

        //Check statically if this state is in our fsm
        static_assert(((Super::_hfsm->_top_state).template seek_state<CUSTOM_STATE_T>()),
                      "The state targeted by this transition does not belong to this state machine");

        //If the transition target is self
        if constexpr (std::is_same<CUSTOM_STATE_T, USER_STATE_T>::value) {

            static_cast<USER_STATE_T *>(this)->exit();
            static_cast<USER_STATE_T *>(this)->entry();

            if constexpr (Super::has_substates()) {
                static_cast<USER_STATE_T *>(this)->init();
                _init_transition_cb();
            } else {
                set_as_current_state();
            }
        }
            //If the transition target is one of the substates
        else if constexpr (Super::template seek_state<CUSTOM_STATE_T>()) {
            Super::template drill_state_hierarchy<CUSTOM_STATE_T>();
        }
            //Else, then we ascend state hierarchy to find the state
        else {
            _parent_state->template ascend_state_hierarchy<CUSTOM_STATE_T>();
        }
    };

    return Handling_Result::TRANSITION;
}


template<class USER_HFSM_T, class PARENT_STATE_T, class USER_STATE_T, class... SUB_STATE_T>
void
Custom_State_Base<USER_HFSM_T, PARENT_STATE_T, USER_STATE_T, SUB_STATE_T...>::custom_state_base_handler(
        const Event *const event) {

    Handling_Result &&h_result = static_cast<USER_STATE_T *>(this)->handler(event);

    if (h_result == Handling_Result::TRANSITION) {
        //Exit all substates if there are ones
        if constexpr (Super::has_substates()) {
            (*_subnstate_exit)();
        }
        _trigger_transition_cb();
    } else if (h_result == Handling_Result::IGNORED) {
        _state_exit = [this]() {
            if constexpr (Super::has_substates()) {
                (*_subnstate_exit)();
            }
            static_cast<USER_STATE_T *>(this)->exit();
        };
        _parent_state->handle_event_from_substate_b(event, &_state_exit);
    }


}

template<class USER_HFSM_T, class PARENT_STATE_T, class USER_STATE_T, class... SUB_STATE_T>
void Custom_State_Base<USER_HFSM_T, PARENT_STATE_T, USER_STATE_T, SUB_STATE_T...>::pass_ptrs_to_state(
        USER_HFSM_T *hfsm, PARENT_STATE_T *parent_state) {
    Super::_hfsm = hfsm;
    _parent_state = parent_state;

    // Pass hfsm ptr to substates only if they does exist
    if constexpr (std::tuple_size<Substates_Tuple_T>::value > 0) {
        static_cast<Super *>(this)->pass_ptrs_to_substates(hfsm);
    }
}

/*----------------BASE CLASS FOR TOP STATE---------------- */
template<class USER_HFSM_T, class TOP_STATE_T, class ...SUB_STATE_T>
class Top_State_Base
        : public State<USER_HFSM_T, Top_State_Base<USER_HFSM_T, TOP_STATE_T, SUB_STATE_T...>, TOP_STATE_T, SUB_STATE_T...> {
    using Super = State<USER_HFSM_T, Top_State_Base<USER_HFSM_T, TOP_STATE_T, SUB_STATE_T...>, TOP_STATE_T, SUB_STATE_T...>;
    using Substates_Tuple_T = std::tuple<SUB_STATE_T...>;

public:
    Top_State_Base() {};

    static Handling_Result custom_state_base_handler(const Event *const event) {
        return Handling_Result::IGNORED;
    }

    void handle_event_from_substate(const Event *const event, State_Exit_Func *substate_exit) {
        Super::state_handler(event);
    }

    template<class CUSTOM_STATE_T>
    Handling_Result inline initial_transition_to_state() {
        static_assert(Super::template seek_state<CUSTOM_STATE_T>(),
                      "The state targeted by this transition does not belong to this state machine");
        static_assert(!std::is_same<TOP_STATE_T, CUSTOM_STATE_T>::value,
                      "Top state cannot be a target of state transition");

        Super::template drill_state_hierarchy<CUSTOM_STATE_T>();
        return Handling_Result::TRANSITION;

    };

    void pass_ptrs_to_state(USER_HFSM_T *hfsm) {
        // Pass hfsm ptr to substates only if they does exist
        Super::_hfsm = hfsm;
        if constexpr (std::tuple_size<Substates_Tuple_T>::value > 0) {
            Super::pass_ptrs_to_substates(hfsm);
        }
    }

    template<class CUSTOM_STATE_T>
    inline void ascend_state_hierarchy() {
        Super::template drill_state_hierarchy<CUSTOM_STATE_T>();
    }

};


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
    HFSM_Base();

};

template<class USER_HFSM, class TOP_STATE_T>
HFSM_Base<USER_HFSM, TOP_STATE_T>::HFSM_Base() {
    _top_state.pass_ptrs_to_state(static_cast<USER_HFSM*>(this)); // Pass the this pointer to all substates
    _top_state.init();
};
