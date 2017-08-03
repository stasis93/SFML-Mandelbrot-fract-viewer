#ifndef SFML_VISUALIZER_H
#define SFML_VISUALIZER_H

#include <iostream>
#include <iomanip>
#include <sstream>

#include <SFML/Graphics.hpp>
#include "mandelbrotcalculator.h"
#include "view.h"

class SFML_Visualizer
{
public:
                                SFML_Visualizer();
    void                        run();


private:
    void                        processEvents();
    void                        update();
    void                        render();
    void                        handleKeyboardInput(sf::Keyboard::Key code, bool shift);
    void                        handleMouseInput(float delta);
    void                        handleMouseInput(sf::Mouse::Button btn, int x, int y);
    void                        handleResize(int newWidth, int newHeight);
    void                        initPalette();


private:

    sf::RenderWindow            m_window;
    sf::Image                   m_fractImage;
    sf::Sprite                  m_fractSprite;
    sf::Texture                 m_fractTexture;
    sf::Font                    m_font;
    sf::Text                    m_text;
    sf::Clock                   m_clock;
    sf::Time                    m_updateTime, m_renderTime;


    MandelbrotCalculator        m_calc;
    View<long double>           m_view;

    double                      m_ratio;
    bool                        m_redrawNeeded { true };

    std::vector<sf::Color>      m_palette;
};

#endif // SFML_VISUALIZER_H
