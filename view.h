#ifndef VIEW_H
#define VIEW_H

template<typename T>
class View
{
public:
            View();

    void    setCenter   (T x, T y);
    void    setSize     (T width, T height);
    void    move        (T offsX, T offsY);
    void    scale       (T factor);

    T       getCenterX()    const { return m_cx; }
    T       getCenterY()    const { return m_cy; }
    T       getWidth()      const { return m_width; }
    T       getHeight()     const { return m_height; }

private:


private:
    T m_cy, m_cx, m_height, m_width;
};


template<typename T>
View<T>::View()
{
}


template<typename T>
void View<T>::setCenter(T x, T y)
{
    m_cx = x; m_cy = y;
}


template<typename T>
void View<T>::setSize(T width, T height)
{
    m_width = width; m_height = height;
}


template<typename T>
void View<T>::move(T offsX, T offsY)
{
    m_cx += offsX; m_cy += offsY;
}


template<typename T>
void View<T>::scale(T factor)
{
    m_width *= factor; m_height *= factor;
}

#endif // VIEW_H
