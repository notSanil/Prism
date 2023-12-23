#include "pch.h"
#include "Node.h"

Node::Node(int id)
	:m_Id(id), c_InputStart(id << 8), c_OutputStart(id << 16), c_StaticStart(id << 24)
{}

Node::~Node()
{}
