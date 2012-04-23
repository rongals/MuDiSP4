//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 

#define DUMP_FILE_NAME "param.txt"

#include "port.h"
#include "block.h"
#include "system.h"
#include "param.h"

//class System : public Block;


int main(int argc, char* argv[]) {

  System s;

  s.Build();

  if (argc!=2) {
    cout << "MuDiSP3\n" 
         << "by Luca Ronga\n\n"
	 << "A sample parameter file for your system has been\n"
	 << "generated in \"param.txt\"\n\n"
	 << "To run the simulation use the syntax:\n"
	 << argv[0] << "  <param_file>\n\n";

    ofstream outstr(DUMP_FILE_NAME);
    s._DumpParameters(outstr);
    outstr << endl; 
    outstr.close();
  }
  else {
    string filename(argv[1]);
    s._ReadParameters(filename);
    s.SequenceFinder();
    s.Run();
    s.Finish();
  }
}

