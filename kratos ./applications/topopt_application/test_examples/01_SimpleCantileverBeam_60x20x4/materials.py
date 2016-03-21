
# Importing the Kratos Library
from KratosMultiphysics import *
from KratosMultiphysics.TopOptApplication import *
#from beam_sections_python_utility import SetProperties

def AssignMaterial(Properties):
# GUI property identifier: Property1
# GUI material identifier: Steel_AISI1059
    prop_id = 1;
    prop = Properties[prop_id]
    mat = LinearElastic3DLaw();
    prop.SetValue(CONSTITUTIVE_LAW, mat.Clone());
    
    #YoungModulus = prop.GetValue(YOUNG_MODULUS) + 100000000000000000000000;
    #prop.SetValue(YOUNG_MODULUS, YoungModulus);
