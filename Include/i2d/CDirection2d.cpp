#include "i2d/CDirection2d.h"


// STD includes
#include <math.h>

// ACF includes
#include "imath/imath.h"


namespace i2d
{


CDirection2d::CDirection2d()
:	m_direction(0.0)
{
	I_ASSERT(Invariant());
}


CDirection2d::CDirection2d(const CDirection2d& direction)
:	m_direction(direction.m_direction)
{
	I_ASSERT(direction.Invariant());

	I_ASSERT(Invariant());
}


CDirection2d::CDirection2d(double radian)
:	m_direction(radian)
{
	I_ASSERT(radian >= -I_PI - I_BIG_EPSILON);
	I_ASSERT(radian <= I_PI + I_BIG_EPSILON);

	I_ASSERT(Invariant());
}


double CDirection2d::DistInRadian(const CDirection2d& dir2) const
{
	I_ASSERT(Invariant());
	I_ASSERT(dir2.Invariant());

	double w1 = qMin(m_direction, dir2.m_direction); 
	double w2 = qMax(m_direction, dir2.m_direction);

	double dist = qMin(w2 - w1, I_2PI - (w2 - w1));
	
	I_ASSERT(dist >= 0.0);
	I_ASSERT(dist <= I_PI);
		
	return dist;
}


double CDirection2d::DistInDegree(const CDirection2d& dir2) const
{
	I_ASSERT(Invariant());
	I_ASSERT(dir2.Invariant());

	double dist = DistInRadian(dir2) / I_2PI * 360.0;

	I_ASSERT(dist >= 0.0);
	I_ASSERT(dist <= 180.0);
		
	return dist;
}


bool CDirection2d::operator==(const CDirection2d& dir2) const
{
	I_ASSERT(Invariant());
	I_ASSERT(dir2.Invariant());

	return qAbs(DistInRadian(dir2)) < I_BIG_EPSILON;
}


bool CDirection2d::Invariant() const
{
	return (m_direction >= -I_PI - I_BIG_EPSILON) && (m_direction <= I_PI + I_BIG_EPSILON);
}


double CDirection2d::Normalize(double radian)
{
	double retVal = (radian >= 0) ?
					::fmod(radian + I_PI, I_2PI) - I_PI:
					I_PI - ::fmod(I_PI - radian, I_2PI);
					
	I_ASSERT((retVal >= -I_PI - I_BIG_EPSILON) && (retVal <= I_PI + I_BIG_EPSILON));

	return retVal;
}


CDirection2d CDirection2d::FromRadian(double radian)
{
	return CDirection2d(CDirection2d::Normalize(radian));
}


CDirection2d CDirection2d::FromDegree(double degree)
{
	return CDirection2d(CDirection2d::Normalize(degree * (I_2PI / 360.0)));
}


double CDirection2d::ToDegree() const
{
	return fmod(m_direction / I_2PI * 360.0 + 360.0, 360.0);
}


double CDirection2d::ToRadian() const
{
	return m_direction;
}


CDirection2d CDirection2d::Avg(const CDirection2d& dir2) const
{
	double dist(DistInRadian(dir2) / 2.0);
	
	CDirection2d res1(this->TurnByRadian(dist));
	CDirection2d res2(this->TurnByRadian(-dist));

	CDirection2d& res = dir2.DistInRadian(res1) <= dir2.DistInRadian(res2) ? res1 : res2;	

	I_ASSERT(res.DistInRadian(*this) + res.DistInRadian(dir2) == DistInRadian(dir2));

	return res;
}


CDirection2d CDirection2d::Avg(double weight, const CDirection2d& dir2) const
{
	I_ASSERT(Invariant());
	I_ASSERT(dir2.Invariant());
	I_ASSERT(weight >= 0.0);
	I_ASSERT(weight <= 1.0);

	double dist(DistInRadian(dir2) * weight);
	
	CDirection2d res1(dir2.TurnByRadian(dist));
	CDirection2d res2(dir2.TurnByRadian(-dist));

	CDirection2d& res = DistInRadian(res1) <= DistInRadian(res2) ? res1 : res2;

	I_IF_DEBUG(
		const double a(res.DistInRadian(*this));
		const double b(res.DistInRadian(dir2));
		const double c(DistInRadian(dir2));
		I_ASSERT((a + b < c + I_BIG_EPSILON) && (a + b > c - I_BIG_EPSILON));
	);
	
	I_ASSERT(res.Invariant());

	return res;
}


CDirection2d CDirection2d::TurnByRadian(double turn) const
{
	I_ASSERT(Invariant()); 

	const CDirection2d& res = CDirection2d::FromRadian(this->ToRadian()+turn);

	I_ASSERT(res.Invariant());

	return res;
}


bool CDirection2d::IsInRange(const CDirection2d& leftDirectionLimit, const CDirection2d& rightDirectionLimit) const
{
	const double wl(leftDirectionLimit.ToRadian());
	const double wr(rightDirectionLimit.ToRadian());
	const double wt(ToRadian());

	if (wl <= wt && wt <= wr){
		return true;
	}
	else if ( wl > wr && (wt <= wr || wt >= wl )){
		return true;
	}
	else {
		return false;
	}
}


} // namespace i2d


