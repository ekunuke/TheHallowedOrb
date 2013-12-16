#ifndef INPUT_H
#define INPUT_H

#include "toolbox.h"

class Input
{
    public:
        enum Actions
        {
            ACTION_LEFT = 0,
            ACTION_DOWN = 1,
            ACTION_RIGHT = 2,
            ACTION_JUMP = 3,
            ACTION_SHOOT = 4,
            NUM_ACTIONS = 5
        };
        struct JoystickEvent
        {
            unsigned button;
            bool usingAxis;
            sf::Joystick::Axis axis;
            bool axisPositive;
            JoystickEvent():
                button(0),
                usingAxis(false),
                axis(sf::Joystick::X),
                axisPositive(false)
                {
                }
        };
        static const int BIND_TIME = 40;

        bool isHeld(Actions action);
        bool isPressed(Actions action);
        bool isReleased(Actions action);

        vector<sf::Keyboard::Key> keyBindings;
        vector<JoystickEvent> joystickBindings;

        vector<bool> binded;
        vector<bool> keys;
        vector<bool> prevKeys;

        bool controllerChosen;
        bool binding;

        int bindTimer;
        int currentlyBinding;

        sf::Keyboard::Key currKey;
        sf::Keyboard::Key lastKeyUsed;

        unsigned currButton;
        sf::Joystick::Axis currAxis;
        bool currAxisPositive;
        bool bindingAxis;

        unsigned lastButtonUsed;
        sf::Joystick::Axis lastAxisUsed;
        bool lastAxisPositive;

        bool justSelected;

        vector<sf::Joystick::Axis> validAxes;


        bool joystickDetected;
        int controllerChooseTimer;
        bool usingJoystick;
        unsigned joystickId; // 0 - 7
        unsigned maxJoystickButtons; // Maximum number of buttons joystick supports

        void step();
        void reset();

        Input();
        ~Input();
};

#endif // INPUT_H
