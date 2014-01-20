from __future__ import print_function, absolute_import, division #makes KratosMultiphysics backward compatible with python 2.6 and 2.7
import sys

kratos_benchmarking_path = '../../../../benchmarking'
sys.path.append(kratos_benchmarking_path)

import benchmarking


def Run():
    print("Running scordelis low roof")
    return benchmarking.RunBenchmark("run_test.py", "min_displacements.txt")

if __name__ == '__main__':
    Msg = Run()
    if(Msg):
        print("Test successful")
    else:
        print("Test failed")
        print(Msg)
