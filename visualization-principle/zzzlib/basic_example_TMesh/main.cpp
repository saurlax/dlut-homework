#include <iostream>

#include "TetTestMesh.h"
#include "TetTest.h"

using namespace TMeshLib;

int main(int argc, char * argv[])
{
	CTTMesh tmesh;
	tmesh._load_t(argv[1]);
	CTetTest<CTTMesh> tet_test(&tmesh);
	tet_test.test();
	tmesh._write_t(argv[2]);
	
	cout << "finished !!!" << endl;
	getchar();
}