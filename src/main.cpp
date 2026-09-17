#include <iostream>
#include <string>

using namespace std;

enum class State
{
    Start,

    AOdd,
    AEven,

    BOdd,
    BEven,

    CMod1,
    CMod2,
    CMod0,

    Error
};

class FiniteAutomaton
{
public:
    bool Recognize(const string& word) const
    {
        State state = State::Start;

        for (char character : word)
        {
            state = Transition(state, character);

            if (state == State::Error)
                return false;
        }

        return IsAcceptingState(state);
    }

private:
    State Transition(State state, char character) const
    {
        switch (state)
        {
            case State::Start:
            {
                if (character == 'a')
                    return State::AOdd;

                return State::Error;
            }

            case State::AOdd:
            {
                if (character == 'a')
                    return State::AEven;

                return State::Error;
            }

            case State::AEven:
            {
                if (character == 'a')
                    return State::AOdd;

                if (character == 'b')
                    return State::BOdd;

                return State::Error;
            }

            case State::BOdd:
            {
                if (character == 'b')
                    return State::BEven;

                return State::Error;
            }

            case State::BEven:
            {
                if (character == 'b')
                    return State::BOdd;

                if (character == 'c')
                    return State::CMod1;

                return State::Error;
            }

            case State::CMod1:
            {
                if (character == 'c')
                    return State::CMod2;

                return State::Error;
            }

            case State::CMod2:
            {
                if (character == 'c')
                    return State::CMod0;

                return State::Error;
            }

            case State::CMod0:
            {
                if (character == 'c')
                    return State::CMod1;

                return State::Error;
            }

            case State::Error:
                return State::Error;
        }

        return State::Error;
    }

    bool IsAcceptingState(State state) const
    {
        return state == State::CMod0;
    }
};

int main()
{
    FiniteAutomaton automaton;
    string word;

    cout << "Variant 18: a^n b^m c^k, \n";
    cout << "Conditions:\n";
    cout << "- n >= 1\n";
    cout << "- m >= 2\n";
    cout << "- k >= 3\n";
    cout << "- n is even\n";
    cout << "- m is even\n";
    cout << "- k is divisible by 3\n\n";
    cout << "Enter word: ";
    cin >> word;

    if (automaton.Recognize(word))
        cout << "Word belongs to the language.\n";
    else
        cout << "Word does not belong to the language.\n";

    return 0;
}
