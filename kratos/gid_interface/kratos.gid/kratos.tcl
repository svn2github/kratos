########################################################################
#    KRATOS: GiD interface for the Kratos problem type
########################################################################
#
#    NAME: Kratos.tcl
#
#    PURPOSE: Init script for Kratos problem type
#
#    AUTHOR : G. Socorro
#
#    CREATED AT: 01/11/09
#
#    HISTORY: 
# 
#     3.7- 19/06/13- G. Socorro, delete the event AfterRenameCondGroup, AfterCreateCondGroup and BeforeDeleteCondGroup used in the Compass library
#     3.6- 18/06/13- G. Socorro, delete the global variable KPriv(NewGiDGroups)
#     3.5- 17/06/13- G. Socorro, modify the proc BeforeMeshGeneration to delete the old Compass condition
#     3.4- 14/06/13- G. Socorro, modify the procs CheckRequiredGiDVersion
#     3.3- 18/06/13- A. Melendo, simplify the proc BeforeMeshGeneration to write conditional data condition in all applications
#     3.2- 19/04/13- G. Socorro, modify the proc BeforeMeshGeneration to write conditional data condition in the fluid application
#     3.1- 26/11/12- J. Garate,  BeforeMeshGeneration modified, support PFEM Application
#     3.0- 12/11/12- J. Garate, Minor Fixing
#     2.9- 07/11/12- J. Garate, GiD 11.1.2d is the minimum Required version for New GiD Groups
#     2.8- 29/10/12- G. Socorro, Add the proc AfterCreateGroup
#     2.7- 22/10/12- J. Garate, Function correction 
#     2.6- 17/10/12- J. Garate, Correction when transferring old Cond_Groups to GiD_Groups
#     2.5- 10/10/12- J. Garate, Adapatation for New GiD_Groups
#     2.4- 10/10/12- G. Socorro, update the proc SelectGIDBatFile to include the structural analysis bat files for MPI and OpenMP
#     2.3- 08/10/12- J. G�rate, enable/disable New GiD Groups module KPriv(NewGiDGroups) [0 -> Old Groups  | 1 -> New Groups]
#     2.2- 04/10/12- G. Socorro, update the proc SelectGIDBatFile to include the LevelSet bat file 
#     2.1- 04/10/12- G. Socorro, add the proc BeforeDeleteCondGroup,AfterCreateCondGroup and AfterRenameCondGroup
#     2.0- 01/10/12- J. G�rate, enable/disable curves module KPriv(CurvesModule) [0 -> Disable  | 1 -> Enable]
#     1.9- 28/09/12- G. Socorro, modify the event SelectGIDBatFile tp include the number of threads for the OpenMP case
#     1.8- 24/09/12- G. Socorro, modify the event SelectGIDBatFile to include the number of processors in the command line
#     1.7- 21/09/12- G. Socorro, correct a bug in the proc BeforeMeshGeneration use {*} to get all the surface identifier
#     1,6- 22/07/12- G. Socorro, modify the BeforeMeshGeneration to automatically mesh de boundary lines/surfaces when use Is-Slip BC
#     1.5- 09/05/12- G. Socorro, use conditions only in the fluid application
#     1.4- 07/05/12- G. Socorro, update the proc BeforeMeshGeneration to write the Condition2D and Condition3D properties
#     1.3- 04/05/12- G. Socorro, update the proc BeforeDeleteGroup
#     1.2- 03/05/12- J. Garate, proc EndGIDPostProces
#     1.1- 03/05/12- G. Socorro, load the new gid_groups package
#     1.0- 23/03/12- J.G�rate, No se puede cancelar un renombre
#     0.9- 20/03/12- J.G�rate, Group Event Procedures
#     0.8- 16/11/11- G. Socorro, add the global variable KPriv(SRCConfig) to use TCL or TBE file distribution
#     0.7- 22/06/11- G. Socorro, add the proc KLoadTBEFiles to load the sources from TBE files
#     0.6- 03/02/11- G. Socorro, update the procedure UnsetGlobalVars
#     0.5- 08/09/10- G. Socorro, add the event InitGIDPostProcess to read Kratos result files when pass from preproces to postprocess
#     0.4- 03/09/10- G. Socorro, add BeforeMeshGeneration option to modify the normal to the line and surfaces
#     0.3- 01/02/10- G. Socorro, add a new global procedure msg to call WarnWinText
#     0.2- 24/12/09- G. Socorro, add new xmlutils, xpathq and wkcf namespaces
#     0.1- 01/11/09- G. Socorro, create the base source file
#
########################################################################

proc ReadSomePTData {dir} {
	
    global ProgramName VersionNumber MinimumGiDVersion

    dom parse [tDOM::xmlReadFile [file join $dir kratos.xml]] doc
    
    set ProgramName [$doc selectNodes string(Infoproblemtype/Program/Name)]
    set VersionNumber [$doc selectNodes string(Infoproblemtype/Program/Version)]
    set MinimumGiDVersion [$doc selectNodes string(Infoproblemtype/Program/MinimumGiDVersion)]

    $doc delete
}


proc UnsetGlobalVars {} {
	
    global KData KPriv MinimumGiDVersion
    global VersionNumber ProgramName

    foreach arrid [list KData KPriv VersionNumber ProgramName MinimumGiDVersion] {
	if {[info exists $arrid]} {
	    unset $arrid
	}
    }
}

proc CheckRequiredGiDVersion {VersionRequired} {
    set ok 1
   
    set comp -1
    catch { 
	set comp [::GidUtils::VersionCmp $VersionRequired]
    }
    if { $comp < 0 } {
	WarnWin [= "Error: This interface requires GiD %s or later" $VersionRequired].
	set ok 0
    }
    return $ok
}

proc LoadGIDProject {filename} {
    ::kfiles::LoadSPD $filename
}

proc SaveGIDProject {filename} {
    ::kfiles::SaveSPD $filename
}

proc AfterTransformProblemType { file oldproblemtype newproblemtype } {
	set name [lindex [split $file "/"] end]
	#msg "${file}/${name}.spd"
	::kfiles::LoadSPD "${file}.gid/${name}.spd"
	return 0
}

proc BeforeTransformProblemType { file oldproblemtype newproblemtype } {
    #WarnWinText 2
	#msg "before transform"
	#global KPriv
	#msg "$KPriv(dir)"
	#set path [GiD_Info problemtypepath]
	#set name [lindex [split $file "/"] end]
	#
	#msg "after transform path:$path name:$name\n$file $oldproblemtype $newproblemtype"
	#
	##LoadGIDProject "$file/kratos_default.spd"
	#
	#
	##
	###Transforma el spd si son versiones distintas
	 #::xmlutils::checkSpdVersion
	 #
	 return -cancel-
}

proc InitGIDProject { dir } {
    global KData KPriv
    global VersionNumber ProgramName MinimumGiDVersion

    # Unset global variables
    UnsetGlobalVars
    
    # Read kratos.xml file
    ReadSomePTData $dir
    
    # WarnWinText "VersionNumber:$VersionNumber ProgramName:$ProgramName MinimumGiDVersion:$MinimumGiDVersion"
    # Check the required GiD version
    set VersionRequired "$MinimumGiDVersion"
    set isvalidversion [CheckRequiredGiDVersion $VersionRequired]
    if {!$isvalidversion} {
	# return ""
    }
    
    # Init packages
    SRC gid_groups_public.tcl
    gid_groups_conds::init_package
    
    # For activating the Curves Module [Disabled -> 0 | Enabled -> 1]
    set KPriv(CurvesModule) 0
    

    
    # For release/debug options [Release =>1|Debug => 0]
    set KPriv(RDConfig) 1
    # For distribution srctcl/srctbe options [srctbe =>1|srctcl => 0]
    set KPriv(SRCConfig) 0

    # Load the application scripts 
    if {!$KPriv(SRCConfig)} {
	# For scripts directory
	set scriptspath "$dir/scripts/"
	if { [catch {source $scriptspath/initptype.tcl}] } {
	    return 0
	} else {
	    # Init some xml global variables
	    ::kipt::InitGlobalXMLVariables
	    
	    ::kipt::LoadSourceFiles $dir
	}
    } else {
	# Load tbe files
	KLoadTBEFiles $dir

	# Init some xml global variables
	::kipt::InitGlobalXMLVariables
    }

    # Init problem type
    ::kipt::InitPType $dir
}

proc EndGIDProject {} {

    # Free problem type
    ::kipt::FreePType
}

proc BeforeWriteCalcFileGIDProject { file } {
 
    #return 1 ;
    return [::KMValid::ValidateModel]

}

proc AfterWriteCalcFileGIDProject {filename errorflag } {
    # WarnWinText "AfterWriteCalcFileGIDProject\n filename:$filename\n errorflag:$errorflag"
    
    set ret 1

    # Try to write the Kratos input data file
    set err [catch { ::wkcf::WriteCalculationFiles $filename} ret]
    if { $err } {
	snit_messageBox -parent .gid -message \
	    [= "Error when preparing data for analysis (%s)" $ret]
	return "-cancel-"
    }
    return $ret
}

proc msg {mesage} {
    
    WarnWinText $mesage
}

proc msgS {mesage} {
    
    WarnWin $mesage
}

proc wa {mesage} {
    
    WarnWinText $mesage
}

proc BeforeMeshGeneration {elementsize} { 
    global KPriv
    
    ::GidUtils::DisableGraphics

    
    set ndime "3D"
    # Get the spatial dimension
    set cxpath "GeneralApplicationData//c.Domain//i.SpatialDimension"
    set cproperty "dv"
    catch { set ndime [::xmlutils::setXml $cxpath $cproperty] }
 
   
    # Reset Automatic Conditions from previous executions
    GiD_Process Mescape Meshing MeshCriteria DefaultMesh Lines 1:end
    GiD_Process Mescape Meshing MeshCriteria DefaultMesh Surfaces 1:end escape
    #end common
    
	if {$ndime =="2D"} {
	    
	    # Align the normal
	    ::wkcf::AlignLineNormals Outwards 
	    
	    # Reset Automatic Conditions from previous executions 
	    set entitytype "line"
	    
	   
		# Automatic Kratos Group for Boundary Condition
		set groupid "-AKGSkinMesh2D"
		::wkcf::CleanAutomaticConditionGroupGiD $entitytype $groupid
	 
	 
		    # Find boundaries
		    set blinelist [::wkcf::FindBoundaries $entitytype]
		    # wa "belist:$blinelist"
		    
		    # Automatically meshing all the boundary lines
		    GiD_Process Mescape Meshing MeshCriteria Mesh Lines {*}$blinelist escape 
		    
		    # Assign the boundary condition
		    ::wkcf::AssignConditionToGroupGID $entitytype $blinelist $groupid

	    
	} elseif {$ndime =="3D"} {
	    
	    # Align the normal
	    ::wkcf::AlignSurfNormals Outwards
	    
	    # Reset Automatic Conditions from previous executions 
	    set entitytype "surface"
	    
      # Automatic Kratos Group for Boundary Condition
      set groupid "-AKGSkinMesh3D"
      ::wkcf::CleanAutomaticConditionGroupGiD $entitytype $groupid

		    # Find boundaries
		    set bsurfacelist [::wkcf::FindBoundaries $entitytype]
		    # WarnWinText "bsurfacelist:$bsurfacelist"
		    
		    # Assign the triangle element type
		    GiD_Process Mescape Meshing ElemType Triangle $bsurfacelist escape 
		    
		    # Automatically meshing all the boundary surfaces
		    GiD_Process Mescape Meshing MeshCriteria Mesh Surfaces {*}$bsurfacelist escape 
		    
		    ::wkcf::AssignConditionToGroupGID $entitytype $bsurfacelist $groupid

    }
    
    ::GidUtils::EnableGraphics
    
}

proc InitGIDPostProcess {} { 

    set ::KMProps::RestoreWinFromPost 0
    if {[info exists ::KMProps::Layout]} {
	if {($::KMProps::Layout eq "INSIDE_LEFT") ||($::KMProps::Layout eq "INSIDE_RIGHT")} {
	    set w ".gid.kmprops" 
	    if {[winfo exists $w]} {
	    destroy $w
	    set ::KMProps::RestoreWinFromPost 1
	    }
	    }
    }

    # Get application type
    # Structural analysis
    set cxpath "GeneralApplicationData//c.ApplicationTypes//i.StructuralAnalysis"
    set cproperty "dv"
    set StructuralAnalysis [::xmlutils::setXml $cxpath $cproperty]
    
    # WarnWinText "StructuralAnalysis:$StructuralAnalysis"

    # Fuild application
    set cxpath "GeneralApplicationData//c.ApplicationTypes//i.Fluid"
    set cproperty "dv"
    set FluidApplication [::xmlutils::setXml $cxpath $cproperty]

    # WarnWinText "FluidApplication:$FluidApplication"
    set appid ""
    if {$FluidApplication =="Yes"} {
	set appid "Fluid"
    } elseif {$StructuralAnalysis=="Yes"} {
	set appid "StructuralAnalysis"
    }

    if {$appid !=""} {
	# Get the result type
	set cprop "GiDMultiFileFlag"
	set cxpath "$appid//c.Results//c.GiDOptions//i.${cprop}"
	     set cproperty "dv"
	     set rtype [::xmlutils::setXml $cxpath $cproperty]
	# WarnWinText "rtype:$rtype"
	
	# Get the GiD post mode
	set cprop "GiDPostMode"
	set cxpath "$appid//c.Results//c.GiDOptions//i.${cprop}"
	     set cproperty "dv"
	     set pmode [::xmlutils::setXml $cxpath $cproperty]
	# WarnWinText "pmode:$pmode"

	set existfiles [::KUtils::ReadResultsFromFiles $appid $rtype $pmode "CheckRFiles"]
	if {!$existfiles} {
	    WarnWin [= "The simulation is not calculated yet or is currently being calculated"].
	    return ""
	} else {
	    # Try to read the result files
	    set ok [::KUtils::ReadResultsFromFiles $appid $rtype $pmode "ReadRFiles"]
	}
    } 
}

proc KLoadTBEFiles {dir} {
    
    # For scripts directory
    set scriptspath "$dir/scripts/"
    cd $scriptspath
      
    set dirlist [glob *]
    # WarnWinText "dirlist:$dirlist\n\n"
    foreach cdir $dirlist {
	if {[file isdirectory $cdir]} {
	    cd $cdir
	    set tbelist ""
	    catch { set tbelist [glob *] }
	    # WarnWinText "cdir:$cdir => tbelist:$tbelist\n"
	    if {[llength $tbelist]} {
		foreach tbe_level1 $tbelist {
		    # WarnWinText "Current tbe_level1:$tbe_level1"
		    if {[file isdirectory $tbe_level1]} {
		        cd $tbe_level1
		        set tbe_l2list ""
		        catch { set tbe_l2list [glob *] }
		        # WarnWinText "tbe_l2list:$tbe_l2list"
		        if {[llength $tbe_l2list]} {
		            foreach tbe_level2 $tbe_l2list {
		                if {[file extension $tbe_level2]==".tbe"} {
		                    # WarnWinText "current l2 tbe_l2list:$tbe_level2"
		                    loadtbefile $tbe_level2                    
		                }
		            }
		        }
		        cd ..
		    } else {
		        if {[file extension $tbe_level1]==".tbe"} {
		            # WarnWinText "current level1:$tbe_level1\n"
		            loadtbefile $tbe_level1
		        }
		    }
		}
	    }
	    cd ..
	} else {
	    if {[file extension $cdir]==".tbe"} {
		# WarnWinText "current cdir:$cdir\n"
		loadtbefile $cdir
	    }
	}
    }
} 

 
proc BeforeDeleteGroup { name } {
    
    # wa "delete name:$name"
    set DeleteGroup "Delete" 
    if {[info exists ::KPriv(Groups,DeleteGroup)]} {
	if {$::KPriv(Groups,DeleteGroup)} {
	    set DeleteGroup [::KEGroups::BorraGrupo $name]
	} 
    }
    if { $DeleteGroup eq "-cancel-" } {
	return $DeleteGroup
    }
 }
 
proc AfterRenameGroup { oldname newname } {
    # V�lida para los grupos de GiD 11.1.1d
     # wa "oldname:$oldname newname:$newname"
    ::KEGroups::RenombraGrupo $oldname $newname 1
    #Si se renombra un grupo, no nos queda otra... no se puede impedir.
 }


proc EndGIDPostProcess { } {
    
    # Try to restore the properties window
    if {[info exists ::KMProps::RestoreWinFromPost]} {
	if {$::KMProps::RestoreWinFromPost} {
	    ::KMProps::StartBaseWindow
	    }
    }
}

proc SelectGIDBatFile {directory basename } {

    set batfilename ""
    set args ""
 
    # Get application type
    # Structural analysis
    set cxpath "GeneralApplicationData//c.ApplicationTypes//i.StructuralAnalysis"
    set cproperty "dv"
    set StructuralAnalysis [::xmlutils::setXml $cxpath $cproperty]
    
    # WarnWinText "StructuralAnalysis:$StructuralAnalysis"
    
    # Fuild application
    set cxpath "GeneralApplicationData//c.ApplicationTypes//i.Fluid"
    set FluidApplication [::xmlutils::setXml $cxpath $cproperty]
    
    # WarnWinText "FluidApplication:$FluidApplication"
    
    # Structural analyis
    if {$StructuralAnalysis eq "Yes"} {
	set rootid "StructuralAnalysis"
	# Kratos key word xpath
	set kxpath "Applications/$rootid"
	# Get the parallel solution type
	set cxpath "$rootid//c.SolutionStrategy//i.ParallelSolutionType"
	set ParallelSolutionType [::xmlutils::setXml $cxpath $cproperty]
	
	# Solution type
	set cxpath "$rootid//c.AnalysisData//i.SolutionType"
	set SolutionType [::xmlutils::setXml $cxpath $cproperty]
	# wa "SolutionType:$SolutionType"

	if {$ParallelSolutionType eq "MPI"} {
	    if {($SolutionType =="Dynamic")||($SolutionType =="RelaxedDynamic")} {
		if {($::tcl_platform(os) eq "Linux")} {
		    set batfilename "kratos-structuraldynamic-mpi.unix.bat"
		} else {
		    # set batfilename "kratos-structuraldynamic-mpi.win.bat"
		}
	    } elseif {$SolutionType =="Static"} {
		if {($::tcl_platform(os) eq "Linux")} {
		    set batfilename "kratos-structuralstatic-mpi.unix.bat"
		} else {
		    # set batfilename "kratos-structuralstatic-mpi.win.bat"
		}
	    }

	    #  Get the number of processors
	    set cxpath "$rootid//c.SolutionStrategy//i.MPINumberOfProcessors"
	    set MPINumberOfProcessors [::xmlutils::setXml $cxpath $cproperty]
	    # wa "MPINumberOfProcessors:$MPINumberOfProcessors"
	    if {$MPINumberOfProcessors>0} {
		# Calculate arguments
		set args "$MPINumberOfProcessors"
	    }
	} else {
	    # OpenMP
	    #  Get the number of threads
	    set cxpath "$rootid//c.SolutionStrategy//i.OpenMPNumberOfThreads"
	    set OpenMPNumberOfThreads [::xmlutils::setXml $cxpath $cproperty]
	    # wa "OpenMPNumberOfThreads:$OpenMPNumberOfThreads"
	    if {$OpenMPNumberOfThreads>0} {
		# Calculate arguments
		set args "$OpenMPNumberOfThreads"
	    }
	    if {($SolutionType =="Dynamic")||($SolutionType =="RelaxedDynamic")} {
		if {($::tcl_platform(os) eq "Linux")} {
		    set batfilename "kratos-structuraldynamic-openmp.unix.bat"
		} else {
		    set batfilename "kratos-structuraldynamic-openmp.win.bat"
		}
	    } elseif {$SolutionType =="Static"} {
		if {($::tcl_platform(os) eq "Linux")} {
		    set batfilename "kratos-structuralstatic-openmp.unix.bat"
		} else {
		    set batfilename "kratos-structuralstatic-openmp.win.bat"
		}
	    }
	}
    }

    # Fluid application
    if {$FluidApplication eq "Yes"} {
	set rootid "Fluid"
	# Kratos key word xpath
	set kxpath "Applications/$rootid"
	# Get the parallel solution type
	set cxpath "$rootid//c.SolutionStrategy//i.ParallelSolutionType"
	set ParallelSolutionType [::xmlutils::setXml $cxpath $cproperty]
	
	# Free surface
	set cxpath "$rootid//c.AnalysisData//i.FreeSurface"
	set FreeSurface [::xmlutils::setXml $cxpath $cproperty]
	# wa "FreeSurface:$FreeSurface"

	# Solver type for free surface
	set cxpath "$rootid//c.AnalysisData//i.SolverTypeFreeSurf"
	set SolverTypeFreeSurf [::xmlutils::setXml $cxpath $cproperty]
	# WarnWinText "SolverTypeFreeSurf:$SolverTypeFreeSurf"

	if {$ParallelSolutionType eq "MPI"} {
	    if {($::tcl_platform(os) eq "Linux")} {
		set batfilename "kratos-mpi.unix.bat"
		#  Get the number of processors
		set cxpath "$rootid//c.SolutionStrategy//i.MPINumberOfProcessors"
		set MPINumberOfProcessors [::xmlutils::setXml $cxpath $cproperty]
		# wa "MPINumberOfProcessors:$MPINumberOfProcessors"
		if {$MPINumberOfProcessors>0} {
		    # Calculate arguments
		    set args "$MPINumberOfProcessors"
		}
	    }
	} else {
	    # OpenMP
	    #  Get the number of threads
	    set cxpath "$rootid//c.SolutionStrategy//i.OpenMPNumberOfThreads"
	    set OpenMPNumberOfThreads [::xmlutils::setXml $cxpath $cproperty]
	    # wa "OpenMPNumberOfThreads:$OpenMPNumberOfThreads"
	    if {$OpenMPNumberOfThreads>0} {
		# Calculate arguments
		set args "$OpenMPNumberOfThreads"
	    }
	    if {($FreeSurface eq "Yes") && ($SolverTypeFreeSurf eq "LevelSet")} {
		if {($::tcl_platform(os) eq "Linux")} {
		    set batfilename "kratos-openmplevelset.unix.bat"
		} else {
		    set batfilename "kratos-openmplevelset.win.bat"
		}
	    } else {
		if {($::tcl_platform(os) eq "Linux")} {
		    set batfilename "kratos.unix.bat"
		} else {
		    set batfilename "kratos.win.bat"
		}
	    }
	}
    }
    
    set ret "$batfilename $args"
    # set ret "$batfilename"
    # wa "\n\nret:$ret" 
    if {$batfilename != ""} {
	return $ret
    } else {
	return ""
    }
}  
