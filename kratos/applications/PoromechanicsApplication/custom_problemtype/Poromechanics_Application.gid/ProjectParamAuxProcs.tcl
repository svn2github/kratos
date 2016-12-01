proc AppendGroupNames {String CondName} {
    upvar $String MyString
    
    set Groups [GiD_Info conditions $CondName groups]
    for {set i 0} {$i < [llength $Groups]} {incr i} {
        append MyString \" [lindex [lindex $Groups $i] 1] \" ,
    }
}

#-------------------------------------------------------------------------------

proc AppendGroupNamesWithNum {String GroupNum CondName} {
    upvar $String MyString
    upvar $GroupNum MyGroupNum
    
    set Groups [GiD_Info conditions $CondName groups]
    for {set i 0} {$i < [llength $Groups]} {incr i} {
        incr MyGroupNum
        append MyString \" [lindex [lindex $Groups $i] 1] \" ,
    }
}

#-------------------------------------------------------------------------------

proc AppendGroupVariables {String CondName VarName} {
    upvar $String MyString
    
    set Groups [GiD_Info conditions $CondName groups]
    for {set i 0} {$i < [llength $Groups]} {incr i} {
        append MyString \" $VarName \" ,
    }
}

#-------------------------------------------------------------------------------

proc AppendOutputVariables {String GroupNum QuestionName VarName} {
    upvar $String MyString
    upvar $GroupNum MyGroupNum
    
    if {[GiD_AccessValue get gendata $QuestionName]==true} {
        incr MyGroupNum
        append MyString \" $VarName \" ,
    }
}

#-------------------------------------------------------------------------------

proc WriteConstraintVectorProcess {FileVar GroupNum Groups EntityType VarName TableDict NumGroups} {
    upvar $FileVar MyFileVar
    upvar $GroupNum MyGroupNum

    for {set i 0} {$i < [llength $Groups]} {incr i} {
        set Entities [GiD_EntitiesGroups get [lindex [lindex $Groups $i] 1] $EntityType]
        if {[llength $Entities] > 0} {
            incr MyGroupNum
            puts $MyFileVar "        \"python_module\": \"apply_constraint_vector_table_process\","
            puts $MyFileVar "        \"kratos_module\": \"KratosMultiphysics.PoromechanicsApplication\","
            puts $MyFileVar "        \"process_name\":  \"ApplyConstraintVectorTableProcess\","
            puts $MyFileVar "        \"Parameters\":    \{"
            puts $MyFileVar "            \"mesh_id\":         0,"
            puts $MyFileVar "            \"model_part_name\": \"[lindex [lindex $Groups $i] 1]\","
            puts $MyFileVar "            \"variable_name\":   \"$VarName\","
            puts $MyFileVar "            \"active\":          \[[lindex [lindex $Groups $i] 3],[lindex [lindex $Groups $i] 8],[lindex [lindex $Groups $i] 13]\],"
            puts $MyFileVar "            \"is_fixed\":        \[[lindex [lindex $Groups $i] 5],[lindex [lindex $Groups $i] 10],[lindex [lindex $Groups $i] 15]\],"
            puts $MyFileVar "            \"value\":           \[[lindex [lindex $Groups $i] 4],[lindex [lindex $Groups $i] 9],[lindex [lindex $Groups $i] 14]\],"
            puts $MyFileVar "            \"table\":           \[[dict get $TableDict [lindex [lindex $Groups $i] 1] Table0],[dict get $TableDict [lindex [lindex $Groups $i] 1] Table1],[dict get $TableDict [lindex [lindex $Groups $i] 1] Table2]\]"
            puts $MyFileVar "        \}"
            if {$MyGroupNum < $NumGroups} {
                puts $MyFileVar "    \},\{"
            } else {
                puts $MyFileVar "    \}\],"
            }
        }
    }
}

#-------------------------------------------------------------------------------

proc WritePressureConstraintProcess {FileVar GroupNum Groups EntityType VarName TableDict NumGroups} {
    upvar $FileVar MyFileVar
    upvar $GroupNum MyGroupNum

    for {set i 0} {$i < [llength $Groups]} {incr i} {
        set Entities [GiD_EntitiesGroups get [lindex [lindex $Groups $i] 1] $EntityType]
        if {[llength $Entities] > 0} {
            incr MyGroupNum
            puts $MyFileVar "        \"python_module\": \"apply_pore_pressure_table_process\","
            puts $MyFileVar "        \"kratos_module\": \"KratosMultiphysics.PoromechanicsApplication\","
            puts $MyFileVar "        \"process_name\":  \"ApplyPorePressureTableProcess\","
            puts $MyFileVar "        \"Parameters\":    \{"
            puts $MyFileVar "            \"mesh_id\":              0,"
            puts $MyFileVar "            \"model_part_name\":      \"[lindex [lindex $Groups $i] 1]\","
            puts $MyFileVar "            \"variable_name\":        \"$VarName\","
            puts $MyFileVar "            \"is_fixed\":             [lindex [lindex $Groups $i] 8],"
            puts $MyFileVar "            \"value\":                [lindex [lindex $Groups $i] 4],"
            puts $MyFileVar "            \"table\":                [dict get $TableDict [lindex [lindex $Groups $i] 1] Table0],"
            if {[lindex [lindex $Groups $i] 3] == "Hydrostatic"} {
                set PutStrings true
            } else {
                set PutStrings false
            }
            puts $MyFileVar "            \"hydrostatic\":          $PutStrings,"
            if {[lindex [lindex $Groups $i] 5] == "Y"} {
                set PutStrings 2
            } elseif {[lindex [lindex $Groups $i] 5] == "Z"} {
                set PutStrings 3
            } else {
                set PutStrings 1
            }
            puts $MyFileVar "            \"gravity_direction\":    $PutStrings,"
            puts $MyFileVar "            \"reference_coordinate\": [lindex [lindex $Groups $i] 6],"
            puts $MyFileVar "            \"specific_weight\":      [lindex [lindex $Groups $i] 7]"
            puts $MyFileVar "        \}"
            if {$MyGroupNum < $NumGroups} {
                puts $MyFileVar "    \},\{"
            } else {
                puts $MyFileVar "    \}\],"
            }
        }
    }
}

#-------------------------------------------------------------------------------

proc WriteLoadVectorProcess {FileVar GroupNum Groups EntityType VarName TableDict NumGroups} {
    upvar $FileVar MyFileVar
    upvar $GroupNum MyGroupNum

    for {set i 0} {$i < [llength $Groups]} {incr i} {
        set Entities [GiD_EntitiesGroups get [lindex [lindex $Groups $i] 1] $EntityType]
        if {[llength $Entities] > 0} {
            incr MyGroupNum
            puts $MyFileVar "        \"python_module\": \"apply_load_vector_table_process\","
            puts $MyFileVar "        \"kratos_module\": \"KratosMultiphysics.PoromechanicsApplication\","
            puts $MyFileVar "        \"process_name\":  \"ApplyLoadVectorTableProcess\","
            puts $MyFileVar "        \"Parameters\":    \{"
            puts $MyFileVar "            \"mesh_id\":         0,"
            puts $MyFileVar "            \"model_part_name\": \"[lindex [lindex $Groups $i] 1]\","
            puts $MyFileVar "            \"variable_name\":   \"$VarName\","
            puts $MyFileVar "            \"active\":          \[[lindex [lindex $Groups $i] 3],[lindex [lindex $Groups $i] 7],[lindex [lindex $Groups $i] 11]\],"
            puts $MyFileVar "            \"value\":           \[[lindex [lindex $Groups $i] 4],[lindex [lindex $Groups $i] 8],[lindex [lindex $Groups $i] 12]\],"
            if {[GiD_AccessValue get gendata Strategy_Type]=="Arc-Length"} {
                puts $MyFileVar "            \"table\":           \[0,0,0\]"
            } else {
                puts $MyFileVar "            \"table\":           \[[dict get $TableDict [lindex [lindex $Groups $i] 1] Table0],[dict get $TableDict [lindex [lindex $Groups $i] 1] Table1],[dict get $TableDict [lindex [lindex $Groups $i] 1] Table2]\]"
            }
            puts $MyFileVar "        \}"
            if {$MyGroupNum < $NumGroups} {
                puts $MyFileVar "    \},\{"
            } else {
                puts $MyFileVar "    \}\]"
            }
        }
    }
}

#-------------------------------------------------------------------------------

proc WriteNormalLoadProcess {FileVar GroupNum Groups EntityType VarName TableDict NumGroups} {
    upvar $FileVar MyFileVar
    upvar $GroupNum MyGroupNum

    for {set i 0} {$i < [llength $Groups]} {incr i} {
        set Entities [GiD_EntitiesGroups get [lindex [lindex $Groups $i] 1] $EntityType]
        if {[llength $Entities] > 0} {
            incr MyGroupNum
            puts $MyFileVar "        \"python_module\": \"apply_normal_load_table_process\","
            puts $MyFileVar "        \"kratos_module\": \"KratosMultiphysics.PoromechanicsApplication\","
            puts $MyFileVar "        \"process_name\":  \"ApplyNormalLoadTableProcess\","
            puts $MyFileVar "        \"Parameters\":    \{"
            puts $MyFileVar "            \"mesh_id\":              0,"
            puts $MyFileVar "            \"model_part_name\":      \"[lindex [lindex $Groups $i] 1]\","
            puts $MyFileVar "            \"variable_name\":        \"$VarName\","
            puts $MyFileVar "            \"active\":               \[[lindex [lindex $Groups $i] 3],[lindex [lindex $Groups $i] 11]\],"
            puts $MyFileVar "            \"value\":                \[[lindex [lindex $Groups $i] 5],[lindex [lindex $Groups $i] 12]\],"
            if {[GiD_AccessValue get gendata Strategy_Type]=="Arc-Length"} {
                puts $MyFileVar "            \"table\":                \[0,0\],"
            } else {
                puts $MyFileVar "            \"table\":                \[[dict get $TableDict [lindex [lindex $Groups $i] 1] Table0],[dict get $TableDict [lindex [lindex $Groups $i] 1] Table1]\],"
            }
            if {[lindex [lindex $Groups $i] 4] == "Hydrostatic"} {
                set PutStrings true
            } else {
                set PutStrings false
            }
            puts $MyFileVar "            \"hydrostatic\":          $PutStrings,"
            if {[lindex [lindex $Groups $i] 6] == "Y"} {
                set PutStrings 2
            } elseif {[lindex [lindex $Groups $i] 6] == "Z"} {
                set PutStrings 3
            } else {
                set PutStrings 1
            }
            puts $MyFileVar "            \"gravity_direction\":    $PutStrings,"
            puts $MyFileVar "            \"reference_coordinate\": [lindex [lindex $Groups $i] 7],"
            puts $MyFileVar "            \"specific_weight\":      [lindex [lindex $Groups $i] 8]"
            puts $MyFileVar "        \}"
            if {$MyGroupNum < $NumGroups} {
                puts $MyFileVar "    \},\{"
            } else {
                puts $MyFileVar "    \}\]"
            }
        }
    }
}

#-------------------------------------------------------------------------------

proc WriteLoadScalarProcess {FileVar GroupNum Groups EntityType VarName TableDict NumGroups} {
    upvar $FileVar MyFileVar
    upvar $GroupNum MyGroupNum

    for {set i 0} {$i < [llength $Groups]} {incr i} {
        set Entities [GiD_EntitiesGroups get [lindex [lindex $Groups $i] 1] $EntityType]
        if {[llength $Entities] > 0} {
            incr MyGroupNum
            puts $MyFileVar "        \"python_module\": \"apply_load_scalar_table_process\","
            puts $MyFileVar "        \"kratos_module\": \"KratosMultiphysics.PoromechanicsApplication\","
            puts $MyFileVar "        \"process_name\":  \"ApplyLoadScalarTableProcess\","
            puts $MyFileVar "        \"Parameters\":    \{"
            puts $MyFileVar "            \"mesh_id\":         0,"
            puts $MyFileVar "            \"model_part_name\": \"[lindex [lindex $Groups $i] 1]\","
            puts $MyFileVar "            \"variable_name\":   \"$VarName\","
            puts $MyFileVar "            \"value\":           [lindex [lindex $Groups $i] 3],"
            if {[GiD_AccessValue get gendata Strategy_Type]=="Arc-Length"} {
                puts $MyFileVar "            \"table\":           0"
            } else {
                puts $MyFileVar "            \"table\":           [dict get $TableDict [lindex [lindex $Groups $i] 1] Table0]"
            }
            puts $MyFileVar "        \}"
            if {$MyGroupNum < $NumGroups} {
                puts $MyFileVar "    \},\{"
            } else {
                puts $MyFileVar "    \}\]"
            }
        }
    }
}