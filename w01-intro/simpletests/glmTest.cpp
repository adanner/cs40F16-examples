#include <iostream>
#include <glm/glm.hpp> //vec2
#include <glm/gtx/string_cast.hpp> //to_string

/* Test GLM http://glm.g-truc.net/0.9.7/index.html */

using namespace std;

int main(){
  cout << "Hello CS40!" << endl;
	glm::vec2 v1(0,1);
	glm::vec2 v2(1,0);
	cout << "V1 = " << glm::to_string(v1) << endl;
	cout << "V2 = " << glm::to_string(v1) << endl;
	cout << "V1+V2 = " << glm::to_string(v1+v2) << endl;
  return 0;
}
