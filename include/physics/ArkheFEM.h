#ifndef __ARKHE_FEM_H__
#define __ARKHE_FEM_H__

//foundation includes
#include <foundation/ArkheFoundation.h>

//std includes
#include <vector>
#include <algorithm>
using namespace std;

namespace arkhe
{

namespace physics
{

//FEM test - only spring element

class Element;	

struct Node
{
	unsigned int id;
	Real x; //position along x-axis
	bool boundary;
	vector<Element *> sharedElements;
};

class Element
{
public:
	//constructor
	Element(Real l,Real k,Node *a,Node *b) : length(l),stiffness(k),A(a),B(b)
	{
		stiffnessMatrix(0,0) = stiffness;
		stiffnessMatrix(0,1) = -stiffness;
		stiffnessMatrix(1,0) = -stiffness;
		stiffnessMatrix(1,1) = stiffness;
	}
	//methods
	const TMatrixNN<Real,2> getStiffnessMatrix() const { return stiffnessMatrix; }
	//data
	Real length;
	Real stiffness;
	Node *A,*B;
	TMatrixNN<Real,2> stiffnessMatrix;
};

//takes ownership of the nodes and elements
class Assemblage
{
public:
	//destructor
	~Assemblage()
	{
		for(int i=0; i<elements.size(); i++) delete elements.at(i);
		for(int i=0; i<nodes.size(); i++) delete nodes.at(i);
	}
	//methods
	void addNode(Node *n) { nodes.push_back(n); }
	void addElement(Element *e) { elements.push_back(e); }
	void sortNodes()
	{
		std::sort(nodes.begin(),nodes.end());
	}
	//data
	vector<Node *> nodes;
	vector<Element *> elements;
};

} //namespace physics

} //namespace arkhe

#endif //__ARKHE_FEM_H__
