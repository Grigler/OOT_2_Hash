#ifndef __VEC2__
#define __VEC2__

struct Vec2
{
	float m_x;
	float m_y;
	
	//Operator overload for ease of use
	Vec2 operator*(float _f)
	{
		Vec2 r;
		
		r.m_x = m_x * _f;
		r.m_y = m_y * _f;

		return r;
	}
	//May need to re-visit and compare with an epsilon value
	bool operator==(const Vec2& other)
	{
		return m_x == other.m_x && m_y == other.m_y;		
	}

	const Vec2 operator+(const Vec2& other)
	{
		Vec2 r;
		r.m_x = m_x + other.m_x;
		r.m_y = m_y + other.m_y;
		return r;
	}
};

#endif