#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>
#include<iostream>
using namespace std;

       int
       main(int argc, char **argv)
       {
           int c;
           int digit_optind = 0;
           string delimiter, fieldNumber;

           while (1) {
               int this_option_optind = optind ? optind : 1;
               int option_index = 0;
               static struct option long_options[] = {
                      {"fields", required_argument, NULL, 'f'},
                      {"delimiter", required_argument, NULL, 'd'},
                      {NULL, 0, NULL, 0}
               };

               c = getopt_long(argc, argv, "d:f:",
                        long_options, &option_index);
               if (c == -1)
                   break;

               switch (c) {
               case 'd':
                   printf("option d with value '%s'\n", optarg);
                   delimiter = optarg;
                   break;

               case 'f':
                   printf("option f with value '%s'\n", optarg);
                   fieldNumber = optarg;
                   break;

               case '?':
                   break;

               default:
                   printf("?? getopt returned character code 0%o ??\n", c);
               }
           }

           if (optind < argc) {
               while (optind < argc)
                   printf("%s ", argv[optind++]);
               printf(" : options not available.\nTry using [d] and [f] ");
               exit(0);
           }

           exit(EXIT_SUCCESS);
      }
