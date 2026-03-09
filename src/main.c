#include <stdio.h>

#include "handler.h"

int
main(int argc, char* argv[])
{
      int is_correct = 0x00;

      is_correct = parse_args(argc, argv);
      if (is_correct)
      {
            fprintf(stderr, "ERROR: cannot parse args.\n");
      }
      
      return 0;
}