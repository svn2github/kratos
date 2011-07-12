import sys
kratos_benchmarking_path = '../../../../benchmarking' 
sys.path.append(kratos_benchmarking_path)
import benchmarking

print "Building reference data for consolidation.gid..."
benchmarking.BuildReferenceData("consolidation_benchmarking.py", "consolidation_ref.txt")
