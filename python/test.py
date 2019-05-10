#!/usr/local/bin/python3

import os

def funcA(A):
    print("function A")
    def resFunA(a):
        print("resFunA:a=",a)
    return resFunA

def funcB(B):
    print(B(2))
    print("function B")
    def resFunB(a):
        print("resFunb:a=",a)
    return resFunB

@funcA
@funcB
def func(c):
    # print("function C %d",{c})
    print("function c ",c)
    return c**2

func(2)
exit


def square_sum(fn):
    def square(a):
        print("args a=", a)
    print("square_sum")
    return square


@square_sum
def sum_a(a):
    print("3=", a)
    
# sum_a(10)

# print("00000")
