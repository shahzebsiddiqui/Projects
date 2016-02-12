# Shahzeb Siddiqui
# Date: 02/12/2016

import numpy
import math
# f(x) = x
def myfunc1(a):
	return a;
# f(x) = sin(x)
def myfunc2(a):
        return math.sin(a)
# f(x) = log(x)
def myfunc3(a):
	return math.log(x)
print "f(x) = x                          f(x) = sin(x)                         f(x) = log(x)"
print "Sample       Area               Sample       Area               Sample       Area"                   
# get a list of evenly spaced values between 10,1000 with a step of 100
samples = numpy.arange(4,100,4)
N = 1

for x in numpy.nditer(samples):
	data = numpy.linspace(0,N,x,endpoint=True)
	sample_interval = data[1] - data[0]

	# apply function to numpy array so that we can calcuate f(x) of each sample point
	function1 =  numpy.vectorize(myfunc1)
 	function2 =  numpy.vectorize(myfunc2)
 	function3 =  numpy.vectorize(myfunc3)

	# returns a numpy array of the applied function
 	function_data1 = function1(data)
 	function_data2 = function2(data)
 	function_data3 = function3(data)

	# Applying rectangular integral approximation by multiplying f(x) with step size
	sample_mult_function1 = numpy.multiply(function_data1,sample_interval)
	sample_mult_function2 = numpy.multiply(function_data2,sample_interval)
	sample_mult_function3 = numpy.multiply(function_data3,sample_interval)

	area1 = numpy.sum(sample_mult_function1)
	area2 = numpy.sum(sample_mult_function2)
	area3 = numpy.sum(sample_mult_function3)
	# print sample_mult_function
	print x, "\t", area1, "\t\t",  x,"\t", area2, "\t\t",  x, "\t", area3
