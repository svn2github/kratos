import os
import sys

kratos_benchmarking_path = '../../../benchmarking' 
sys.path.append(kratos_benchmarking_path)
import benchmarking


def Run():
	Msg = ""
	Text = "========== DEM ==========\n"

        ###############################################################################
	
	Text += "DEM element no damp test: "
	os.chdir("two_balls_no_damp.gid")	
	sys.path.append(os.getcwd())

	print "running the DEM two_balls_no_damp test..."
        Msg = benchmarking.RunBenchmark("two_balls_no_damp.py", "two_balls_no_damp_ref.txt")

        if (Msg == True):
            Text += "OK\n"
            print "two_balls_no_damp test succesful"
        else:
            Text += "FAILED\n"
            Text += Msg
            Text += "\n\n"
            print "two_balls_no_damp example test FAILED"


	os.chdir("..")
        ###############################################################################

	Text += "DEM element normal damp test: "
	os.chdir("two_balls_normal_damp.gid")	
	sys.path.append(os.getcwd())

	print "running the DEM two_balls_normal_damp test..."
        Msg = benchmarking.RunBenchmark("two_balls_normal_damp.py", "two_balls_normal_damp_ref.txt")

        if (Msg == True):
            Text += "OK\n"
            print "two_balls_normal_damp test succesful"
        else:
            Text += "FAILED\n"
            Text += Msg
            Text += "\n\n"
            print "two_balls_normal_damp example test FAILED"


	os.chdir("..")
        ###############################################################################

	Text += "DEM element rotation no tangential damp test: "
	os.chdir("rotating_ball_no_tangent_damp.gid")	
	sys.path.append(os.getcwd())

	print "running the DEM rotating_ball_no_tangent_damp test..."
        Msg = benchmarking.RunBenchmark("rotating_ball_no_tangent_damp.py", "rotating_ball_no_tangent_damp_ref.txt")

        if (Msg == True):
            Text += "OK\n"
            print "rotating_ball_no_tangent_damp test succesful"
        else:
            Text += "FAILED\n"
            Text += Msg
            Text += "\n\n"
            print "rotating_ball_no_tangent_damp test FAILED"


	os.chdir("..")
        ###############################################################################

	Text += "DEM element rotation with rolling friction test: "
	os.chdir("rotating_ball_rolling_friction.gid")	
	sys.path.append(os.getcwd())

	print "running the DEM rotating_ball_rolling_friction test..."
        Msg = benchmarking.RunBenchmark("rotating_ball_rolling_friction.py", "rotating_ball_rolling_friction_ref.txt")

        if (Msg == True):
            Text += "OK\n"
            print "rotating_ball_rolling_friction test succesful"
        else:
            Text += "FAILED\n"
            Text += Msg
            Text += "\n\n"
            print "rotating_ball_rolling_friction test FAILED"


	os.chdir("..")
        ###############################################################################

	# Add other examples here



        ################################################################
        print "resume of all of the examples for the DEM application :"
        print Text
	return Text

if __name__=='__main__':
  Run();


