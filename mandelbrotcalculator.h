#ifndef MANDELBROTCALCULATOR_H
#define MANDELBROTCALCULATOR_H

#include <vector>
#include <thread>
#include <iostream>

class MandelbrotCalculator
{
public:
    using Field =       std::vector<std::vector<int>>;

                        MandelbrotCalculator(int getWidth,
                                             int getHeight,
                                             int maxIter);

    void                setMaxIter(int newMaxIter);
    void                resize(int newWidth,
                               int newHeight);
    void                calculate(long double xCenter,
                                  long double yCenter,
                                  long double width,
                                  long double height);

    const Field&        getField()      const { return m_iterField; }


    int                 maxIter()      const { return m_iterLimit; }
    int                 getWidth()     const { return m_width; }
    int                 getHeight()    const { return m_height; }


private:
    int                 m_width;
    int                 m_height;
    int                 m_iterLimit;
    int                 m_numThreads;

    Field               m_iterField;
};

#endif // MANDELBROTCALCULATOR_H
