import sys

input_num = int(sys.stdin.readline())
count = 0

def pivonachi(num: int):
    global count

    count += 1
    if num == 0:
        return 0
    elif num <= 2:
        return 1
    else:
        return pivonachi(num-1) + pivonachi(num-2)


print(pivonachi(input_num))
print(count)