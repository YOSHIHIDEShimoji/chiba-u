import random

def bubble_sort(l):
    for i in range(len(l) - 1):
        for j in range(len(l) - 1):
           if l[j] > l[j + 1]:
               l[j], l[j + 1] = l[j + 1], l[j]
               j += 1

def main():
    l = []
    for _ in range(10):
        v = random.randrange(-100, 100)
        l.append(v)
    print('before:', l)
    bubble_sort(l)
    print('after: ', l)
    
if __name__ == '__main__':
    main()

    