from xlutils.copy import copy
import xlrd
import xlwt

#xlutils:修改excel

def nthUglyNumber2(n):

    if(n < 5):
        return n

    array=[i+1 for i in range(n)]
    # print(array)
    index2=1;
    index3=0;
    index5=0;
    index=0;
    minN = 5;
    minNumber = 0;
    while(index != n):

        minNumber = array[index2]
        if(minNumber > array[index3]):
            minNumber = array[index3];
        if (minNumber > array[index5]):
            minNumber = array[index5];


        if (minNumber == array[index2]):
            array[index2+1] = array[index2] * 2;
            index2 += 1;


        if (minNumber == array[index3]):
            array[index3 + 1] = array[index3] * 3;
            index3 += 1;
        if (minNumber == array[index5]):
            array[index5 + 1] = array[index5] * 5;
            index5 += 1;

        array[index] = minNumber;
        print("index %d minNumber = %d" % (index, minNumber))
        index += 1;

    print(array)
    return minNumber

def nthUglyNumber(n):
    if (n < 7):
        return n

    array = [i + 1 for i in range(n)]
    index2 = 2;
    index3 = 1;
    index5 = 1;

    next2 = array[index2] * 2;
    next3 = array[index3] * 3;
    next5 = array[index5] * 5;
    index = 5;
    min = next2;
    while(index != n):
        min = next2;
        if(min > next3):
            min = next3
        if (min > next5):
            min = next5

        if min == next2:
            index2 += 1
        if min == next3:
            index3 += 1
        if min == next5:
            index5 += 1

        array[index] = min;
        index += 1

        next2 = array[index2] * 2;
        next3 = array[index3] * 3;
        next5 = array[index5] * 5;

    return min



def test2(n):
    array_first = [0 for i in range(6 * n + 1)]
    array_last = [0 for i in range(6 * n + 1)]
    array_current=None
    for k in range(1, n+1):
        print("k %d" % (k))
        array_pre=None
        if k  % 2 == 0:
            array_current=array_last
            array_pre=array_first
        else:
            array_current=array_first
            array_pre = array_last

        if k == 1:
            for i in range(1, 7):
                array_current[i] = 1
            continue

        for i in range(1,7):
            pre_count= 5 * (k - 1)+1
            for j in range(1,pre_count+1):
                index=k+i+j-2
                if (index <= pre_count):
                    if i == 1:
                        array_current[k+i+j-2]=array_pre[index - 1]
                    else:
                        array_current[index] += 1

                else:
                    if i == 1:
                        array_current[index] = 1
                    else:
                        array_current[index] += 1
    #
    for i in range(0, n):
        array_current[i]=0

    sum=0
    for i in array_current:
        sum += i;
    print(sum)
    print(array_first)
    print(array_last)
    # print(array_current)
    return array_current

class Solution:
    # @param {int} n an integer
    # @return {tuple[]} a list of tuple(sum, probability)
    def dicesSum(self, n):
        array_first = [0 for i in range(6 * n + 1)]
        array_last = [0 for i in range(6 * n + 1)]
        array_current=None
        for k in range(1, n+1):
            print("k %d" % (k))
            array_pre=None
            if k  % 2 == 0:
                array_current=array_last
                array_pre=array_first
            else:
                array_current=array_first
                array_pre = array_last

            for i in range(1, 6 * k):
                array_current[i] = 0;

            if k == 1:
                for i in range(1, 7):
                    array_current[i] += 1
                continue

            for i in range(1,7):
                for j in range(k - 1,6 * k - 6 + 1):
                    index=i+j
                    array_current[index] += array_pre[j]

        total=6;
        for i in range(1,n):
            total *= 6;

        res = []
        index=0
        for a in array_current:

            if a != 0:
                res.append([index,a * 1.0/ total])
            index += 1;
        # print(res)
        return res


def maxSubArray2(nums):

        length=len(nums);
        if len == 0:
            return 0
        if len == 1:
            return nums[0]

        sum=nums[0]
        currentSum=sum
        for i in range(1,length):
            currentSum += nums[i]
            if(currentSum > sum):
                sum=currentSum
            elif (currentSum < 0):
                currentSum=0

        return currentSum

def maxSubArray(nums):
        length = len(nums);
        if length == 0:
            return 0
        if length == 1:
            return nums[0]
        sum=nums[0];
        maxSum=nums[0];
        for i in range(1,length):
            if(sum < 0):
                sum=nums[i]
            else:
                sum +=nums[i]
            if sum > maxSum:
                maxSum = sum

        return maxSum


def flatten(nestedList):
        res=[]
        list_data=[]
        list_data.append(nestedList)
        # for i in nestedList:
        #     if isinstance(i,list):
        #         list_data.append(i)
        #     else
        #         res.append(i)
        while(list_data):
            pop_this = list_data.pop(0)
            # print(pop_this)
            if isinstance(pop_this, list):
                index = 0
                for i in pop_this:
                    list_data.insert(index,i)
                    index += 1
            else:
                res.append(pop_this)

        return  res

def kthLargestElement(k, A):
        if k < 1:
            return 0;
        if len(A) < k:
            return 0;

        temp_array=A
        tempK = k

        while(temp_array):
            a0=temp_array[0]
            current_array = []
            pre_array = []
            sub_array = []
            for i in temp_array:
                if(i > a0):
                    pre_array.append(i)
                elif i == a0:
                    current_array.append(i)
                else:
                    sub_array.append(i)
            if len(pre_array) >= tempK:
                temp_array=pre_array
            else:
                tempK -= len(pre_array);
                if len(current_array) >= tempK:
                    return current_array[0]
                else:
                    tempK -= len(current_array)
                    temp_array = sub_array
        return 0


def mergeSortedArray(A, B):
        lenA = len(A);
        lenB = len(B);
        if lenA <= 0:
            return B;
        if lenB <= 0:
            return A;

        moreArray = (B if(lenA > lenB) else A)
        lessArray = (B if (lenA <= lenB) else A)
        index=0
        last = False
        for i in lessArray:
            if last:
                moreArray.append(i)
            else:
                last = True
                for j in range(index,len(moreArray)):
                    if i < moreArray[j]:
                        moreArray.insert(j,i)
                        last = False
                        index=j+1;
                        break
                if last:
                    moreArray.append(i)
        return moreArray



def isToeplitzMatrix(matrix):
        row = len(matrix)
        if row < 1:
            return False

        col = len(matrix[0])
        if col < 1:
            return False

        for i in matrix:
            if len(i) != col:
                return False

        s_x = 0
        s_y = row
        for index in range(1,row+col):
            x = 0
            y = s_y - index
            if y < 0:
                x = -y
                y=0
            value=matrix[y][x]
            while(x < col and y < row):
                if value != matrix[y][x]:
                    return False
                x += 1
                y += 1

        return True

def rotateString(str, offset):
    res = str[-offset:]+str[0:len(str) - offset]
    return res


def fizzBuzz(n):
        res = []
        for i in range(1,n+1):
            if i % 3 == 0 and i % 5 == 0:
                res.append("fizz buzz")
            elif i % 3 == 0:
                res.append("fizz")
            elif i % 5 == 0:
                res.append("buzz")
            else:
                res.append(str(i))
        return res

def test():
    for i in range(1,11):
        print(i,end=' ')
    else:
        print("\n=======")

n1 = [9]
n2 = [5,7]
# n = [[1,2,3,4],[5,1,2,3],[9,5,1,2]]
n = "abcdefg"

test()

# res = fizzBuzz(15)
# print(res)

# res = [nthUglyNumber(i + 1) for i in range(n)]
# for i in range(22):
#     res = nthUglyNumber(i+1)
# print(res)

