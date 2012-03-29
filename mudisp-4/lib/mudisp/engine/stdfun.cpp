#include <iostream>
#include <vector>
class decissor
{
public:
  static bool intdecissor(int p)
  {
	 return (p%2)==0;
  }
};

int main (int argc, char * argv)
{
  std::vector <int> theVec1M;
  std::vector <int> theVec2M;
  for (int cont = 0; cont < 6; cont++)
	 theVec1M.push_back(cont);
  //
  std::remove_copy_if(theVec1M.begin(),
		theVec1M.end(),
		std::back_inserter(theVec2M),
		//the std::not1 is only to force the use of std:ptr_fun
		std::not1(std::ptr_fun(decissor::intdecissor)));
  //std::ptr_fun -> converts a pointer to fun in a object, we need it beacase
  //if we use std::not1(decissor::intdecissor) -> the compiler dont allow it
  std::cout<<"Vector 1"<<std::endl;
  for (std::vector<int>::iterator it = theVec1M.begin();
		 it != theVec1M.end(); it++)
	 std::cout<<" "<<*it;
  std::cout<<"Vector 2"<<std::endl;
  for (std::vector<int>::iterator it = theVec2M.begin();
		 it != theVec2M.end(); it++)
	 std::cout<<" "<<*it;
  std::cout<<std::endl;
}
