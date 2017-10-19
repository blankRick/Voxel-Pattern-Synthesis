-----------------Terminologies ---------------------
-> solid voxel : A voxel which is kept in the set after applying the pattern over it.
-> empty voxel : A voxel which is removed after applying the pattern.
-> connected : Voxels are connected in 26-neighborhood.
-> k neighborhood of v : k is odd number, and k neighborhood means all voxels in given set, and also within 
					cube of length k, and v at center of cube.
=====================================================



-----------------Order of execution -----------------
1> Execute "preProc1.cpp" after compiling with g++.
2> Enter input as prompted.
3> After completion, Execute "preProc2.cpp", after compiling with g++.
4> Enter input as prompted.
5> Completion till this point concludes pre-processing for given voxel set, 

NOTE : If the voxel set is changed, then start from "1>" all over, else only
		if pattern is changed, then continue from "6>".

6> Execute "patternSyn.cpp", after compiling with g++ command.
=====================================================



-----------------OUTPUT -----------------------------
-> output is always "Cube.obj".
=====================================================



-----------------PROGRAM FILES ----------------------
-> "find_min_max.cpp" : Program for finding the bounding box of the object.
-> "preProc1.cpp" : Driver program for above program.
-> "normals.cpp" : Program for finding normals, and storing it in file "normals.txt", along with bounds.
					Takes into consideration 2k+1 neighborhood of the voxel, parameter k = 4.
-> "preProc2.cpp" : Driver program for "normals.cpp".



-> "applyPattern.cpp" : Program for applying the pattern, need not be connected, and storing the output,
						in file "tempout.txt", with voxel centers and tags, according to whether it was,
						marked solid(1) or empty(2).
-> "removeDup.cpp" : Program for removing duplicate voxel centers, in "tempout.txt".
-> "surfaceGraph.cpp" : Program for creating the adjacency list graph on all voxels, irrespective of
						solid or empty. Graph stored in file "graphout.txt".
-> "connectSamples.cpp" : Program for making the final output of voxels connected. Final output stored
							in "finout.txt", containing all the voxel centers.
-> "graphicsb.cpp" : Program to create obj file with given voxel centers.
-> "patternSyn.cpp" : Driver Program for all the above programs. Give input of required voxel set (pre-processed)
						and pattern both in txt format preferably.
=====================================================



------------------INPUT -----------------------------
-> voxel file (txt) : first line - number of voxels (n)
						followed by	n lines of voxel center "x y z" format.
-> pattern (txt) : first line - height (h)
					second line - width(w)
					followed by h lines , each line containing w lines of 1/0.
					1 - voxel is kept(solid)
					0 - voxel is removed(empty)
=====================================================



------------------EXAMPLE INPUTS --------------------
voxel set : bunny64.txt(CogWheel) , icosa20.txt(icosahedral)
pattern : pattern.txt(stripes pattern)
=====================================================