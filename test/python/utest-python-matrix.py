from coolfluid import *

vec1=RealVector3()
unittest.check_equal(len(vec1), 3, 'Static vector size test')

vec2=RealVector(5)
unittest.check_equal(len(vec2), 5, 'Dynamic vector size test')

mat1=RealMatrix3()
unittest.check(mat1.rows() == 3 and mat1.cols() == 3, 'Static matrix size test')

mat2=RealMatrix(5,3)
unittest.check(mat2.rows() == 5 and mat2.cols() == 3, 'Dynamic matrix size test')

vec1[0]=1.0
unittest.check_equal(vec1[0], 1.0, 'vector assignation test')

mat1[0,0]=1.0
unittest.check_equal(mat1[0,0], 1.0, 'matrix assignation test')

vec2.resize(len(vec1))
unittest.check_equal(len(vec2), 3, 'vector resize test')

mat2.resize(mat1.rows(),mat1.cols())
unittest.check(mat2.rows() == 3 and mat2.cols() == 3, 'matrix resize test')

