# import Kratos
import KratosMultiphysics
import KratosMultiphysics.ExternalSolversApplication
import KratosMultiphysics.FluidDynamicsApplication
import KratosMultiphysics.MeshingApplication

# Import Kratos "wrapper" for unittests
import KratosMultiphysics.KratosUnittest as KratosUnittest

# Import the tests o test_classes to create the suits
## SMALL TESTS # TODO: Add test_refine.py to the list of tests
from SmallTests import TwoDHessianTest as TTwoDHessianTest
from SmallTests import ThreeDHessianTest as TThreeDHessianTest
from SmallTests import TwoDCavityTest as TTwoDCavityTest
from SmallTests import CoarseSphereTest as TCoarseSphereTest

## SMALL TESTS
from NightlyTests import StanfordBunnyTest as TStanfordBunnyTest

## VALIDATION TESTS 

def AssambleTestSuites():
    ''' Populates the test suites to run.

    Populates the test suites to run. At least, it should pupulate the suites:
    "small", "nighlty" and "all"

    Return
    ------

    suites: A dictionary of suites
        The set of suites with its test_cases added.
    '''
    suites = KratosUnittest.KratosSuites

    # Create a test suit with the selected tests (Small tests):
    smallSuite = suites['small']
    if( hasattr(KratosMultiphysics.MeshingApplication,  "MmgUtility2D") ):
        smallSuite.addTest(TTwoDHessianTest('test_execution'))
        smallSuite.addTest(TThreeDHessianTest('test_execution'))
        smallSuite.addTest(TTwoDCavityTest('test_execution'))
        smallSuite.addTest(TCoarseSphereTest('test_execution'))
    else:
        print("MMG utility is not compiled and the corresponding tests will not be executed")

    # Create a test suit with the selected tests plus all small tests
    nightSuite = suites['nightly']
    nightSuite.addTests(smallSuite)
    if( hasattr(KratosMultiphysics.MeshingApplication,  "MmgUtility2D") ):
        nightSuite.addTest(TStanfordBunnyTest('test_execution'))
    else:
        print("MMG utility is not compiled and the corresponding tests will not be executed")
    
    # For very long tests that should not be in nighly and you can use to validate 
    validationSuite = suites['validation']

    # Create a test suit that contains all the tests:
    allSuite = suites['all']
    allSuite.addTests(
        KratosUnittest.TestLoader().loadTestsFromTestCases([
        ])
    )

    if( hasattr(KratosMultiphysics.MeshingApplication,  "MmgUtility2D") ):
        allSuite.addTests(
            KratosUnittest.TestLoader().loadTestsFromTestCases([
                TTwoDHessianTest,
                TThreeDHessianTest,
                TTwoDCavityTest,
                TCoarseSphereTest,
                #TStanfordBunnyTest,
            ])
        )
    else:
        print("MMG utility is not compiled and the corresponding tests will not be executed")

    return suites

if __name__ == '__main__':
    KratosUnittest.runTests(AssambleTestSuites())
