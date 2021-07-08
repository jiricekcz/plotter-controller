#include <iostream>
#include <functional>
#include <cstring>
namespace AxisControl
{
    const float PI = 3.141592653589793;
    const int setpsPerRotation = 64;

    namespace MotorY
    {
        const float radius = 1;
        const float axisLength = 400;

        const float rotationDistance = 2 * PI * radius;
        const float stepsPerMillimeter = setpsPerRotation / rotationDistance;
        const int axisSteps = axisLength / rotationDistance * setpsPerRotation;
        int position = 0;

        inline int distToSteps(float dist)
        {
            return dist * stepsPerMillimeter;
        }

        inline void moveStep()
        {
            position++;
            std::cout << "Y: " << position << " ";
        }
        inline void moveStepBack()
        {
            position--;
            std::cout << "Y: " << position << " ";
        }
    }
    namespace MotorX
    {
        const float radius = 1;
        const float axisLength = 280;

        const float rotationDistance = 2 * PI * radius;
        const float stepsPerMillimeter = setpsPerRotation / rotationDistance;
        const int axisSteps = axisLength / rotationDistance * setpsPerRotation;
        int position = 0;

        inline int distToSteps(float dist)
        {
            return dist * stepsPerMillimeter;
        }

        inline void moveStep()
        {
            position++;
            std::cout << "X: " << position << " ";
        }
        inline void moveStepBack()
        {
            position--;
            std::cout << "X: " << position << " ";
        }

    }

    void moveTo(float x, float y, float speed)
    {
        int dxs = MotorX::distToSteps(x) - MotorX::position;
        int dys = MotorY::distToSteps(y) - MotorY::position;
        int dy = abs(dys);
        int dx = abs(dxs);

        int max = dx > dy ? dx : dy;
        int min = dx > dy ? dy : dx;

        std::function<void()> moveMax = dx > dy ? dxs > 0 ? MotorX::moveStep : MotorX::moveStepBack : dys > 0 ? MotorY::moveStep
                                                                                                              : MotorY::moveStepBack;
        std::function<void()> moveMin = dx > dy ? dys > 0 ? MotorY::moveStep : MotorY::moveStepBack : dxs > 0 ? MotorX::moveStep
                                                                                                              : MotorX::moveStepBack;

        float stepsPerMove = max / min;
        float currSteps = 0;
        for (int i = 0; i < max; i++)
        {
            moveMax();
            currSteps++;

            if (currSteps >= stepsPerMove)
            {
                moveMin();
                currSteps -= stepsPerMove;
            }
        }
    }
}
namespace InstructionReader
{
    union char_int
    {
        char chars[4];
        uint32_t num;
    };
    uint32_t composeNumberFromChars(char char1, char char2, char char3, char char4)
    {
        union char_int tmp;
        tmp.chars[0] = char1;
        tmp.chars[1] = char2;
        tmp.chars[2] = char3;
        tmp.chars[3] = char4;
        return tmp.num;
    }

    void moveTo(float x, float y)
    {
        std::cout << "goto " << x << " " << y << "\n";
    }
    void push()
    {
        std::cout << "push\n";
    }
    void pull()
    {
        std::cout << "pull\n";
    }
    void home()
    {
        std::cout << "home\n";
    }
    void end()
    {
        std::cout << "end\n";
    }
    void speed(uint8_t s)
    {
        std::cout << "push " << s << "\n";
    }

    void handleInstructions(uint8_t *instructions, int len)
    {
        for (int pointer = 0; pointer < len; ++pointer)
        {
            uint8_t c = instructions[pointer];
            switch (c)
            {
            case 0x20:
            {
                uint32_t x = composeNumberFromChars(instructions[pointer + 1], instructions[pointer + 2], instructions[pointer + 3], instructions[pointer + 4]);
                pointer += 4;
                uint32_t y = composeNumberFromChars(instructions[pointer + 1], instructions[pointer + 2], instructions[pointer + 3], instructions[pointer + 4]);
                pointer += 4;
                moveTo(float(x) / 10000, float(y) / 10000);
                break;
            }
            case 0x1F:
            {
                home();
                break;
            }
            case 0xF0:
            {
                push();
                break;
            }
            case 0xF1:
            {
                pull();
                break;
            }
            case 0xAA:
            {
                end();
                break;
            }
            case 0xB0:
            {
                uint8_t s = (uint8_t)instructions[pointer + 1];
                pointer++;
                speed(s);
            }
            break;
            }
        }
    }
}
int main()
{
    AxisControl::moveTo(5, 5, 0);
    AxisControl::moveTo(4, 4, 0);

    uint8_t i[] = {0x20, 0xff, 0x57, 0x0a, 0x00, 0xe7, 0xbb, 0x0b, 0x00, 0xf0, 0xf1, 0x20, 0xa0, 0x86, 0x01, 0x00, 0xa0, 0x86, 0x01, 0x00, 0xf0, 0x20, 0x40, 0x42, 0x0f, 0x00, 0x40, 0x42, 0x0f, 0x00, 0xf1};
    // InstructionReader::handleInstructions(i, 31);
    return 0;
}
