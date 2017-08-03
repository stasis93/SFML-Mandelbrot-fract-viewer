#include "sfml_visualizer.h"


constexpr double factor = 1.2;
constexpr auto usageMsg =  "Navigation: WASD or arrows\n"
                           "Scale: Num+, Num-, Mouse wheel\n"
                           "Iteration limit: Num/, Num*\n"
                           "Shift - fine control\n"
                           "Mouse click - move view center to that point\n\n";


SFML_Visualizer::SFML_Visualizer() :
    m_window(sf::VideoMode(1024, 768), "Mandelbrot set visualizer"),
    m_calc(m_window.getSize().x, m_window.getSize().y, 50),
    m_ratio(static_cast<double>(m_calc.getWidth()) / m_calc.getHeight())
{
    m_window.setFramerateLimit(10);
    m_fractImage.create(m_calc.getWidth(), m_calc.getHeight());
    m_view.setCenter(0., 0.);
    m_view.setSize(6., 6. / m_ratio);

    if (!m_font.loadFromFile("arial.ttf"))
        std::cout << "Arial.ttf not found, so no text :(\n";

    m_text.setFont(m_font);
    m_text.setCharacterSize(14);
    m_text.setFillColor(sf::Color::Red);
    m_text.setPosition(10.f, 10.f);

    initPalette();

    std::cout << usageMsg;
}



void SFML_Visualizer::processEvents()
{
    sf::Event ev;
    while (m_window.pollEvent(ev)) {
        switch (ev.type) {
        case sf::Event::Closed:
            m_window.close();
            break;
        case sf::Event::KeyPressed:
            m_redrawNeeded = true;
            handleKeyboardInput(ev.key.code, ev.key.shift);
            break;
        case sf::Event::MouseWheelScrolled:
            m_redrawNeeded = true;
            handleMouseInput(ev.mouseWheelScroll.delta);
            break;
        case sf::Event::MouseButtonPressed:
            m_redrawNeeded = true;
            handleMouseInput(ev.mouseButton.button, ev.mouseButton.x, ev.mouseButton.y);
            break;
        case sf::Event::Resized:
            m_redrawNeeded = true;
            handleResize(ev.size.width, ev.size.height);
            break;
        default:
            break;
        }
    }
}



void SFML_Visualizer::handleKeyboardInput(sf::Keyboard::Key code, bool shift)
{
    double offsX = m_view.getWidth() / 10.;
    double offsY = m_view.getHeight() / 10.;

    if (shift) {
        offsX /= 10.; offsY /= 10.;
    }

    switch (code) {
    case sf::Keyboard::A:
    case sf::Keyboard::Left:
        m_view.move(-offsX, 0.);
        break;
    case sf::Keyboard::S:
    case sf::Keyboard::Down:
        m_view.move(0., -offsY);
        break;
    case sf::Keyboard::D:
    case sf::Keyboard::Right:
        m_view.move(offsX, 0.);
        break;
    case sf::Keyboard::W:
    case sf::Keyboard::Up:
        m_view.move(0., offsY);
        break;
    case sf::Keyboard::Add:
        m_view.scale(1. / factor);
        break;
    case sf::Keyboard::Subtract:
        m_view.scale(factor);
        break;
    case sf::Keyboard::Escape:
        m_window.close();
        break;
    case sf::Keyboard::Multiply:
        m_calc.setMaxIter(m_calc.maxIter() + (shift ? 1 : 10));
        break;
    case sf::Keyboard::Divide:
        m_calc.setMaxIter(m_calc.maxIter() - (shift ? 1 : 10));
        break;
    default:
        break;
    }
}



void SFML_Visualizer::handleMouseInput(float delta)
{
    double f = delta < 0. ? factor : 1. / factor;
    m_view.scale(f);
}



void SFML_Visualizer::handleMouseInput(sf::Mouse::Button btn, int x, int y)
{
    if (!btn == sf::Mouse::Left)
        return;
    // adjust view center according to click
    double dx = m_view.getWidth() / m_calc.getWidth();  // step per 1 pixel
    double dy = m_view.getHeight() / m_calc.getHeight();

    int centerPixX = m_calc.getWidth() / 2;             // current center coords in pixels
    int centerPixY = m_calc.getHeight() / 2;

    double offsX = (x - centerPixX) * dx;
    double offsY = (centerPixY - y) * dy;

    m_view.move(offsX, offsY);
}



void SFML_Visualizer::handleResize(int newWidth, int newHeight)
{
    m_calc.resize(newWidth, newHeight);
    m_window.setSize(sf::Vector2u(m_calc.getWidth(), m_calc.getHeight()));
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, newWidth, newHeight)));
    m_ratio = static_cast<double>(m_calc.getWidth()) / m_calc.getHeight();
    m_fractImage.create(m_calc.getWidth(), m_calc.getHeight());
    m_view.setSize(m_view.getWidth(), m_view.getWidth() / m_ratio);
}



void SFML_Visualizer::update()
{    
    if (m_redrawNeeded) {
        m_clock.restart();
        m_calc.calculate(m_view.getCenterX(), m_view.getCenterY(), m_view.getWidth(), m_view.getHeight());
        m_updateTime = m_clock.getElapsedTime();
    }
}



void SFML_Visualizer::render()
{
    m_clock.restart();
    m_window.clear();
    // Drawing goes here...
    const auto& field = m_calc.getField();

    if (m_redrawNeeded) {
        sf::Color* pixels = reinterpret_cast<sf::Color*>(
                            const_cast<sf::Uint8*>(m_fractImage.getPixelsPtr())); // yeah, that's dirty shit (but faster than setPixel'ing)

        for (auto y = 0; y < m_calc.getHeight(); y++)
            for (auto x = 0; x < m_calc.getWidth(); x++) {
                const auto& iterations = field[y][x];
                auto pixel = &pixels[y * m_calc.getWidth() + x];
                if (iterations == 0 || iterations == m_calc.maxIter())
                    *pixel = sf::Color::Black;
                else
                    *pixel = m_palette[iterations % m_palette.size()];
            }
        m_redrawNeeded = false;
    }
    m_fractTexture.loadFromImage(m_fractImage);
    m_fractSprite.setTexture(m_fractTexture, true);
    // Not so accurate...
    m_renderTime = m_clock.getElapsedTime();

    std::stringstream ss;
    ss << "Resolution: " << m_calc.getWidth() << "x" << m_calc.getHeight() << "\n"
       << "Iteration limit: " << m_calc.maxIter() << "\n"
       << "Update time: " << m_updateTime.asMilliseconds() << " ms\n"
       << "Render time: " << m_renderTime.asMilliseconds() << " ms\n\n"
       << "Place: Re[" << std::fixed << std::setprecision(15) << m_view.getCenterX()
       << "]; Im[" << m_view.getCenterY() << "]\n"
       << "Window size: Re[" << m_view.getWidth() << "]; Im[" << m_view.getHeight() << "]";

    m_text.setString(ss.str());

    m_window.draw(m_fractSprite);
    m_window.draw(m_text);
    m_window.display();
}



void SFML_Visualizer::initPalette()
{
    m_palette.assign( {sf::Color(66, 30, 15),
                       sf::Color(25, 7, 26),
                       sf::Color(9, 1, 47),
                       sf::Color(4, 4, 73),
                       sf::Color(0, 7, 100),
                       sf::Color(12, 44, 138),
                       sf::Color(24, 82, 177),
                       sf::Color(57, 125, 209),
                       sf::Color(134, 181, 229),
                       sf::Color(211, 236, 248),
                       sf::Color(241, 233, 191),
                       sf::Color(248, 201, 95),
                       sf::Color(255, 170, 0),
                       sf::Color(204, 128, 0),
                       sf::Color(153, 87, 0),
                       sf::Color(106, 52, 3) });
}



void SFML_Visualizer::run()
{
    while (m_window.isOpen()) {
        processEvents();
        update();
        render();
    }
}
