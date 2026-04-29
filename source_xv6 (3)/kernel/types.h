typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;

struct procinfo {
  int pid;        // Process ID
  int ppid;       // Parent process ID
  int state;      // Process state
  uint64 sz;      // Size of process memory (bytes)
  char name[16];  // Process name
};
