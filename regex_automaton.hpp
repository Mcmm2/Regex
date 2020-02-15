#include<stack>
#include<iostream>
#include<vector>

// Funções que precisam ser implementadas:
// is_op(Operation&)
// priority(Operation&)
// process_op(Automatas&, Operation&)
// Classe nfa


typedef long StateId;

enum OpCode{
    opcode_alternative,
    opcode_repeat,
    opcode_match,
    opcode_accept
};

struct State
{
    OpCode      opcode;
    StateId     next;
    union
    {
        StateId     alt;     //Opcode = Alternation or Repetition
        char        ch;      //Opcode = Match
    };  
    State(OpCode _opcode) : opcode(_opcode) {}
};

struct NFA : std::vector<State>
{   
    StateId                 start_state;    

    StateId insert_accept()
    {
        return insert_state(State(opcode_accept));
    }

    StateId insert_alt(StateId _next, StateId _alt)
    {
        State tmp(opcode_alternative);
        tmp.next = _next;
        tmp.alt = _alt;
        return insert_state(tmp);
    }

    StateId insert_repeat(StateId _next, StateId _alt)
    {
        State tmp(opcode_repeat);
        tmp.next = _next;
        tmp.alt = _alt;
        return insert_state(tmp);
    }

    StateId insert_matcher(char _ch)
    {
        State tmp(opcode_match);
        tmp.ch = _ch;
        return insert_state(tmp);
    }

    StateId insert_state(State _s)
    {
        this->push_back(_s);
        return this->size()-1;
    }
};

struct NFA_Fragment
{
    typedef NFA Regex;

    Regex&                  nfa;
    StateId                 start;
    StateId                 end;
    std::vector<StateId>    dangling;

    friend void patch(NFA_Fragment& _frag, StateId _s)
    {
        for(auto it : _frag.dangling)
        {
            _frag.nfa[it].next = _s;
        }
    }

    friend std::vector<StateId> append(NFA_Fragment& _frag1, NFA_Fragment& _frag2)
    {
        std::vector<StateId> tmp(_frag1.dangling);
        for(auto it : _frag2.dangling)
        {
            tmp.push_back(it);
        }
        return tmp;
    }
};

//Okay, agr precisamos fazer um Parser para criar nosso NFA
