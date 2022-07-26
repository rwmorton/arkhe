#ifndef __ARKHE_T_METABALLS_HPP__
#define __ARKHE_T_METABALLS_HPP__

//arkhe local includes (foundation)
#include "ArkheFoundationCfg.h"
#include "ArkheTImplicit.hpp"
#include "ArkheTVector3.hpp"

namespace arkhe
{

namespace foundation
{

template<typename T> class TMetaballs : public TImplicit<T>
{
public:
	//data
	struct Metaball
	{
		TVector3<T> c; //center
		T r; //radius
		T R; //radius of influence
		T w; //weight
		T t; //threshold
		//temp for debug
		T color[3];
	}; //struct Metaball
	//methods
	void add(Metaball *metaball) { metaballs.push_back(metaball); }
	void clear() { metaballs.clear(); }
	const T evaluate(const TVector3<T> &p);
	//debugging
#ifdef ARKHE_DEBUG
	void render(); //defer to another source file
#endif //ARKHE_DEBUG
private:
	//variables
	std::vector<Metaball *> metaballs; //does not take ownership of this data
protected:
	//
}; //class TMetaballs

template<typename T> const T TMetaballs<T>::evaluate(const TVector3<T> &p)
{
	T sum = TMath<T>::ZERO;
	std::vector<Metaball *>::iterator i = metaballs.begin();
	Metaball *m;
	while(i != metaballs.end())
	{
		m = *i;

		/*
		//inverse squared distance
		T val = (p-m->c).lengthSq();
		if(!TMath<T>::isZero(val)) val = TMath<T>::reciprocal(val);
		else val = TMath<T>::ZERO;
		sum += val - m->t;
		*/

		/*
		//blobby molecules (Blinn) - ae^(-br^2)
		T a = TMath<T>::ONE;
		T b = TMath<T>::ONE;
		T lenSq = (p-m->c).lengthSq();
		sum += a * TMath<T>::pow(TMath<T>::e,-b*lenSq) - m->t;
		*/

		/**/
		//metaballs
		T a = TMath<T>::ONE;
		T r = (p - m->c).lengthSq();
		if(!TMath<T>::isZero(r))
		{
			r = TMath<T>::sqrt(r);
			T res;
			if(r <= m->R/3)
			{
				res = a * (TMath<T>::ONE - (3*r*r)/m->R*m->R);
			}
			else if(r >= m->R/3 && r <= m->R)
			{
				res = 3.0/2.0 * a * (1 - r/m->R)*(1- r/m->R);
			}
			else res = 0;
			sum += res - m->t;
		}
		/**/

		/*
		//wyvill soft objects
		T r = (p - m->c).lengthSq();
		if(r < m->R)
		{
			T a = 1;
			T b = m->R * m->R;
			T ret = a * (TMath<T>::ONE - (4*r*r*r)/(9*b*b*b) + (17*r*r)/(9*b*b) - (22*r)/(9*b));
			sum += ret - m->t;
		}
		*/

		i++;
	}
	return sum;
}

} //namespace foundation

} //namespace arkhe

#endif //__ARKHE_T_METABALLS_HPP__
