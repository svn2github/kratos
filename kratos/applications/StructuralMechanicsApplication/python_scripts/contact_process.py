 
from __future__ import print_function, absolute_import, division #makes KratosMultiphysics backward compatible with python 2.6 and 2.7
# Importing the Kratos Library // TODO: Remove this libraies
from KratosMultiphysics import *
from KratosMultiphysics.SolidMechanicsApplication import *
from KratosMultiphysics.StructuralMechanicsApplication import *

CheckForPreviousImport()

def Factory(settings, Model):
    if(type(settings) != Parameters):
        raise Exception("Expected input shall be a Parameters object, encapsulating a json string")
    return ContactProcess(Model, settings["Parameters"])

class ContactProcess(Process):
  
    def __init__(self,model_part,params):
        
                
        ##settings string in json format
        default_parameters = Parameters("""
        {
            "origin_model_part_name"      : "",
            "destination_model_part_name" : "",
            "contact_type"                : "MortarMethod",
            "search_factor"               : 1.1,
            "allocation_size"             : 1000,
            "max_number_results"          : 1000,
            "bidirectional"               : true,
            "type_search"                 : "InRadius",
            "integration_order"           : 2
        }
        """)
        
        ## Overwrite the default settings with user-provided parameters
        self.params = params
        self.params.ValidateAndAssignDefaults(default_parameters)
        
        self.o_model_part = model_part[self.params["origin_model_part_name"].GetString()]
        self.d_model_part = model_part[self.params["destination_model_part_name"].GetString()]
        
        self.search_factor      = self.params["search_factor"].GetDouble() 
        self.allocation_size    = self.params["allocation_size"].GetInt() 
        self.max_number_results = self.params["max_number_results"].GetInt() 
        self.bidirectional      = self.params["bidirectional"].GetBool() 
        self.integration_order  = self.params["integration_order"].GetInt() 
        if self.params["type_search"].GetString() == "InRadius":
             self.type_search = 0
        
    def ExecuteInitialize(self):
        
        for node in self.o_model_part.Nodes:
            node.Set(INTERFACE,True)
        del(node)
        
        for node in self.d_model_part.Nodes:
            node.Set(INTERFACE,True)
        del(node)
        
        self.Preprocess  = InterfacePreprocessCondition()
        self.o_interface = ModelPart("origin_interface")
        self.d_interface = ModelPart("destination_interface")
        
        if self.params["contact_type"].GetString() == "MortarMethod":
            condition_name = "MortarContact"
        elif self.params["contact_type"].GetString() == "NTN":
            condition_name = "NTNContact"
        elif self.params["contact_type"].GetString() == "NTS":
            condition_name = "NTSContact"
        
        self.Preprocess.GenerateInterfacePart(self.o_model_part, self.o_interface, condition_name) #It should create the conditions automatically
        self.Preprocess.GenerateInterfacePart(self.d_model_part, self.d_interface, condition_name)
        
        self.contact_search = TreeContactSearch(self.o_interface, self.d_interface, self.allocation_size)
        
        if self.params["contact_type"].GetString() == "MortarMethod":
            self.contact_search.CreatePointListMortar()
            self.contact_search.InitializeMortarConditions()
        elif self.params["contact_type"].GetString() == "NTN":
            self.contact_search.CreatePointListNTN()
            self.contact_search.InitializeNTNConditions()
        elif self.params["contact_type"].GetString() == "NTS":
            self.contact_search.CreatePointListNTS()
            self.contact_search.InitializeNTSConditions()
        
    def ExecuteBeforeSolutionLoop(self):
        pass
    
    def ExecuteInitializeSolutionStep(self):
        if self.params["contact_type"].GetString() == "MortarMethod":
            self.contact_search.CreateMortarConditions(self.search_factor,self.max_number_results,self.type_search, self.bidirectional, self.integration_order)
        elif self.params["contact_type"].GetString() == "NTN":
            self.contact_search.CreateNTNConditions(self.search_factor,self.max_number_results,self.type_search, self.bidirectional, self.integration_order)
        elif self.params["contact_type"].GetString() == "NTS":
            self.contact_search.CreateNTSConditions(self.search_factor,self.max_number_results,self.type_search, self.bidirectional, self.integration_order)
 
    def ExecuteFinalizeSolutionStep(self):
        pass
              
    def ExecuteBeforeOutputStep(self):
        pass

    def ExecuteAfterOutputStep(self):
        pass

    def ExecuteFinalize(self):
        pass
