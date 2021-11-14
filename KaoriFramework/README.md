# What is a state machine? 
A finite state machine (FSM) if the decomposition of the behaviour of a system into a finite number of states. 
The state machine can receive signals of different natures, which will trigger the execution of different actions 
depending on the current state of the state machine. An action can lead to the transition from a state to another.
A FSM starts in an initial state, realizing its entry action. Each time a state is entered, its special entry action is performed.
Also, each time a state is exited, its special exit action if performed.

A hierarchical state machine is a final state machine with states being able to be sub-states of others
Initial transition is a special transition performed when

# About the Kaori Framework 
Kaori is a fully object-oriented C++17 framework that allows you to develop state machines following UML 2.5 standard.
States are objects encapsulated into the state machine, therefore not accessible by user.
The framework makes extensive use of template metaprograming to maximize the "understanding" of your state machine architecture by the compiler.
This aims to shorten the transitions between states and inform your early in the development of your application of
the errors you may do in the implementation of your state machine.

# How to use it
***Note**: The qhsmtst state machine diagram from Miro Samek book Practical UML statecharts 2nd edition has been 
implemented using the kaori framework. You can find it in the `examples` directory of this project.*

1. Define your own HSM, top state and states types which will derive from the framework base types.
You may create a .cpp/.hpp files pair where these custom types are defined:
2. Implement the callback methods in each of your custom states:
   - `exit()` and `entry()` methods are called when exiting and entering a state.
   - `init()` method is called when performing an initial transition from this state to one of its substates.`initial_transition_to_state()` can be called to choose the target state of the initial transition.
   - `handler()` method is called when receiving a signal into this state.`trigger_transition()` can be called at the end of the `handler()` method to trigger the transition to another state.
  User_derived top state does not have a `handler()` method but the `init()` one which is called when initializing the HSM.
     
# Current issues and possible improvements
- The major defect of this framework is the extensive use of `std::function` type for allowing the different `handler()` methods to be called by the `HSM_Base`.
- Top state currently exists for technical purpose but it may be removed to simplify the use of this framework
