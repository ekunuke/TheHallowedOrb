#include "Input.h"

Input::Input()
{
    reset();
}

Input::~Input()
{
    //dtor
}

bool Input::isHeld(Actions action)
{
    return keys[action];
}
bool Input::isPressed(Actions action)
{
    return keys[action] && !prevKeys[action];
}
bool Input::isReleased(Actions action)
{
    return !keys[action] && prevKeys[action];
}

void Input::step()
{
    if(!controllerChosen)
    {
        bool noButtonPressed = true;
        for(unsigned i = 0; i < 8; i++)
        {
            if(sf::Joystick::isConnected(i))
            {
                joystickDetected = true;
                unsigned numButtons = sf::Joystick::getButtonCount(i);
                maxJoystickButtons = numButtons;
                for(unsigned buttonId = 0; buttonId < numButtons; buttonId++)
                {
                    if(sf::Joystick::isButtonPressed(i, buttonId))
                    {
                        noButtonPressed = false;
                        if(usingJoystick == false ||
                           joystickId != i)
                        {
                            controllerChooseTimer = 0;
                        }
                        joystickId = i;
                        usingJoystick = true;
                        controllerChooseTimer++;
                    }
                }
            }
        }
        for(unsigned i = 0; i < sf::Keyboard::KeyCount; i++)
        {
            if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i))
            {
                noButtonPressed = false;
                if(usingJoystick == true)
                {
                    controllerChooseTimer = 0;
                }
                usingJoystick = false;
                controllerChooseTimer++;
            }
        }
        if(noButtonPressed)
        {
            controllerChooseTimer = 0;
        }
        if(controllerChooseTimer >= BIND_TIME)
        {
            controllerChooseTimer = BIND_TIME;
            controllerChosen = true;
            if(usingJoystick)
            {
                validAxes.clear();
                validAxes.push_back(sf::Joystick::X);
                validAxes.push_back(sf::Joystick::Y);
                validAxes.push_back(sf::Joystick::Z);
                validAxes.push_back(sf::Joystick::R);
                validAxes.push_back(sf::Joystick::U);
                validAxes.push_back(sf::Joystick::V);
                validAxes.push_back(sf::Joystick::PovX);
                validAxes.push_back(sf::Joystick::PovY);

                /// check valid axes
                for(unsigned i = 0; i < validAxes.size(); i++)
                {
                    if(!sf::Joystick::hasAxis(joystickId, validAxes[i]))
                    {
                        validAxes.erase(validAxes.begin() + i);
                        i--;
                    }
                }
                justSelected = true;
            }
        }
    }
    else if(binding)
    {
        bool allBinded = true;
        unsigned action;
        for(action = 0; action < binded.size(); action++)
        {
            if(!binded[action])
            {
                allBinded = false;
                currentlyBinding = action;
                //printf("Trying to bind %d\n", action);
                break;
            }
        }

        if(allBinded)
        {
            binding = false;
        }
        else
        {
            if(!usingJoystick)
            {
                int keysHeldCount = 0;
                sf::Keyboard::Key prevKey = currKey;
                for(unsigned i = 0; i < sf::Keyboard::KeyCount; i++)
                {
                    if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i))
                    {
                        keysHeldCount++;
                        currKey = (sf::Keyboard::Key)i;
                    }
                }
                if(keysHeldCount == 0)
                {
                    justSelected = false;
                }
                //printf("heldCount: %d, currKey: %d ?== prevKey: %d\n", keysHeldCount, (int)(currKey), (int)(prevKey));
                if(!justSelected && keysHeldCount == 1 && currKey == prevKey && currKey != lastKeyUsed)
                {
                    if(bindTimer < BIND_TIME)
                    {
                        bindTimer++;
                        if(bindTimer == BIND_TIME)
                        {
                            bindTimer = 0;
                            keyBindings[action] = currKey;
                            binded[action] = true;
                            lastKeyUsed = currKey;
                            //printf("Action %d binded to key %d!!!\n", action, currKey);
                        }
                    }
                }
                else
                {
                    bindTimer = 0;
                }
            }
            else
            {
                unsigned prevButton = currButton;

                int keysHeldCount = 0;
                for(unsigned i = 0; i < maxJoystickButtons; i++)
                {
                    if(sf::Joystick::isButtonPressed(joystickId, i))
                    {
                        keysHeldCount++;
                        currButton = i;
                        bindingAxis = false;
                    }
                }

                sf::Joystick::Axis lastAxis = currAxis;
                bool lastAxisPositive = currAxisPositive;

                for(unsigned i = 0; i < validAxes.size(); i++)
                {
                    float position = sf::Joystick::getAxisPosition(joystickId, validAxes[i]);
                    if(fabs(position) > 50)
                    {
                        bindingAxis = true;
                        keysHeldCount++;
                        currAxis = validAxes[i];
                        currAxisPositive = position > 0;
                    }
                }
//                if(justSelected)
//                {
//                    lastButtonUsed = currButton;
//                    lastAxisUsed = currAxis;
//                    lastAxisPositive = currAxisPositive;
//                    justSelected = false;
//                }
                if(keysHeldCount == 0)
                {
                    justSelected = false;
                }
                if(!justSelected && keysHeldCount == 1 &&
                   ((!bindingAxis && currButton == prevButton) ||
                   (bindingAxis && currAxis == lastAxis)) &&
                    (lastAxisUsed != currAxis || lastButtonUsed != currButton || lastAxisPositive != currAxisPositive))
                {
                    if(bindTimer < BIND_TIME)
                    {
                        bindTimer++;
                        if(bindTimer == BIND_TIME)
                        {
                            bindTimer = 0;
                            joystickBindings[action].usingAxis = bindingAxis;
                            joystickBindings[action].button = currButton;
                            joystickBindings[action].axis = currAxis;
                            joystickBindings[action].axisPositive = currAxisPositive;
                            binded[action] = true;


                            lastButtonUsed = currButton;
                            lastAxisUsed = currAxis;
                            lastAxisPositive = currAxisPositive;
                        }
                    }
                }
                else
                {
                    bindTimer = 0;
                }
            }
        }
    }
    else
    {
        if(!usingJoystick)
        {
            for(unsigned i = 0; i < prevKeys.size(); i++)
            {
                prevKeys[i] = keys[i];
                keys[i] = sf::Keyboard::isKeyPressed(keyBindings[i]);
            }
        }
        else
        {
            for(unsigned i = 0; i < prevKeys.size(); i++)
            {
                prevKeys[i] = keys[i];
                if(!joystickBindings[i].usingAxis)
                {
                    keys[i] = sf::Joystick::isButtonPressed(joystickId, joystickBindings[i].button);
                }
                else
                {
                    float pos = sf::Joystick::getAxisPosition(joystickId, joystickBindings[i].axis);
                    if(joystickBindings[i].axisPositive)
                    {
                        keys[i] = pos > 50;
                    }
                    else
                    {
                        keys[i] = pos < -50;
                    }
                }
            }
        }
    }
}

void Input::reset()
{
    currentlyBinding = 0;
    joystickDetected = false;
    keyBindings.resize(NUM_ACTIONS, sf::Keyboard::Space);
    joystickBindings.resize(NUM_ACTIONS, JoystickEvent());
    binded.resize(NUM_ACTIONS, false);
    keys.resize(NUM_ACTIONS, false);
    prevKeys.resize(NUM_ACTIONS, false);
    bindTimer = 0;
    binding = true;
    usingJoystick = false;
    controllerChosen = false;
    controllerChooseTimer = 0;
}
