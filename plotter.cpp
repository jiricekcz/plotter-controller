#include <iostream>
#include <functional>
namespace AxisControll
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
        int dx = MotorX::distToSteps(x) - MotorX::position;
        int dy = MotorY::distToSteps(y) - MotorY::position;

        int max = dx > dy ? dx : dy;
        int min = dx > dy ? dy : dx;

        std::function<void()> moveMax = dx > dy ? MotorX::moveStep : MotorY::moveStep;
        std::function<void()> moveMin = dx > dy ? MotorY::moveStep : MotorX::moveStep;

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
int main()
{
    AxisControll::moveTo(100, 5, 0);
    return 0;
}
