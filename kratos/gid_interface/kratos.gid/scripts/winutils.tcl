###############################################################################
#
#    NAME: kwinutils.tcl
#
#    PURPOSE: TCL script with utilities procedures to work with window geometry 
#
#    QUANTECH - DEVELOPMENT DEPARTMENT
#
#    AUTHOR : G. Socorro
#
#    CREATED AT: 04/11/09
#
#    LAST MODIFICATION : create the base source code
#
#    VERSION : 0.1
#
#    HISTORY:
#
#     0.1-04/11/09-G. Socorro, create the base source code
#
###############################################################################

#******************************************************************************
#                  BEGIN WinUtils NAMESPACE
#******************************************************************************

namespace eval ::WinUtils:: {
    
}

# List of procedure that is inside the WinUtils namespace
# 1. 
# 2.

proc ::WinUtils::confirmBox {w txt {option okcancel} } {
	
    global msgboxIcon msgboxType
    
    if { $option == "okcancel" } {
    	set button [tk_messageBox -type okcancel \
		-title "Eliminar" -parent $w \
		-message $txt]
	} else {
		set button [tk_messageBox -type $option \
		-title "Eliminar" -icon warning -parent $w \
		-message $txt]
	}
	return $button
}

proc ::WinUtils::WarningLine {state} {
    # Enable or disabled the warning line
    # State 1 => Disable
    #       0 => Enable

    if {$state =="Enable"} {
	# Check the GiD state
	set gwhat [.central.s disable warnline]
	if {$gwhat =="1"} {
	    .central.s disable warnline 0 
	}
    } elseif {$state =="Disable"} {
	# Check the GiD state
	set gwhat [.central.s disable warnline]
	if {$gwhat =="0"} {
	    .central.s disable warnline 1 
	}
    }
}

proc ::WinUtils::PrintArray {a {pattern *}} {
    # ABSTRACT:
    # Print the content of array a to WarnWinText window
    
    upvar 1 $a array  
    if {![array exists array]} {
	error "\"$a\" isn't an array"
    }
    set maxl 0
    foreach name [lsort [array names array $pattern]] {
	if {[string length $name] > $maxl} {
	    set maxl [string length $name]
	}
    }
    set maxl [expr {$maxl + [string length $a] + 2}]
    foreach name [lsort [array names array $pattern]] {
	set nameString [format %s(%s) $a $name]
	WarnWinText "[format "%-*s = %s" $maxl $nameString $array($name)]"
    }
}

proc ::WinUtils::GetImage { imageid } {
    # ABSTRACT:
    # Get images from the image subdirectory
    global KPriv
	
	if { $imageid != "" && [file exists [file native "$KPriv(dir)/images/${imageid}"]] } {
    	#msg "imgId:$imageid [Bitmap::get $KPriv(dir)/images/${imageid}]"
    	return [Bitmap::get [file native "$KPriv(dir)/images/${imageid}"]]
    } else {
    	#msg "NoImage(-1)"
    	return -1
    }
}


proc ::WinUtils::Print {w} {

    if {$::tcl_platform(platform) ne "windows"} {
    WarnWin [= "Printing is only supported under Windows"]
    return 0
    }

    set tempdir ""
    set tempdir [::KUtils::GiveTempDir]
    if {$tempdir ==""} {
    WarnWin [= "An error has been detected when get a temporal directory"]
    return 0
    }
    set filename $tempdir/print.txt
    # Save the license information to this file
    if { [catch { set fileid [open $filename w] }] } {
    WarnWin [= "Cannot write file %s. Permission denied" $file].
    return 0
    }
    fconfigure $fileid -encoding utf-8
    puts $fileid \ufeff[$w get 1.0 end]
    close $fileid
    exec cmd /c start /min notepad /p $filename
    file delete $filename
 }

 proc ::WinUtils::ConfigureListScrollbars {listbox sx sy } {
    # WarnWinText "listbox:$listbox sx:$sx sy:$sy"
    foreach i "x y" {
    if { ![info exists s${i}] || ![winfo exists [set s${i}]] } { continue }
    foreach "${i}1 ${i}2" [$listbox ${i}view] break
    if { [set ${i}1] == 0 && [set ${i}2] == 1 } {
        after idle grid remove [set s${i}]
    } else {
        # WarnWinText "what set:[set s${i}]\n i:$i\n all:[grid info $listbox]"
        after idle grid [set s${i}]
        if {$i=="y"} {
        # set findrow [lindex [grid info $listbox] [expr [lsearch [grid info $listbox] -column]+1]]
        set findcol [lindex [grid info $listbox] 3]
        # WarnWinText "findcol:$findcol by:[lsearch [grid info $listbox] -column]"
        after idle grid configure [set s${i}] -column [expr $findcol+1]
        } elseif {$i=="x"} {
        set findrow [lindex [grid info $listbox] 5]
        # WarnWinText "findrow:$findrow by:[lsearch [grid info $listbox] -column]"
        after idle grid configure [set s${i}] -row [expr $findrow+1]
        }
    }
    }
}