import random

def bubble_sort(num):
    print(num)
    for i in range(9):
        for j in range(i, 9):
            if num[i] > num[i + 1]:
                num[i], num[i + 1] = num[i + 1], num[i]

    print(num)
            


def main():
    num = []
    for _ in range(10):
        v = random.randrange(0, 100)
        num.append(v)

    bubble_sort(num)
    
if __name__ == '__main__':
    main()

    