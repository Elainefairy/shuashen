#include<iostream> 
#include<fstream>
#include <stdlib.h>
#include"ginsengdll.h"  
using namespace std;
int main()
{
	//extern int pictureDifferentDection(char* image1Path, char* image2Path, double threshold_control = 0.1, double width_control = 0, double height_control = 0);
	string a, b;
	cin >> a;
	cin >> b;
	char* image1Path = new char[a.size()]; //"C://Users//yl//Desktop//2.1.jpg	"; 
	char* image2Path = new char[b.size()]; //"C://Users//yl//Desktop//2.2.jpg";  

	strcpy(image1Path, a.data());
	strcpy(image2Path, b.data());
	pictureDifferentDection(image2Path, image1Path);
	system("pause");
	return 0;
}
