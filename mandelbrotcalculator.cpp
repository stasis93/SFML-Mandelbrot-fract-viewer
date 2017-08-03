#include "mandelbrotcalculator.h"

MandelbrotCalculator::MandelbrotCalculator(int calcWidth,
                                           int calcHeight,
                                           int maxIter)
    : m_width(calcWidth),
      m_height(calcHeight)
{
    resize(calcWidth, calcHeight);
    setMaxIter(maxIter);

    m_numThreads = std::thread::hardware_concurrency();
    m_numThreads = m_numThreads == 0 ? 1 : m_numThreads;

    std::cout << "Going to run with " << m_numThreads << " thread(s)..." << std::endl;
}


void MandelbrotCalculator::calculate(long double xCenter,
                                     long double yCenter,
                                     long double width,
                                     long double height)
{
    long double dx = width / m_width;
    long double dy = height / m_height;

    std::thread t[m_numThreads];

    int chunkSize = m_height / m_numThreads;
    int tail = m_height - m_numThreads * chunkSize;

    auto func = [=](int id)
    {
        int yStart = chunkSize * id;
        int yEnd = yStart + chunkSize;
        yEnd = yEnd > m_height ? m_height : yEnd;

        for (auto y = yStart; y < yEnd; y++)
            for (auto x = 0; x < m_width; x++) {
                long double z_re = 0.0L, z_im = 0.0L;
                long double c_im = yCenter - height / 2.0L + dy * y;
                long double c_re = xCenter - width  / 2.0L + dx * x;

                double mod2 = 0L;
                int iter = 0;

                for (;;) {
                    // Z <- Z^2 + c
                    long double z_re_new = (z_re * z_re - z_im * z_im) + c_re;
                    long double z_im_new = (2 * z_re * z_im) + c_im;
                    z_re = z_re_new;
                    z_im = z_im_new;

                    mod2 = z_re * z_re + z_im * z_im;
                    if (iter == m_iterLimit || mod2 >= 4.0L)
                        break;
                    iter++;
                }
                m_iterField[m_height - y - 1][x] = iter;
            }
    };

    for (auto i = 0; i < m_numThreads; i++)
        t[i] = std::thread(func, i);

    if (tail > 0)
        func(m_numThreads);

    for (auto i = 0; i < m_numThreads; i++)
        t[i].join();
}



void MandelbrotCalculator::resize(int newWidth, int newHeight)
{
    newWidth  = newWidth  < 300 ? 300 : newWidth;
    newHeight = newHeight < 300 ? 300 : newHeight;

    m_iterField.resize(newHeight);
    for (auto& row : m_iterField)
        row.resize(newWidth);

    m_width = newWidth;
    m_height = newHeight;
}



void MandelbrotCalculator::setMaxIter(int newMaxIter)
{
    m_iterLimit = newMaxIter >= 1 ? newMaxIter : 1;
}
