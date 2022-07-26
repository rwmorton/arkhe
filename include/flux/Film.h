#ifndef __FILM_H__
#define __FILM_H__

//flux includes
#include "Base.h"

class Film
{
public:
	//constructor
	Film(int w,int h)
	{
		m_width = w;
		m_height = h;
		m_aspect = (float)w / h;
		m_pixels = new float[w * h * 3];
	}
	//destructor
	~Film()
	{
		if(m_pixels) delete[] m_pixels;
		m_pixels = 0;
	}
	//methods
	void clearTo(int r,int g,int b)
	{
		for(int x=0; x<m_width; x++)
		{
			for(int y=0; y<m_height; y++)
			{
				setPixel(x,y,r,g,b);
			}
		}
	}
	inline void setPixel(int x,int y,float r,float g,float b)
	{
		if(x < 0 || y < 0 || x >= m_width || y >= m_height)
		{
			throw Exception(Exception::OUT_OF_RANGE);
		}
		if(m_pixels)
		{
			int pos = 3*x + 3*y*m_width;
			m_pixels[pos] = r;
			m_pixels[pos+1] = g;
			m_pixels[pos+2] = b;
		}
		else
			throw Exception(Exception::NULL_POINTER);
	}
	float *getBuffer() const { return m_pixels; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	float getAspect() const { return m_aspect; }
private:
	//non-copyable
	Film(const Film &);
	Film &operator=(const Film &);
	//data
	int m_width,m_height;
	float m_aspect;
	float *m_pixels; //must be float otherwise OpenGL will complain
protected:
	//
}; //class Film

#endif //__FILM_H__
